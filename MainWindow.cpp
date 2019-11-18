#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtWidgets>
#include <QRubberBand>

#include "WorkspaceArea.h"
#include "Palette/Brush.h"
#include "Palette/Histogram.h"
#include "Palette/ColorControls.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
     * Adds a toolbar on runtime, layed out vertically, aligned to the right
     */
    // 1. Set the toolbar dimensions
    ui->toolBar->setIconSize(QSize(55, 55));
    ui->toolBar->setFixedHeight(60);
    ui->toolBar->setContentsMargins(30, 0, 30, 0);
    ui->toolBar->setStyleSheet("QToolBar{spacing:30px;}");

    // 2. Setup the toolbar alignment
    QWidget* spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);

    // 3. Adds actions to the toolbar
    ui->toolBar->addWidget(spacerWidget);
    ui->toolBar->addAction(ui->actionNew);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionOpen);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionSave);

    // Setup our actions shortcuts
    ui->actionNew->setShortcuts(QKeySequence::New);
    ui->actionOpen->setShortcuts(QKeySequence::Open);
    ui->actionSave->setShortcuts(QKeySequence::Save);
    ui->actionUndo->setShortcuts(QKeySequence::Undo);
    ui->actionPrint->setShortcuts(QKeySequence::Print);
    ui->actionHistory->setShortcut(tr("Ctrl+H"));
    ui->actionExit->setShortcuts(QKeySequence::Quit);

    ///////////////////////////////////////////////////////////////////////////////////////////

    // Spawns a WorkspaceArea
    workspaceArea = new WorkspaceArea;

    // Adds the workspaceArea into our graphicsView
    graphicsView = new QGraphicsView(this);
    graphicsView->setScene(workspaceArea);
    ui->workspaceView->addWidget(graphicsView);
    workspaceArea->setParent(graphicsView);

    ///////////////////////////////////////////////////////////////////////////////////////////

    // Create additional actions and menus
    createActions();
    createMenus();

    // Setup zoom options
    QComboBox* comboBox = new QComboBox(this);
    comboBox->addItem("50%");
    comboBox->addItem("100%");
    comboBox->addItem("120%");
    comboBox->setCurrentText("100%");
    ui->statusBar->addWidget(comboBox);

    connect(comboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onZoom(const QString&)));

    ///////////////////////////////////////////////////////////////////////////////////////////

    // Setup a treeWidget, which are the menus in our palette
    ui->palette->setColumnCount(1);
    histogram       = new QTreeWidgetItem(ui->palette);
    basicControls   = new QTreeWidgetItem(ui->palette);
    colorControls   = new QTreeWidgetItem(ui->palette);
    brushControls   = new QTreeWidgetItem(ui->palette);
    effects         = new QTreeWidgetItem(ui->palette);

    /*
     * Fills in our treeWidget palette with all the widgets it has
     */
    // 1. Adds a Histogram widget to the treeWidget palette
    addRoot(histogram, "Histogram");
    histo = new Histogram();
    customAddChild(histogram, histo);

    // 2. Adds a BasicControls widget to the treeWidget palette
    addRoot(basicControls, "Basic Controls");
    basics = new BasicControls();
    customAddChild(basicControls, basics);

    // 3. Adds a ColorControls widget to the treeWidget palette
    addRoot(colorControls, "Color");
    colors = new ColorControls();
    customAddChild(colorControls, colors);

    // 4. Adds a BrushControls widget to the treeWidget palette
    addRoot(brushControls, "Brush");
    brush = new Brush();
    customAddChild(brushControls, brush);

    // TODO 5. Adds an Effects widget to the treeWidget palette
    addRoot(effects, "Effects");

    ///////////////////////////////////////////////////////////////////////////////////////////

    // Setup all our signal and slots
    reconnectConnection();
    connect(basics, &BasicControls::crossCursorSignal, this, &MainWindow::crossCursorChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Setup all connections related to the workspaceArea
// This is needed in order to reestablish connection after workspaceArea is recreated
void MainWindow::reconnectConnection()
{
    connect(workspaceArea, &WorkspaceArea::edit, this, &MainWindow::on_edit);
    connect(workspaceArea, &WorkspaceArea::onImageLoaded, histo, &Histogram::imageLoaded);
    connect(brush, &Brush::onPenColorChanged, workspaceArea, &WorkspaceArea::setPenColor);
    connect(brush, &Brush::onPenWidthChanged, workspaceArea, &WorkspaceArea::setPenWidth);    
}

/*
 * Delete the current workspaceArea and recreate it with the new image dimensions
 * Then resetup our graphicsView
 */
void MainWindow::reconstructWorkspaceArea(int imageWidth, int imageHeight){
    if(workspaceArea != nullptr) {
        delete workspaceArea;
        workspaceArea = nullptr;
    }

    workspaceArea = new WorkspaceArea(imageWidth, imageHeight);
    reconnectConnection();

    graphicsView->setScene(workspaceArea);
    ui->workspaceView->addWidget(graphicsView);
    workspaceArea->setParent(graphicsView);
}

/*
 * Resize our graphicsView dimensions to a specified width and height.
 * This is done usually after we reconstruct our workspaceArea
 */
void MainWindow::resizeGraphicsViewBoundaries(int newWidth, int newHeight)
{
    graphicsView->setFixedSize(newWidth, newHeight);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    graphicsView->setFocusPolicy(Qt::NoFocus);
    graphicsView->setSceneRect(0, 0, newWidth - 2, newHeight - 2);    // Allow for extra 2px boundaries
}

// Adds a root into a parent widget, in the palette treeWidget
void MainWindow::addRoot(QTreeWidgetItem* parent, QString name)
{
    parent->setText(0, name);
    ui->palette->addTopLevelItem(parent);
}

/*
 * Adds a child to a parent, this is a custom implementation of the addChild function
 * This child is a QWidget
 */
void MainWindow::customAddChild(QTreeWidgetItem* parent, QWidget* widget)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    parent->addChild(item);
    ui->palette->setItemWidget(item, 0, widget);
}

// When user is closing the app, spawn a "do you want to save" widget
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();    // If no changes have been made and the app closes
    }
    else {
        event->ignore();    // If there have been changes ignore the event
    }
}

/*
 * Called when the user clicks Save As in the menu,
 * then this function will call the saveAsFile function to actually save the image with the right format
 */
void MainWindow::saveAs()
{
    QAction *action = qobject_cast<QAction *>(sender());        // Represents the action of the user clicking
    QByteArray defaultFormat("jpg", 3);                         // Sets default
    fileFormat = action->data().toByteArray();                  // Stores the array of bytes of the users data
    if (fileFormat.isEmpty()) {
        fileFormat = defaultFormat;
    }

    saveAsFile(fileFormat);
}

// clears the workspaceArea
void MainWindow::clearImage()
{
    reconstructWorkspaceArea(workspaceArea->getImageWidth(), workspaceArea->getImageHeight());
    workspaceArea->setModified(true);
    update();
    workspaceArea ->setImageLoaded(false);
}

// Define menu actions for SaveAs and clearScreen
void MainWindow::createActions()
{
    // Get a list of the supported file formats
    // QImageWriter is used to write images to files
    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);                      // Create an action for each file format

        action->setData(format);                                        // Set an action for each file format
        saveAsActs.append(action);                                      // Attach each file format option menu item to Save As
        connect(action, SIGNAL(triggered()), this, SLOT(saveAs()));
    }

    // Create clear screen action and tie to ScribbleWindow::clearImage()
    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), this, SLOT(clearImage()));
}

// Create additional menus for certain actions, i.e. clearScreen action
void MainWindow::createMenus()
{
    // Create Save As option and the list of file types
    foreach (QAction *action, saveAsActs)
        ui->menuSave_As->addAction(action);

    // Attach all actions to Options
    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(clearScreenAct);

    menuBar()->addMenu(optionMenu);
}

/*
 * maybeSave will prompt users to save the file when user tried to quit/change image
 * maybeSave return true if there has been no changes
 */
bool MainWindow::maybeSave()
{
    // Check for changes since last save
    if (workspaceArea->isModified()) {
        QMessageBox::StandardButton ret;

        ret = QMessageBox::warning(this, tr("Scribble"),
                                   tr("The image has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return saveAsFile("png");
        }
        else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

// Saves the current image with the specified fileFormat, return true if image saved, otherwise false.
bool MainWindow::saveAsFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    // Add the proper file formats and extensions
    fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    initialPath,
                                                    tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                    .arg(QString::fromLatin1(fileFormat)));

    if (fileName.isEmpty()) {
        return false;
    }
    else {
        bool saved = workspaceArea->saveImage(fileName, fileFormat.constData());    // Call for the file to be saved
        if (saved) {
            fileSaved = true;
        }
        else {
            fileSaved = false;
        }
        return saved;
    }
}

// Clears the workspaceArea
void MainWindow::on_actionNew_triggered()
{
    if (maybeSave()) {
        clearImage();
    }
    workspaceArea->setModified(false);
}

// Check if the current image has been changed and then open a dialog to open a file
void MainWindow::on_actionOpen_triggered()
{
    if (maybeSave()) {      // Check if changes have been made since last save
        // Get the file to open from a dialog
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        QImage loadedImage;
        if (!fileName.isEmpty()){
            if (!loadedImage.load(fileName)) {
                return;
            }
            this->fileName = fileName;
            fileSaved = false;

            QImageReader reader(fileName);
            QSize sizeOfImage = reader.size();
            int imageHeight = sizeOfImage.height();
            int imageWidth = sizeOfImage.width();

            reconstructWorkspaceArea(imageWidth, imageHeight);

            workspaceArea->openImage(loadedImage, imageWidth, imageHeight);
            resizeGraphicsViewBoundaries(workspaceArea->getImageWidth(), workspaceArea->getImageHeight());
        }
    }
}

// Tries to save the file instead of saveAs the file, if it has once been saved.
void MainWindow::on_actionSave_triggered()
{
    if (fileSaved) {
        bool saved = workspaceArea->saveImage(fileName, fileFormat.constData());    // Call for the file to be saved
        if (saved) {
            fileSaved = true;
        }
        else {
            fileSaved = false;
        }
    }
    else {
        saveAs();
    }
}

// Undo brush stroke
void MainWindow::on_actionUndo_triggered()
{
    if (history.isEmpty()) {
        return;
    }
    QGraphicsPathItem* toBeDeleted = history.back();
    history.pop_back();
    workspaceArea->removeItem(toBeDeleted);
}

// Print the workspaceArea
void MainWindow::on_actionPrint_triggered()
{
    on_actionSave_triggered();
    workspaceArea->print();
}

// Shows About us widget when slot triggered
void MainWindow::on_actionAbout_Us_triggered()
{
    AboutUs aboutUs;
    aboutUs.setModal(true);
    aboutUs.exec();
}

void MainWindow::wheelEvent(QWheelEvent* event) {
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.1;
    graphicsView->centerOn(0, 0);
    int workspaceAreaWidth = workspaceArea->getImageWidth();
    int workspaceAreaHeight = workspaceArea->getImageHeight();
    if (event->orientation() == Qt::Vertical) {
        if (event->delta() > 7 && workspaceAreaWidth < 1920 && workspaceAreaHeight < 1080) {
            int a = static_cast<int>(workspaceAreaWidth * scaleFactor);
            int b = static_cast<int>(workspaceAreaHeight * scaleFactor);
            graphicsView->scale(static_cast<double>(a) / workspaceAreaWidth, static_cast<double>(b) / workspaceAreaHeight);
            resizeGraphicsViewBoundaries(static_cast<int>(workspaceAreaWidth * scaleFactor), static_cast<int>(workspaceAreaHeight * scaleFactor));
            workspaceArea->resize(workspaceAreaWidth * scaleFactor, workspaceAreaHeight * scaleFactor);
            currentZoom *= scaleFactor;
        }
        else if (event->delta() < -7 && workspaceArea->getImageWidth() > 200 && workspaceArea->getImageHeight() > 200) {
            int a = workspaceAreaWidth * (1.0 / scaleFactor);
            int b = workspaceAreaHeight * (1.0 / scaleFactor);
            graphicsView->scale(static_cast<double>(a) / workspaceAreaWidth, (double) b / (double) workspaceAreaHeight);
            resizeGraphicsViewBoundaries(workspaceAreaWidth * (1.0 / scaleFactor), workspaceAreaHeight * (1.0 / scaleFactor));
            workspaceArea->resize(workspaceAreaWidth * (1.0 / scaleFactor), workspaceAreaHeight * (1.0 / scaleFactor));
            currentZoom /= scaleFactor;
        }
    }
    graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
}


void MainWindow::onZoom(const QString& level) {

    double originalFactor = 1.0 / currentZoom;
    int a = workspaceArea->getImageWidth()*originalFactor;
    int b = workspaceArea->getImageHeight()*originalFactor;
    graphicsView->scale((double) a / (double) workspaceArea->getImageWidth(), (double) b / (double) workspaceArea->getImageHeight());
    resizeGraphicsViewBoundaries(workspaceArea->getImageWidth()*originalFactor, workspaceArea->getImageHeight()*originalFactor);
    workspaceArea->resize(workspaceArea->getImageWidth()*originalFactor, workspaceArea->getImageHeight()*originalFactor);
    graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);


    double scaleFactor = 1.0;

    if (level == "50%") {
        scaleFactor = 0.5;
        currentZoom = 0.5;
    } else if (level == "100%") {
        scaleFactor = 1.0;
        currentZoom = 1.0;
    } else if (level == "120%") {
        scaleFactor = 1.2;
        currentZoom = 1.2;
    }

    a = workspaceArea->getImageWidth()*scaleFactor;
    b = workspaceArea->getImageHeight()*scaleFactor;
    graphicsView->scale((double) a / (double) workspaceArea->getImageWidth(), (double) b / (double) workspaceArea->getImageHeight());
    resizeGraphicsViewBoundaries(workspaceArea->getImageWidth()*scaleFactor, workspaceArea->getImageHeight()*scaleFactor);
    workspaceArea->resize(workspaceArea->getImageWidth()*scaleFactor, workspaceArea->getImageHeight()*scaleFactor);


}

void MainWindow::crossCursorChanged(bool cross)
{
    if(cross){
        graphicsView->setCursor(Qt::CrossCursor);
    }
    else {
        graphicsView->setCursor(Qt::ArrowCursor);
    }
}


