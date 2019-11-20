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
    graphicsView = new QGraphicsView(this);
    // Spawns a graphicsView
    workspaceArea = new WorkspaceArea();

    // Adds the workspaceArea into our graphicsView
    graphicsView->setScene(workspaceArea);
    ui->workspaceView->addWidget(graphicsView);
    workspaceArea->setParent(graphicsView);
    graphicsView->scene()->installEventFilter(this);

    ///////////////////////////////////////////////////////////////////////////////////////////

    // Create additional actions and menus
    createActions();
    createMenus();

    // Setup zoom options
    comboBox = new QComboBox(this);
    comboBox->addItem("50%");
    comboBox->addItem("100%");
    comboBox->addItem("120%");
    comboBox->addItem("Fit to screen");
    comboBox->setCurrentText("100%");
    ui->statusBar->addWidget(comboBox);

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
    reconnectConnection();                                                                                      // Workspace related connection
    connect(basics, &BasicControls::crossCursorChanged, this, &MainWindow::onCrossCursorChanged);                  // Cursor change connection
    connect(comboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onZoom(const QString&)));         // Zoom level change connection

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Setup all connections related to the workspaceArea
// This is needed in order to reestablish connection after workspaceArea is recreated
void MainWindow::reconnectConnection()
{
    connect(workspaceArea, &WorkspaceArea::edited, this, &MainWindow::on_edit);
    connect(workspaceArea, &WorkspaceArea::imageLoaded, histo, &Histogram::onImageLoaded);
    connect(brush, &Brush::penColorChanged, workspaceArea, &WorkspaceArea::setPenColor);
    connect(brush, &Brush::penWidthChanged, workspaceArea, &WorkspaceArea::setPenWidth);
    connect(workspaceArea, &WorkspaceArea::imageCropped, this, &MainWindow::onImageCropped);
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
    graphicsView->scene()->installEventFilter(this);

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
        //clearImage();
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

            resizedImageWidth = imageWidth;
            resizedImageHeight = imageHeight;

            workspaceArea->openImage(loadedImage, imageWidth, imageHeight);
            resizeGraphicsViewBoundaries(resizedImageWidth, resizedImageHeight);

            fitImageToScreen(imageWidth, imageHeight);

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

bool MainWindow::eventFilter(QObject*, QEvent* event)
{
    if (event->type() == QEvent::GraphicsSceneWheel) {
        handleWheelEvent(static_cast<QGraphicsSceneWheelEvent*> (event));

        // Don't propagate event to the whole maindinwo
        event->accept();
        return true;
    }
    return false;
}

void MainWindow::handleWheelEvent(QGraphicsSceneWheelEvent *event)
{
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.1;
    graphicsView->centerOn(0, 0);
    int workspaceAreaWidth = resizedImageWidth;
    int workspaceAreaHeight = resizedImageHeight;
    const QRect screenSize = WindowHelper::screenFromWidget(qApp->desktop())->geometry();
    if (event->orientation() == Qt::Vertical) {
        if (event->delta() > 7 && workspaceAreaWidth < screenSize.width() && workspaceAreaHeight < screenSize.height()) {
            resizedImageWidth = static_cast<int>(workspaceAreaWidth * scaleFactor);
            resizedImageHeight = static_cast<int>(workspaceAreaHeight * scaleFactor);
            graphicsView->scale(static_cast<double>(resizedImageWidth) / workspaceAreaWidth, static_cast<double>(resizedImageHeight) / workspaceAreaHeight);
            resizeGraphicsViewBoundaries(resizedImageWidth, resizedImageHeight);
            currentZoom *= scaleFactor;
        }
        else if (event->delta() < -7 && workspaceAreaWidth > 200 && workspaceAreaHeight > 200) {
            resizedImageWidth = static_cast<int>(workspaceAreaWidth * (1.0 / scaleFactor));
            resizedImageHeight = static_cast<int>(workspaceAreaHeight * (1.0 / scaleFactor));
            graphicsView->scale(static_cast<double>(resizedImageWidth) / workspaceAreaWidth, static_cast<double>(resizedImageHeight) / workspaceAreaHeight);
            resizeGraphicsViewBoundaries(resizedImageWidth, resizedImageHeight);
            currentZoom /= scaleFactor;
        }
    }
    graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
}

void MainWindow::onZoom(const QString& level)
{
    if (level == "Fit to screen") {
        fitImageToScreen(resizedImageWidth, resizedImageHeight);
        return;
    }

    double originalFactor = 1.0 / currentZoom;
    int a = static_cast<int>(resizedImageWidth * originalFactor);
    int b = static_cast<int>(resizedImageHeight * originalFactor);
    graphicsView->scale((double) a / (double) resizedImageWidth, (double) b / (double) resizedImageHeight);
    resizeGraphicsViewBoundaries(resizedImageWidth*originalFactor, resizedImageHeight*originalFactor);
    resizedImageWidth *= originalFactor;
    resizedImageHeight *= originalFactor;
    graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);


    double scaleFactor = 1.0;

    if (level == "50%") {
        scaleFactor = 0.5;
        currentZoom = 0.5;
    }
    else if (level == "100%") {
        scaleFactor = 1.0;
        currentZoom = 1.0;
    }
    else if (level == "120%") {
        scaleFactor = 1.2;
        currentZoom = 1.2;
    }

    a = static_cast<int>(resizedImageWidth * scaleFactor);
    b = static_cast<int>(resizedImageHeight * scaleFactor);
    graphicsView->scale((double) a / (double) resizedImageWidth, (double) b / (double) resizedImageHeight);
    resizeGraphicsViewBoundaries(resizedImageWidth*scaleFactor, resizedImageHeight*scaleFactor);
    resizedImageWidth *= scaleFactor;
    resizedImageHeight *= scaleFactor;

    graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
}

void MainWindow::onCrossCursorChanged(bool cross)
{
    if (cross) {
        graphicsView->setCursor(Qt::CrossCursor);
        workspaceArea->setCursorMode(WorkspaceArea::CursorMode::RECTANGLECROP);
    }
    else {
        graphicsView->setCursor(Qt::ArrowCursor);
        workspaceArea->setCursorMode(WorkspaceArea::CursorMode::SCRIBBLE);
    }
}

void MainWindow::fitImageToScreen(int currentWidth, int currentHeight)
{
    const QRect screenSize = WindowHelper::screenFromWidget(qApp->desktop())->geometry();
    int screenWidth = screenSize.width();
    int screenHeight = screenSize.height();

    if (currentWidth > screenWidth || currentHeight > screenHeight) {
        double ratio = qMax((double) screenWidth / (double) currentWidth, (double) screenHeight / (double) currentHeight)*0.5;
        int a = resizedImageWidth*ratio;
        int b = resizedImageHeight*ratio;
        graphicsView->scale((double) a / (double) resizedImageWidth, (double) b / (double) resizedImageHeight);
        resizeGraphicsViewBoundaries(resizedImageWidth*ratio, resizedImageHeight*ratio);
        resizedImageWidth *= ratio;
        resizedImageHeight *= ratio;
        currentZoom *= ratio;
        graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
        comboBox->setCurrentText("Fit to screen");

    } else if (currentWidth < screenWidth || currentHeight < screenHeight) {
        double ratio = qMin((double) screenWidth / (double) currentWidth, (double) screenHeight / (double) currentHeight)*0.5;
        int a = resizedImageWidth*ratio;
        int b = resizedImageHeight*ratio;
        graphicsView->scale((double) a / (double) resizedImageWidth, (double) b / (double) resizedImageHeight);
        resizeGraphicsViewBoundaries(resizedImageWidth*ratio, resizedImageHeight*ratio);
        resizedImageWidth *= ratio;
        resizedImageHeight *= ratio;
        currentZoom *= ratio;
        graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
        comboBox->setCurrentText("Fit to screen");

    }
    centerAndResize();
}

void MainWindow::centerAndResize() {
    // get the dimension available on this screen
    QSize availableSize = WindowHelper::screenFromWidget(qApp->desktop())->availableSize();
    int width = availableSize.width();
    int height = availableSize.height();
    qDebug() << "Available dimensions " << width << "x" << height;
    QSize newSize( width, height );

    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            newSize,
            WindowHelper::screenFromWidget(qApp->desktop())->availableGeometry()
        )
    );
}

void MainWindow::onImageCropped(const QImage& image, int width, int height) {

    qDebug() << "CROPPED";

    //reconstructWorkspaceArea(width, height);

    resizedImageWidth = width;
    resizedImageHeight = height;

    qDebug() << resizedImageWidth << " " << resizedImageHeight;

    workspaceArea->openImage(image, resizedImageWidth, resizedImageHeight);
    resizeGraphicsViewBoundaries(resizedImageWidth, resizedImageHeight);

    fitImageToScreen(resizedImageWidth, resizedImageHeight);
}
