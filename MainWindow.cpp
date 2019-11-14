#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtWidgets>

#include "WorkspaceArea.h"

#include "Palette/Brush.h"

#include "Palette/Histogram.h"

static const int SCENE_WIDTH = 1080;    // The width of the workspace
static const int SCENE_HEIGHT = 1920;   // The height of the workspace

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
     * Add a new toolbar dynamically, with specified fixed height,
     * since we need to add an expanding spacer, and widgets
    */
    ui->toolBar->setIconSize(QSize(55, 55));
    ui->toolBar->setFixedHeight(60);
    ui->toolBar->setContentsMargins(30, 0, 30, 0);
    ui->toolBar->setStyleSheet("QToolBar{spacing:30px;}");

    QWidget* spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);

    ui->toolBar->addWidget(spacerWidget);
    ui->toolBar->addAction(ui->actionNew);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionOpen);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionSave);

    // Setup our actions shortcuts
    ui->actionOpen->setShortcuts(QKeySequence::Open);
    ui->actionExit->setShortcuts(QKeySequence::Quit);

    // Spawns a Workspace Area
    workspaceArea = new WorkspaceArea;

    graphicsView = new QGraphicsView(this);
    graphicsView->setScene(workspaceArea);
    ui->workspaceView->addWidget(graphicsView);
    workspaceArea->setParent(graphicsView);
//    resizeGraphicsViewBoundaries(SCENE_WIDTH, SCENE_HEIGHT);

    connect(workspaceArea, &WorkspaceArea::edit, this, &MainWindow::on_edit);

    // Create actions and menus
    createActions();
    createMenus();

    /*
     * Setup the treeWidget, which will be in our palette
     * TODO: fill each root
     */
    ui->palette->setColumnCount(1);
    histogram = new QTreeWidgetItem(ui->palette);
    basicControls = new QTreeWidgetItem(ui->palette);
    colorControls = new QTreeWidgetItem(ui->palette);
    brushControls = new QTreeWidgetItem(ui->palette);
    effects = new QTreeWidgetItem(ui->palette);

    // TODO: check for canvas size difference.
    addRoot(histogram, "Histogram");
    histo = new Histogram(workspaceArea->getImage());
    customAddChild(histogram, histo);

    // TODO
    addRoot(basicControls, "Basic Controls");

    // TODO
    addRoot(colorControls, "Color");

    addRoot(brushControls, "Brush");
    brush = new Brush();
    customAddChild(brushControls, brush);

    // TODO
    addRoot(effects, "Effects");

    reconnectConnection();

    // Sets the initial dimensions for the palette view and the workspace
    ui->splitter->setStretchFactor(0, 10);
    ui->splitter->setStretchFactor(1, 1);

}

void MainWindow::reconnectConnection()
{
    connect(workspaceArea, &WorkspaceArea::onImageLoaded, histo, &Histogram::imageLoaded);
    connect(brush, &Brush::onPenColorChanged, workspaceArea, &WorkspaceArea::setPenColor);
    connect(brush, &Brush::onPenWidthChanged, workspaceArea, &WorkspaceArea::setPenWidth);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeGraphicsViewBoundaries(int newWidth, int newHeight)
{
    graphicsView->setFixedSize(newWidth, newHeight);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    graphicsView->setFocusPolicy(Qt::NoFocus);
    graphicsView->setSceneRect(0, 0, newWidth - 2, newHeight - 2);    // Allow for extra 2px boundaries
}

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
    // If they try to close maybeSave() returns true
    // if no changes have been made and the app closes
    if (maybeSave()) {
        event->accept();
    } else {

        // If there have been changes ignore the event
        event->ignore();
    }
}

// Check if the current image has been changed and then
// open a dialog to open a file
void MainWindow::open()
{
    // Check if changes have been made since last save
    // maybeSave() returns true if no changes have been made
    if (maybeSave()) {

        // Get the file to open from a dialog
        // QDir opens the current dirctory
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"), QDir::currentPath());

        if (!fileName.isEmpty()){
            if (!loadedImage.load(fileName)) {
                return;
            }
            QImageReader reader(fileName);
            QSize sizeOfImage = reader.size();
            int imageHeight = sizeOfImage.height();
            int imageWidth = sizeOfImage.width();

            if(workspaceArea != nullptr) {
                delete workspaceArea;
                workspaceArea = nullptr;
            }

            workspaceArea = new WorkspaceArea(imageWidth, imageHeight);
            reconnectConnection();

            graphicsView->setScene(workspaceArea);
            ui->workspaceView->addWidget(graphicsView);
            workspaceArea->setParent(graphicsView);

            workspaceArea->openImage(loadedImage, imageWidth, imageHeight);
            resizeGraphicsViewBoundaries(workspaceArea->getImageWidth(), workspaceArea->getImageHeight());
        }
    }
}

// Called when the user clicks Save As in the menu
void MainWindow::save()
{
    // A QAction represents the action of the user clicking
    QAction *action = qobject_cast<QAction *>(sender());

    // Stores the array of bytes of the users data
    QByteArray fileFormat = action->data().toByteArray();

    // Pass it to be saved
    saveFile(fileFormat);
}

// Define menu actions that call functions
void MainWindow::createActions()
{
    // Get a list of the supported file formats
    // QImageWriter is used to write images to files
    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        // Create an action for each file format
        QAction *action = new QAction(text, this);

        // Set an action for each file format
        action->setData(format);

        // When clicked call ScribbleWindow::save()
        connect(action, SIGNAL(triggered()), this, SLOT(save()));

        // Attach each file format option menu item to Save As
        saveAsActs.append(action);
    }

    // Create clear screen action and tie to ScribbleWindow::clearImage()
    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()),
            workspaceArea, SLOT(clearImage()));
}

// Create the menubar
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
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return saveFile("png");
        }
        else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    // Add the proper file formats and extensions
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    initialPath,
                                                    tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                    .arg(QString::fromLatin1(fileFormat)));

    if (fileName.isEmpty()) {
        return false;
    }
    else {
        // Call for the file to be saved
        return workspaceArea->saveImage(fileName, fileFormat.constData());
    }
}

void MainWindow::on_actionNew_triggered()
{
    if (maybeSave()) {
        workspaceArea->clearImage();
    }
}

void MainWindow::on_actionPrint_triggered()
{
    workspaceArea->print();
}

void MainWindow::on_actionOpen_triggered()
{
    open();
}

void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}

void MainWindow::on_actionAbout_Us_triggered()
{
    AboutUs aboutUs;
    aboutUs.setModal(true);
    aboutUs.exec();
}

void MainWindow::on_actionUndo_triggered()
{
    QGraphicsPathItem* toBeDeleted = history.back();
    history.pop_back();
    workspaceArea->removeItem(toBeDeleted);
}

void MainWindow::on_edit(QGraphicsPathItem* item) {
    history.push_back(item);
}
