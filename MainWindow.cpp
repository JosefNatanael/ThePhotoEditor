#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtWidgets>

#include "ScribbleArea.h"

#include "Palette/Brush.h"

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

    /*
     * Setup the treeWidget, which will be in our palette
     * TODO: fill each root
     */
    ui->palette->setColumnCount(1);
    QTreeWidgetItem* histogram = new QTreeWidgetItem(ui->palette);
    QTreeWidgetItem* basicControls = new QTreeWidgetItem(ui->palette);
    QTreeWidgetItem* colorControls = new QTreeWidgetItem(ui->palette);
    QTreeWidgetItem* brushControls = new QTreeWidgetItem(ui->palette);
    QTreeWidgetItem* effects = new QTreeWidgetItem(ui->palette);

    // TODO
    addRoot(histogram, "Histogram");

    // TODO
    addRoot(basicControls, "Basic Controls");

    // TODO
    addRoot(colorControls, "Color");

    addRoot(brushControls, "Brush");
    Brush* brush = new Brush();
    customAddChild(brushControls, brush);

    // TODO
    addRoot(effects, "Effects");

    // Sets the initial dimensions for the palette view and the workspace
    ui->splitter->setStretchFactor(0, 10);
    ui->splitter->setStretchFactor(1, 1);

    // Spawns a Scribble Area
    scribbleArea = new ScribbleArea;
    ui->workspaceView->addWidget(scribbleArea);

    // Create actions and menus
    createActions();
    createMenus();

    // TODO: Connect all the palette components

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addRoot(QTreeWidgetItem* parent, QString name)
{
    parent->setText(0, name);
    ui->palette->addTopLevelItem(parent);
}

void MainWindow::customAddChild(QTreeWidgetItem* parent, QWidget* widget)
{
    QTreeWidgetItem* item = new QTreeWidgetItem();
    parent->addChild(item);
    ui->palette->setItemWidget(item, 0, widget);
}

void MainWindow::on_actionPrint_triggered()
{
    scribbleArea->print();
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

// When user is closing the app, spawn a "do you want to save" widge
void MainWindow::closeEvent(QCloseEvent *event)
{
    // If they try to close maybeSave() returns true
    // if no changes have beaen made and the app closes
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
        // tr sets the window title to Open File
        // QDir opens the current dirctory
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"), QDir::currentPath());

        // If we have a file name load the image and place
        // it in the scribbleArea
        if (!fileName.isEmpty())
            scribbleArea->openImage(fileName);
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

// Opens a dialog to change the pen color
void MainWindow::penColor()
{
    // Store the chosen color from the dialog
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());

    // If a valid color set it
    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}

// Opens a dialog that allows the user to change the pen width
void MainWindow::penWidth()
{
    // Stores button value
    bool ok;

    // Get the current pen width
    // Define the min, max, step and ok button
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width:"),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    // Change the pen width
    if (ok)
        scribbleArea->setPenWidth(newWidth);
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

    // Create pen color action and tie to ScribbleWindow::penColor()
    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    // Create pen width action and tie to ScribbleWindow::penWidth()
    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    // Create clear screen action and tie to ScribbleWindow::clearImage()
    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()),
            scribbleArea, SLOT(clearImage()));
}

// Create the menubar
void MainWindow::createMenus()
{
    // Create Save As option and the list of file types
    foreach (QAction *action, saveAsActs)
        ui->menuSave_As->addAction(action);

    // Attach all actions to Options
    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    // Add menu items to the menubar
    menuBar()->addMenu(optionMenu);
}

bool MainWindow::maybeSave()
{
    // Check for changes since last save
    if (scribbleArea->isModified()) {
        QMessageBox::StandardButton ret;

        // Scribble is the title
        // Add text and the buttons
        ret = QMessageBox::warning(this, tr("Scribble"),
                                   tr("The image has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                   | QMessageBox::Cancel);

        // If save button clicked call for file to be saved
        if (ret == QMessageBox::Save) {
            return saveFile("png");

            // If cancel do nothing
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    // Define path, name and default file type
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    // Get selected file from dialog
    // Add the proper file formats and extensions
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    initialPath,
                                                    tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                    .arg(QString::fromLatin1(fileFormat)));

    // If no file do nothing
    if (fileName.isEmpty()) {
        return false;
    } else {

        // Call for the file to be saved
        return scribbleArea->saveImage(fileName, fileFormat.constData());
    }
}
