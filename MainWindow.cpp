/*!
 *  @author         Ferris Prima Nugraha
 *  @author         Josef Natanael
 *  @author         Alvin Harjanto
 *  @version        1.0.0
 *  @date           2019
 *  @copyright      GNU Public License.
 *  @mainpage       The Photo Editor
 *  @section        intro_sec Introduction
 *  This code is developed to implement version control and multiplayer support to a standard photo editing software.
 *  @section        compile_sec Compilation
 *  Here I will describe how to compile this code with Qt
 *  1. Load .pro project file to QtCreator
 *  2. Select kits (Tested with MinGW 5.10.0 and above)
 *  3. Build and run
 *  @class MainWindow
 *  @brief Central hub for all functions and classes in this application.
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtWidgets>
#include <QRubberBand>
#include "FilterTransform/NonKernelBased/MagicWand.h"
#include <QJsonObject>

#include "WorkspaceArea.h"
#include "Palette/BasicControls.h"
#include "Palette/Brush.h"
#include "Palette/Histogram.h"
#include "Palette/ColorControls.h"
#include "Palette/Effects.h"

#include "FilterTransform/NonKernelBased/GrayscaleFilter.h"
#include "FilterTransform/NonKernelBased/InvertFilter.h"

#include "FilterTransform/NonKernelBased/HueFilter.h"
#include "FilterTransform/NonKernelBased/SaturationFilter.h"
#include "FilterTransform/NonKernelBased/TemperatureFilter.h"
#include "FilterTransform/NonKernelBased/TintFilter.h"

#include "FilterTransform/NonKernelBased/BrightnessFilter.h"
#include "FilterTransform/NonKernelBased/ContrastFilter.h"
#include "FilterTransform/NonKernelBased/ExposureFilter.h"

#include "FilterTransform/NonKernelBased/ClockwiseRotationTransform.h"
#include "FilterTransform/NonKernelBased/CounterClockwiseRotationTransform.h"
#include "FilterTransform/NonKernelBased/FlipHorizontalTransform.h"
#include "FilterTransform/NonKernelBased/FlipVerticalTransform.h"

#include "FilterTransform/KernelBased/GaussianBlurFilter.h"
#include "FilterTransform/KernelBased/MeanBlurFilter.h"
#include "FilterTransform/KernelBased/EmbossFilter.h"
#include "FilterTransform/KernelBased/EdgeDetectionFilter.h"
#include "FilterTransform/KernelBased/ImageInpainting.h"
#include "FilterTransform/KernelBased/ImageScissors.h"

#include "ServerRoom.h"

/**
 * @brief Construct a new Main Window::MainWindow object.
 * 
 * @param parent to be passed to QMainWindow(parent) constructor.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
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
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);

    // 3. Adds actions to the toolbar
    ui->toolBar->addWidget(spacerWidget);
    ui->toolBar->addAction(ui->actionNew);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionOpen);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionSave);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionCommit_Changes);

    // Setup our actions shortcuts
    ui->actionNew->setShortcuts(QKeySequence::New);
    ui->actionOpen->setShortcuts(QKeySequence::Open);
    ui->actionSave->setShortcuts(QKeySequence::Save);
    ui->actionUndo->setShortcuts(QKeySequence::Undo);
    ui->actionRedo->setShortcuts(QKeySequence::Redo);
    ui->actionPrint->setShortcuts(QKeySequence::Print);
    ui->actionExit->setShortcuts(QKeySequence::Quit);

    ///////////////////////////////////////////////////////////////////////////////////////////

    // Spawns a graphicsView
    graphicsView = new QGraphicsView(this);
    // Spawns a WorkspaceArea
    workspaceArea = new WorkspaceArea();

    // Adds the workspaceArea into our graphicsView
    graphicsView->setScene(workspaceArea);
    ui->workspaceView->addWidget(graphicsView);
    workspaceArea->setParent(graphicsView);
    graphicsView->scene()->installEventFilter(this);

    // Sets the graphicsViewBoundaries at launch, to the default width and height of the workspaceArea
    resizeGraphicsViewBoundaries(WorkspaceArea::SCENE_WIDTH, WorkspaceArea::SCENE_HEIGHT);

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
    histogram = new QTreeWidgetItem(ui->palette);
    basicControls = new QTreeWidgetItem(ui->palette);
    colorControls = new QTreeWidgetItem(ui->palette);
    brushControls = new QTreeWidgetItem(ui->palette);
    effects = new QTreeWidgetItem(ui->palette);

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
    effect = new Effects();
    customAddChild(effects, effect);
    ///////////////////////////////////////////////////////////////////////////////////////////

    // Setup all our signal and slots
    reconnectConnection();                                                                                // Workspace related connection
    connect(basics, &BasicControls::crossCursorChanged, this, &MainWindow::onCrossCursorChanged);         // Cursor change connection
    connect(basics, &BasicControls::applyTransformClicked, this, &MainWindow::applyFilterTransform);      // Image transformation connection to basic controls
    connect(colors, &ColorControls::applyColorFilterClicked, this, &MainWindow::applyFilterTransform);    // Image filters connection to color controls
    connect(effect, &Effects::applyEffectClicked, this, &MainWindow::applyFilterTransform);               // Image effects connection to effects widget
    connect(comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(onZoom(const QString &))); // Zoom level change connection
    connect(colors, &ColorControls::applyColorFilterOnPreview, this, &MainWindow::applyFilterTransformOnPreview);
}

/**
 * @brief Destroy the Main Window::MainWindow object.
 * 
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete temporaryArea;
}

/**
 * @brief Setup all connections related to the workspaceArea.
 * @details This is need in order to reestablish connection of signal and slots to the a new workspaceArea, i.e. workspaceArea is recreated.
 */
void MainWindow::reconnectConnection()
{
    connect(workspaceArea, &WorkspaceArea::imageDrawn, this, &MainWindow::onImageDrawn);
    connect(workspaceArea, &WorkspaceArea::imageLoaded, histo, &Histogram::onImageLoaded);
    connect(brush, &Brush::penColorChanged, workspaceArea, &WorkspaceArea::setPenColor);
    connect(brush, &Brush::penWidthChanged, workspaceArea, &WorkspaceArea::setPenWidth);
    connect(workspaceArea, &WorkspaceArea::imageCropped, this, &MainWindow::rerenderWorkspaceArea);
    connect(basics, &BasicControls::resizeButtonClicked, workspaceArea, &WorkspaceArea::resizeImage);
    connect(workspaceArea, &WorkspaceArea::imageResized, this, &MainWindow::rerenderWorkspaceArea);
    connect(workspaceArea, &WorkspaceArea::sendResize, this, &MainWindow::onSendResize);
    connect(workspaceArea, &WorkspaceArea::sendCrop, this, &MainWindow::onSendCrop);
    connect(workspaceArea, &WorkspaceArea::sendCropWithMagicWand, this, &MainWindow::onSendCropWithMagicWand);
    connect(workspaceArea, &WorkspaceArea::sendMoveScribble, this, &MainWindow::onSendMoveScribble);
    connect(workspaceArea, &WorkspaceArea::sendReleaseScribble, this, &MainWindow::onSendReleaseScribble);
    connect(workspaceArea, &WorkspaceArea::updateImagePreview, this, &MainWindow::onUpdateImagePreview);
    connect(workspaceArea, &WorkspaceArea::commitChanges, this, &MainWindow::onCommitChanges);
}

/**
 * @brief Reconstruct workspaceArea when changes are made to the image.
 * 
 * @details Delete the current workspaceArea and recreates it with the new image dimensions,
 * Then resetup our graphicsView.
 * 
 * @param imageWidth reconstruct the workspaceArea with width imageWidth.
 * @param imageHeight reconstruct the workspaceArea with height imageHeight.
 */
void MainWindow::reconstructWorkspaceArea(int imageWidth, int imageHeight)
{
    delete workspaceArea;
    workspaceArea = nullptr;

    workspaceArea = new WorkspaceArea(imageWidth, imageHeight);
    reconnectConnection();

    // Re-Setup graphicsView.
    graphicsView->setScene(workspaceArea);
    ui->workspaceView->addWidget(graphicsView);
    workspaceArea->setParent(graphicsView);
    graphicsView->scene()->installEventFilter(this);
}

/**
 * @brief Resize graphics view dimensions and boundaries, to fit the workspaceArea.
 * 
 * @details Resize our graphicsView dimensions to a specified width and height.
 * This is done usually after we reconstruct our workspaceArea.
 * 
 * @param newWidth
 * @param newHeight
 */
void MainWindow::resizeGraphicsViewBoundaries(int newWidth, int newHeight)
{
    graphicsView->setFixedSize(newWidth, newHeight);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    graphicsView->setFocusPolicy(Qt::NoFocus);
    graphicsView->setSceneRect(0, 0, newWidth - 2, newHeight - 2);          // Allow for extra 2px boundaries, 1px on the left/top and 1px on the right/bottom.
}

/**
 * @brief Adds a root into a parent QTreeWidgetItem, located in the palette QTreeWidget.
 * 
 * @param parent QTreeWidgetItem to insert to.
 * @param name Name of the root.
 */
void MainWindow::addRoot(QTreeWidgetItem *parent, QString name)
{
    parent->setText(0, name);
    ui->palette->addTopLevelItem(parent);
}

/**
 * @brief Custom implementation of the addChild function of a QTreeWidgetItem
 * 
 * @details Adds a child to a parent, the child is a QWidget.
 * 
 * @param parent The parent QTreeWidgetItem to insert child to.
 * @param widget The QWidget/child.
 */
void MainWindow::customAddChild(QTreeWidgetItem *parent, QWidget *widget)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    parent->addChild(item);
    ui->palette->setItemWidget(item, 0, widget);
}

/**
 * @brief Spawns a "do you want to save" widget when closing the app.
 * 
 * @param event "Save" or "Discard" event.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        event->accept(); // If no changes have been made and the app closes
    }
    else
    {
        event->ignore(); // If there have been changes ignore the event
    }
}

/**
 * @brief Called when the user clicks Save As in the menu.
 * 
 * @details This function will call the saveAsfile function to save the image with the right format.
 * Default format is "jpg".
 */
void MainWindow::saveAs()
{
    QAction *action = qobject_cast<QAction *>(sender());        // Represents the action of the user clicking.
    QByteArray defaultFormat("jpg", 3);                         // Sets default format.
    fileFormat = action->data().toByteArray();                  // Stores the array of bytes of the users data.
    if (fileFormat.isEmpty())
    {
        fileFormat = defaultFormat;
    }

    saveAsFile(fileFormat);                                     // Pass fileFormat to be saved.
}

/**
 * @brief Clears the workspaceArea.
 * 
 * @details this will not invoke maybeSave(), nor commit changes to version control. 
 */
void MainWindow::clearImage()
{
    reconstructWorkspaceArea(workspaceArea->getImageWidth(), workspaceArea->getImageHeight());
    workspaceArea->setModified(true);
    update();
    workspaceArea->setImageLoaded(false);
}

/**
 * @brief Create menu actions for SaveAs and clearScreen.
 */
void MainWindow::createActions()
{
    // Get a list of the supported file formats
    // QImageWriter is used to write images to files
    foreach (QByteArray format, QImageWriter::supportedImageFormats())
    {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this); // Create an action for each file format

        action->setData(format);   // Set an action for each file format
        saveAsActs.append(action); // Attach each file format option menu item to Save As
        connect(action, SIGNAL(triggered()), this, SLOT(saveAs()));
    }

    // Create clear screen action and tie to MainWindow::clearImage()
    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), this, SLOT(on_actionNew_triggered()));
}

/**
 * @brief Generate image history/version control in the menu bar.
 */
void MainWindow::generateHistoryMenu()
{
    // Clean image history menu
    for (QMenu *_ : imageHistoryMenu)
    {
        delete imageHistoryMenu[0];
        imageHistoryMenu.pop_front();
    }

    // Traverse the imageHistory (Version Control)
    QLinkedList<VersionControl::MasterNode>::iterator it = imageHistory.masterBranch.begin();
    // 1a. Traverse nodes in master branch
    for (int masterNodeNumber = 0; it != imageHistory.masterBranch.end(); ++it, ++masterNodeNumber)
    {

        // 1b. Create a menu for each master node
        QMenu *masterNodeMenu = new QMenu(it->changes, this);

        // 2a. Traverse images in side branches
        QLinkedList<VersionControl::SideNode>::iterator i = it->sideBranch.begin();
        for (int sideNodeNumber = 0; i != it->sideBranch.end(); ++i, ++sideNodeNumber)
        {

            // 2b. Create an action for each image
            QAction *action = new QAction(i->changes, masterNodeMenu);
            masterNodeMenu->addAction(action);
            connect(action, &QAction::triggered, this, [=]() { checkoutCommit(masterNodeNumber, sideNodeNumber, true); });
        }

        // 3. Add the menu to our menuHistory
        ui->menuHistory->addMenu(masterNodeMenu);
        imageHistoryMenu.append(masterNodeMenu);
    }
}

/**
 * @brief Checkout a commit. A commit is differentiated by their masterNodeNumber and sideNodeNumber.
 * 
 * @param masterNodeNumber Which branch in the master branch.
 * @param sideNodeNumber Which commit in the branch. 
 */
void MainWindow::checkoutCommit(int masterNodeNumber, int sideNodeNumber, bool fromActionMenu)
{
    this->masterNodeNumber = masterNodeNumber;
    this->sideNodeNumber = sideNodeNumber;
    // Go through the imageHistory (Version Control)
    QLinkedList<VersionControl::MasterNode>::iterator it = imageHistory.masterBranch.begin();
    // 1. Go through nodes in master branch
    it += masterNodeNumber;

    // 2. Go through images in the side branch
    QLinkedList<VersionControl::SideNode>::iterator it2 = it->sideBranch.begin();
    it2 += sideNodeNumber;
    rerenderWorkspaceArea(it2->currentImage, it2->currentImage.width(), it2->currentImage.height());
    if (fromActionMenu) {
        sendVersion("checkoutCommit", masterNodeNumber, sideNodeNumber);
    }
}

/**
 * @brief Commit changes to the version control/image history.
 * 
 * @param changedImage Changed image.
 * @param changes Description of the commit.
 */
void MainWindow::commitChanges(QImage changedImage, QString changes)
{
    if (masterNodeNumber == 0)      // Commit changes to master branch.
    {
        imageHistory.commitChanges(changedImage, changes);
    }
    else        // Commit changes to current branch (i.e. not master branch).
    {
        imageHistory.getMasterNodeIteratorAtIndex(masterNodeNumber)->commitChanges(changedImage, changes);
    }
    generateHistoryMenu();      // update history menu. 
}

/**
 * @brief Create additional menus for certain actions, i.e. clearScreen action.
 */
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

/**
 * @brief Prompt user to save the file when user tried to change image/quit application.
 * 
 * @return true There has been changes/modifications.
 * @return false There was no changes/modifications made.
 */
bool MainWindow::maybeSave()
{
    // Check for changes since last save
    if (workspaceArea->isModified())
    {
        QMessageBox::StandardButton ret;

        ret = QMessageBox::warning(this, tr("Scribble"),
                                   tr("The image has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save)
        {
            return saveAsFile("png");
        }
        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief Saves the current image with the specified fileFormat.
 * 
 * @param fileFormat 
 * @return true Image successfully saved.
 * @return false Image not saved.
 */
bool MainWindow::saveAsFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    // Add the proper file formats and extensions
    fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                            initialPath,
                                            tr("%1 Files (*.%2);;All Files (*)")
                                                .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                .arg(QString::fromLatin1(fileFormat)));

    if (fileName.isEmpty())
    {
        return false;
    }
    else
    {
        bool saved = workspaceArea->saveImage(fileName, fileFormat.constData()); // Call for the file to be saved
        if (saved)
        {
            fileSaved = true;
        }
        else
        {
            fileSaved = false;
        }
        return saved;
    }
}

/**
 * @brief Clears image from the workspaceArea;
 * 
 * @details Note the difference between this function to clearImage();
 * This function will prompt the user to save changes made to image, if there has been unsaved changes.
 */
void MainWindow::on_actionNew_triggered()
{
    if (isConnected && !isHost) {
        QMessageBox::information(this, QString("Not host"), QString("You are not host. Please disconnect to create new image."));
        return;
    }
    if (maybeSave())
    {
        clearImage();
    }
    workspaceArea->setModified(false);

    sendClearScreen();
}


/**
 * @brief Opens a new image.
 * 
 * @details Checks whether the current image has been modified, and prompt "do you want to save".
 * Otherwise open an open file dialog to open the image.
 */
void MainWindow::on_actionOpen_triggered()
{
    if (isConnected && !isHost) {
        QMessageBox::information(this, QString("Not host"), QString("You are not host. Please disconnect to open your own image."));
        return;
    }
    if (maybeSave())
    { // Check if changes have been made since last save
        // Get the file to open from a dialog
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        QImage loadedImage;
        if (!fileName.isEmpty())
        {
            if (!loadedImage.load(fileName))
            {
                return;
            }
            this->fileName = fileName;
            fileSaved = false;

            // Reads image dimensions
            QImageReader reader(fileName);
            QSize sizeOfImage = reader.size();
            int imageHeight = sizeOfImage.height();
            int imageWidth = sizeOfImage.width();

            // Recontruct our graphicsView and workspaceArea, according to the new image dimensions
            resetGraphicsViewScale();
            reconstructWorkspaceArea(imageWidth, imageHeight);

            // Updating data members that keeps image width and height
            resizedImageWidth = imageWidth;
            resizedImageHeight = imageHeight;
            basics->setImageDimensions(imageWidth, imageHeight);

            // Setup our workspace
            workspaceArea->openImage(loadedImage, imageWidth, imageHeight);
            resizeGraphicsViewBoundaries(imageWidth, imageHeight);
            fitImageToScreen(imageWidth, imageHeight);

            // Setup image preview, which is situated in color controls
            colors->setImagePreview(workspaceArea->commitImageForPreview());
            colors->resetSliders();

            // Commit changes to version control.
            commitChanges(loadedImage, "Original Image");

            sendInitialImage();
        }
    }
}

/**
 * @brief Handle file saves.
 * 
 * @details This slot will try to save the file, instead of saveAs the file, if it was once saved. 
 */
void MainWindow::on_actionSave_triggered()
{
    if (fileSaved)
    {
        bool saved = workspaceArea->saveImage(fileName, fileFormat.constData());    // Call for the file to be saved
        if (saved)
        {
            fileSaved = true;
        }
        else
        {
            fileSaved = false;
        }
    }
    else
    {
        saveAs();
    }
}

/**
 * @brief Handle undo actions.
 * 
 * @details Checkout a commit based on current checkout position (i.e. sideNodenumber and masterNodeNumber)
 */
void MainWindow::on_actionUndo_triggered()
{
    undo();
    sendVersion("undo");
}

/**
 * @brief Undo implementation
 *
 * @details Checks the masterNodeNumber and sideNodeNumber to perform suitable checkoutCommit
 */
void MainWindow::undo()
{
    // sideNodeNumber > 0 means we are in a sideBranch, sideBranchLength > 1 means we are also in a sideBranch
    if (sideNodeNumber > 0 || imageHistory.getMasterNodeIteratorAtIndex(masterNodeNumber)->getBranchLength() > 1)
    {
        // Node starts at 0, +2 because we want to compare length and if the image is undoable
        if (sideNodeNumber + 2 <= imageHistory.getMasterNodeIteratorAtIndex(masterNodeNumber)->getBranchLength())
            checkoutCommit(masterNodeNumber, ++sideNodeNumber);
    }
    else if (masterNodeNumber + 2 <= imageHistory.getBranchLength())
    {
        checkoutCommit(++masterNodeNumber, 0);
    }
}

/**
 * @brief Handle redo actions.
 * 
 * @details Checkout a commit based on current checkout position (i.e. sideNodenumber and masterNodeNumber)
 */
void MainWindow::on_actionRedo_triggered()
{
    redo();
    sendVersion("redo");
}

/**
 * @brief Redo implementation
 *
 * @details Checks the masterNodeNumber and sideNodeNumber to perform suitable checkoutCommit
 */
void MainWindow::redo()
{
    // Check if redo is available on current branch
    if (sideNodeNumber > 0)
    {
        checkoutCommit(masterNodeNumber, --sideNodeNumber);
    }
    else if (masterNodeNumber > 0)
    {
        checkoutCommit(--masterNodeNumber, 0);
    }
}

/**
 * @brief Revert commit to last commit.
 */
void MainWindow::on_actionRevert_to_Last_Commit_triggered()
{
    revertToLastCommit();
    sendVersion("revertCommit");
}

/**
 * @brief Revert commit to last commit implementation
 *
 * @details Checks the masterNodeNumber and sideNodeNumber
 * and check for imageHistory
 * whether it is possible to perform suitable checkoutCommit and reverseCommit
 */
void MainWindow::revertToLastCommit()
{
    if (!imageHistory.getBranchLength())
        return;
    // sideNodeNumber > 0 means we are in a sideBranch, sideBranchLength > 1 means we are also in a sideBranch
    if (sideNodeNumber > 0 || imageHistory.getMasterNodeIteratorAtIndex(masterNodeNumber)->getBranchLength() > 1)
    {
        // Node starts at 0, +2 because we want to compare length and if the image is revertible
        if (sideNodeNumber + 2 <= imageHistory.getMasterNodeIteratorAtIndex(masterNodeNumber)->getBranchLength())
        {
            checkoutCommit(masterNodeNumber, sideNodeNumber + 1);
            imageHistory.getMasterNodeIteratorAtIndex(masterNodeNumber)->reverseCommit();
            --sideNodeNumber;
        }
    }
    else if (masterNodeNumber + 2 <= imageHistory.getBranchLength())
    {
        checkoutCommit(masterNodeNumber + 1, 0);
        imageHistory.reverseCommit();
        --masterNodeNumber;
    }
    else
    {
        return;
    }
    generateHistoryMenu();
}

/**
 * @brief Prints the workspace area.
 * 
 * @details calls workspaceArea's print function.
 */
void MainWindow::on_actionPrint_triggered()
{
    on_actionSave_triggered();
    workspaceArea->print();
}

/**
 * @brief Shows About us dialog when slot is triggered.
 */
void MainWindow::on_actionAbout_Us_triggered()
{
    AboutUs aboutUs;
    aboutUs.setModal(true);
    aboutUs.exec();
}

/**
 * @brief Updates image on image drawn/scribbled.
 * 
 * @details commits changes to version control.
 */
void MainWindow::onImageDrawn()
{
    QImage image = workspaceArea->commitImage();
    commitChanges(image, "Brush");
}

/**
 * @brief Filter mouse scroll event to not propagate to the whole mainwindow, instead only to the workspaceArea.
 * 
 * @details Mouse scroll/wheel event will be handled by MainWindow::handleWheelEvent(QGraphicsSceneWheelEvent*).
 * 
 * @param event 
 * @return true event is a GraphicsSceneWheel.
 * @return false event is not a GraphicsSceneWheel.
 */
bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneWheel)
    {
        handleWheelEvent(static_cast<QGraphicsSceneWheelEvent *>(event));

        // Don't propagate event to the whole mainwindow
        event->accept();
        return true;
    }
    return false;
}

/**
 * @brief Zooms in/out the workspaceArea/graphicsView on mouse scroll/wheel event
 * 
 * @param event Scene wheel event.
 */
void MainWindow::handleWheelEvent(QGraphicsSceneWheelEvent *event)
{
    // Sets how the view should position during scene transformations
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Sets up the zooming properties,
    // Saves the current screen size in a QRect for the upper bound of zooming
    const double scaleFactor = 1.1;
    const QRect screenSize = WindowHelper::screenFromWidget(qApp->desktop())->geometry();
    const int upperBound = qMin(screenSize.width(), screenSize.height());
    const int lowerBound = 100;

    /*
     * Register a scroll if the scroll is vertical, within 7 (or -7) degree
     * with maximum workspaceArea dimensions equal to the upperbound
     * and minimum workspaceArea dimensions to the lowerbound
     */
    if (event->orientation() == Qt::Vertical)
    {
        if (event->delta() > 7 && resizedImageWidth < upperBound && resizedImageHeight < upperBound)
        {
            // Update resizedImage dimensions
            graphicsView->scale(scaleFactor, scaleFactor);
            resizeGraphicsViewBoundaries(static_cast<int>(resizedImageWidth * scaleFactor), static_cast<int>(resizedImageHeight * scaleFactor));
            resizedImageWidth = resizedImageWidth * scaleFactor;
            resizedImageHeight = resizedImageHeight * scaleFactor;
            currentZoom *= scaleFactor;
        }
        else if (event->delta() < -7 && resizedImageWidth > lowerBound && resizedImageHeight > lowerBound)
        {
            graphicsView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
            resizeGraphicsViewBoundaries(static_cast<int>(resizedImageWidth / scaleFactor), static_cast<int>(resizedImageHeight / scaleFactor));
            currentZoom /= scaleFactor;
            resizedImageWidth = resizedImageWidth * (1.0 / scaleFactor);
            resizedImageHeight = resizedImageHeight * (1.0 / scaleFactor);
        }
    }

    // Scrolls the contents of the view to ensure the item(workspaceArea) will always be fully visible in the view
    graphicsView->centerOn(0, 0);
    graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

/**
 * @brief Changes workspaceArea zoom level based on level.
 * 
 * @param level The level of zoom desired.
 */
void MainWindow::onZoom(const QString &level)
{
    if (level == "Fit to screen")
    {
        fitImageToScreen(static_cast<int>(resizedImageWidth), static_cast<int>(resizedImageHeight));
        return;
    }

    // Neutralizes any zoom changes made before.
    double originalFactor = 1.0 / currentZoom;
    double a = resizedImageWidth * originalFactor;
    double b = resizedImageHeight * originalFactor;
    graphicsView->scale(a / resizedImageWidth, b / resizedImageHeight);
    resizeGraphicsViewBoundaries(static_cast<int>(resizedImageWidth * originalFactor), static_cast<int>(resizedImageHeight * originalFactor));
    resizedImageWidth *= originalFactor;
    resizedImageHeight *= originalFactor;
    graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    double scaleFactor = 1.0;

    if (level == "50%")
    {
        scaleFactor = 0.5;
        currentZoom = 0.5;
    }
    else if (level == "100%")
    {
        scaleFactor = 1.0;
        currentZoom = 1.0;
    }
    else if (level == "120%")
    {
        scaleFactor = 1.2;
        currentZoom = 1.2;
    }

    // Setup scaling settings to graphicsView settings.
    a = resizedImageWidth * scaleFactor;
    b = resizedImageHeight * scaleFactor;
    graphicsView->scale(a / resizedImageWidth, b / resizedImageHeight);
    resizeGraphicsViewBoundaries(static_cast<int>(resizedImageWidth * scaleFactor), static_cast<int>(resizedImageHeight * scaleFactor));
    resizedImageWidth *= scaleFactor;
    resizedImageHeight *= scaleFactor;

    graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

/**
 * @brief Updates workspaceArea's cursor.
 * 
 * @details This slot receives basic controls' cursor change signals.
 * Drawing/cropping/cutting an image will be signaled by the change of cursor state.
 * 
 * @param cursor a WorkspaceArea::CursorMode cursor.
 * @param data Any integer data that is needed for certain cursor, e.g. Magic Wand threshold.
 */
void MainWindow::onCrossCursorChanged(WorkspaceArea::CursorMode cursor, int data)
{
    graphicsView->setCursor(Qt::CrossCursor);
    switch (cursor)
    {
    case WorkspaceArea::CursorMode::RECTANGLECROP:
        workspaceArea->setCursorMode(WorkspaceArea::CursorMode::RECTANGLECROP);
        break;
    case WorkspaceArea::CursorMode::MAGICWAND:
        workspaceArea->setCursorMode(WorkspaceArea::CursorMode::MAGICWAND);
        workspaceArea->setMagicWandThreshold(data);
        break;
    case WorkspaceArea::CursorMode::SCRIBBLE:
        graphicsView->setCursor(Qt::ArrowCursor);
        workspaceArea->setCursorMode(WorkspaceArea::CursorMode::SCRIBBLE);
        break;
    }
}

/**
 * @brief Fits the image to screen.
 * 
 * @details If any of the currentImage dimensions is larger than the screen dimensions, we would like to scale down.
 * Else if any of the currentImage dimensions is smaller than the screen dimensions, we would like to scale up.
 * Otherwise, both dimensions are equal, we can leave the function.
 * 
 * @param currentImageWidth 
 * @param currentImageHeight 
 */
void MainWindow::fitImageToScreen(int currentImageWidth, int currentImageHeight)
{
    const QRect screenRect = WindowHelper::screenFromWidget(qApp->desktop())->geometry();
    int screenWidth = screenRect.width();
    int screenHeight = screenRect.height();

    double ratio;

    if (currentImageWidth > screenWidth || currentImageHeight > screenHeight)
    {
        ratio = qMax(static_cast<double>(screenWidth) / currentImageWidth, static_cast<double>(screenHeight) / currentImageHeight) * 0.5;
    }
    else if (currentImageWidth < screenWidth || currentImageHeight < screenHeight)
    {
        ratio = qMin(static_cast<double>(screenWidth) / currentImageWidth, static_cast<double>(screenHeight) / currentImageHeight) * 0.5;
    }
    else
    {
        return;
    }
    graphicsView->scale(ratio, ratio);
    resizeGraphicsViewBoundaries(static_cast<int>(resizedImageWidth * ratio), static_cast<int>(resizedImageHeight * ratio));
    resizedImageWidth *= ratio;
    resizedImageHeight *= ratio;
    currentZoom *= ratio;
    graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    comboBox->setCurrentText("Fit to screen");
}

/**
 * @brief Rerenders the workspace area.
 * 
 * @details Mechanism to view the cropped/filtered/transformed image:
 * 1. store current workspaceArea to a temporaryArea
 * 2. create a new workspaceArea to hold the new cropped image, and set the graphicsView to contain this new workspaceArea
 * 3. the temporaryArea will then be destroyed when a) destroyed by the destructor, or b) there is a new signal to crop the image.
 * We cannot destroy/reconstruct the workspaceArea in this slot, as this slot is connected (signal-slot connection) to the caller signal in the workspaceArea.
 * @param image Image to be rendered.
 * @param imageWidth Width of the image.
 * @param imageHeight Height of the image.
 */
void MainWindow::rerenderWorkspaceArea(const QImage &image, int imageWidth, int imageHeight)
{
    resetGraphicsViewScale();
    // Removes previous temporaryArea. If temporaryArea is not nullptr,
    // this means temporaryArea holds the previous workspaceArea.
    delete temporaryArea;

    // Let the temporaryArea hold our current workspaceArea
    temporaryArea = workspaceArea;

    // Create a new workspaceArea to hold the newly cropped image, while setting up all the connections needed.
    workspaceArea = new WorkspaceArea(imageWidth, imageHeight);
    reconnectConnection();

    // Contain the new workspaceArea into our graphicsView
    graphicsView->setScene(workspaceArea);
    ui->workspaceView->addWidget(graphicsView);
    workspaceArea->setParent(graphicsView);
    graphicsView->scene()->installEventFilter(this);

    // Update our data members
    resizedImageWidth = imageWidth;
    resizedImageHeight = imageHeight;

    // Actually open the cropped image
    workspaceArea->openImage(image, imageWidth, imageHeight);
    resizeGraphicsViewBoundaries(imageWidth, imageHeight);
    fitImageToScreen(imageWidth, imageHeight);

    // Reset crop buttons state (make buttons pressable after crop is finished)
    basics->on_cancelCutoutPushButton_clicked();
    basics->setImageDimensions(imageWidth, imageHeight);
    colors->setImagePreview(workspaceArea->commitImageForPreview());
    colors->resetSliders();

    onUpdateImagePreview();
}

/**
 * @brief Resets graphics view scale.
 * 
 * @details Updates resizedImageWidth and resizedImageHeight, the graphicsView scale matrix, and resizes the graphics view boundaries. 
 */
void MainWindow::resetGraphicsViewScale()
{
    // Resize image dimensions properties by reverting horizontal/vertical scaling factor changes
    resizedImageWidth /= graphicsView->matrix().m11();
    resizedImageHeight /= graphicsView->matrix().m22();

    // Reset graphics view scaling factor
    graphicsView->resetMatrix();

    resizeGraphicsViewBoundaries(static_cast<int>(resizedImageWidth), static_cast<int>(resizedImageHeight));
    graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

/**
 * @brief Applies filter/transformation to current workspaceArea.
 * 
 * @param filterTransform Filter/transform to be applied.
 * @param size Filter kernel size (if any).
 * @param strength Filter strength (if any).
 * @param fromServer If the filter/transformation is sent from the server.
 */
void MainWindow::applyFilterTransform(AbstractImageFilterTransform *filterTransform, int size, double strength, bool fromServer)
{
    // Commit all brush strokes before applying transform
    workspaceArea->commitImageAndSet();

    // Get filtered image and rerender the workspaceArea with the new image.
    QImage &&result = filterTransform->applyFilter(workspaceArea->getImage(), size, strength);

    if (!fromServer) {
        if (filterTransform->getName() == "Image Scissors") {
            ImageScissors* temp = reinterpret_cast<ImageScissors*>(filterTransform);
            sendFilterWithMask(filterTransform->getName(), size, strength, temp->getMask());
        } else if (filterTransform->getName() == "Image Inpainting") {
            ImageInpainting* temp = reinterpret_cast<ImageInpainting*>(filterTransform);
            sendFilterWithMask(filterTransform->getName(), size, strength, temp->getMask());
        } else {
            sendFilter(filterTransform->getName(), size, strength);
        }
    }
    rerenderWorkspaceArea(result, result.width(), result.height());

    // Add after-filter-applied image to our image history version control, generate our history menu
    commitChanges(workspaceArea->getImage(), filterTransform->getName());

    delete filterTransform;
}

/**
 * @brief Updates our image previewer in color controls with this filter/transform.
 * 
 * @param filterTransform Filter/Transform to be applied.
 * @param size Filter kernel size (if any).
 * @param strength Filter strength (if any).
 */
void MainWindow::applyFilterTransformOnPreview(AbstractImageFilterTransform *filterTransform, int size, double strength)
{
    QImage &&previewImageNoFilter = workspaceArea->commitImageForPreview();
    QImage &&result = filterTransform->applyFilter(previewImageNoFilter, size, strength);
    colors->setImagePreview(result);
    delete filterTransform;
}

/**
 * @brief Updates the image in our color controls' image previewer.
 */
void MainWindow::onUpdateImagePreview()
{
    QImage &&previewImage = workspaceArea->commitImageForPreview();
    colors->setImagePreview(previewImage);
}

/**
 * @brief Handles commit changes action/merging to master branch.
 * 
 * @details This action is triggered manually by the user. This means user will try to commit changes/merge to master branch.
 */
void MainWindow::on_actionCommit_Changes_triggered()
{
    CommitDialog commitDialog;
    commitDialog.setModal(true);
    if (commitDialog.exec())
    {
        commitBranch();
        sendVersion("commitBranch");
    }
}

/**
 * @brief Commit branch implementation
 *
 * @details Save image in workspaceArea,
 * Remove node,
 * Reset masterNodeNumber and sideNodeNumber,
 * Commit image and generate history
 */
void MainWindow::commitBranch()
{
    // Save the image
    QImage toMerge = workspaceArea->getImage();
    // Remove the node
    imageHistory.masterBranch.erase(imageHistory.getMasterNodeIteratorAtIndex(masterNodeNumber));
    // Set master and side to 0, 0
    masterNodeNumber = 0;
    sideNodeNumber = 0;
    // Commit image with saved image
    commitChanges(toMerge, "Merged");
    // Generate history
    generateHistoryMenu();
}

/**
 * @brief Commit changes to version control.
 * 
 * @param changes Description/commit message.
 */
void MainWindow::onCommitChanges(QString changes)
{
    if (masterNodeNumber == 0)      // commit at the master branch
    {
        imageHistory.commitChanges(workspaceArea->getImage(), changes);
    }
    else        // commit at a side branch
    {
        imageHistory.getMasterNodeIteratorAtIndex(masterNodeNumber)->commitChanges(workspaceArea->getImage(), changes);
    }
    generateHistoryMenu();
}

/**
 * @brief Handles creating room (for multiplayer feature)
 * @param name Name of the user
 *
 * @details create new server and connects the user through his/her ip and port
 * join the new room
 */
void MainWindow::onCreateRoom(QString name)
{
    username = name;
    if (name.isEmpty())
    {
        QMessageBox::information(this, QString("Empty Name"), QString("Name cannot be empty"));
        return;
    }

    server = new Server(this);
    connect(server, &Server::newPlayerConnected, this, &MainWindow::sendInitialImage);
    ip = server->getIP();
    port = server->getPort();
    isHost = true;
    joinRoom();
}

/**
 * @brief Handles joining room
 * @param name Name of the user
 * @param address IP address of the user
 * @param port user's port number
 *
 * @details Checks if user inputs valid server or uses valid port
 * If true, then registers the user to the desired room
 * The user won't be the host of the room
 */
void MainWindow::onJoinRoom(QString name, QString address, quint16 port)
{
    ip = address;
    if (ip.isEmpty())
    {
        QMessageBox::information(this, QString("Empty Server Name"), QString("Server name cannot be empty"));
        return;
    }

    this->port = port;
    if (!port)
    {
        QMessageBox::information(this, QString("Invalid Port"), QString("Invalid Port"));
        return;
    }

    username = name;
    if (username.isEmpty())
    {
        QMessageBox::information(this, QString("Empty Name"), QString("Name cannot be empty"));
        return;
    }

    isHost = false;
    joinRoom();
}

/**
 * @brief Joining room implementation
 *
 * @details user becomes client,
 * Connects client to the server (with the ip and port)
 */
void MainWindow::joinRoom()
{
    if (!isHost) {
        clearImage();
    }
    client = new Client();
    connect(client, &Client::connectionFailedBad, this, &MainWindow::onConnectionFailed);
    connect(client, &Client::connected, [=]() {goToServerRoom(true);});
    connect(client, &Client::receiveJson, this, &MainWindow::clientJsonReceived);
    connect(client, &Client::connected, this, &MainWindow::sendPlayerName);
    connect(client, &Client::connectionStopped, this, &MainWindow::onConnectionStopped);
    connect(client, &Client::disconnected, [=]() {destroyConnection();});
    client->connectToServer(QHostAddress(ip), port);
}

/**
 * @brief Informs us if connection failed
 *
 * @details sets room's modality into false, closes the room
 */
void MainWindow::onConnectionFailed()
{
    QMessageBox::information(this, QString("Connection failed"), QString("Invalid IP address or port."));
//    if (room)
//    {
//        room->setModal(false);
//        room->close();
//    }
}

/**
 * @brief Go to server room
 *
 * @details Set room with ip and port, user is connected
 */
void MainWindow::goToServerRoom(bool onEnter)
{
    room->setServerRoom(ip, port);
    room->setModal(true);
    if (!onEnter) {
        room->exec();
    } else {
        isConnected = true;
    }
}

/**
 * @brief Handles create room
 *
 * @details Unless the user is connected to any other rooms,
 * room will be created
 */
void MainWindow::on_actionCreate_Room_triggered()
{
    if (isConnected)
    {
        QMessageBox::information(this, QString("Unable to create"), QString("You have an active connection."));
        return;
    }
    room = new ServerRoom();
    room->setCreateRoom();
    connect(room, &ServerRoom::createRoom, this, &MainWindow::onCreateRoom);
    connect(room, &ServerRoom::disconnect, this, &MainWindow::onDisconnect);
    room->setModal(true);
    room->exec();
}

/**
 * @brief Handles joining room
 *
 * @details Unless the user is connected to any other rooms,
 * the user can join the room
 */
void MainWindow::on_actionJoin_Room_triggered()
{
    if (isConnected)
    {
        QMessageBox::information(this, QString("Unable to create"), QString("You have an active connection."));
        return;
    }
    room = new ServerRoom();
    room->setJoinRoom();
    connect(room, &ServerRoom::joinRoom, this, &MainWindow::onJoinRoom);
    connect(room, &ServerRoom::disconnect, this, &MainWindow::onDisconnect);
    room->setModal(true);
    room->exec();
}

/**
 * @brief
 * @param json QJsonObject
 *
 * @details Checks the string-typed value of json and performs desired actions
 */
void MainWindow::clientJsonReceived(const QJsonObject &json)
{
    qDebug() << json;
    const QString type = json.value(QString("type")).toString();
    if (type == "newPlayer")
    {
        qDebug() << "New Player has entered";
    }
    else if (type == "playerList")
    {
        room->emptyPlayers();
        for (QJsonValue playerName : json.value(QString("playerNames")).toArray())
            room->addPlayer(playerName.toString());
    }
    else if (type == "playerFull")
    {
        qDebug() << "CANNOT ENTER BRO";
        QMessageBox::information(this, QString("Room Full"), QString("Room is full. Please try again or find a different room."));
        isConnected = false;
        client->disconnectFromHost();
        client->deleteLater();

        room->setJoinRoom();
    }
    else if (type == "initialImage")
    {
        QByteArray ba = QByteArray::fromBase64(json.value(QString("data")).toString().toLatin1());
        QImage img = QImage::fromData(ba, "PNG");
        if (img.isNull())
        {
            qDebug() << "image error";
        }
        else
        {
            qDebug() << "image success";
            int imageWidth = img.width();
            int imageHeight = img.height();
            resetGraphicsViewScale();
            reconstructWorkspaceArea(imageWidth, imageHeight);

            // Updating data members that keeps image width and height
            resizedImageWidth = imageWidth;
            resizedImageHeight = imageHeight;
            basics->setImageDimensions(imageWidth, imageHeight);

            // Setup our workspace
            workspaceArea->openImage(img, imageWidth, imageHeight);
            resizeGraphicsViewBoundaries(imageWidth, imageHeight);
            fitImageToScreen(imageWidth, imageHeight);

            // Setup image preview, which is situated in color controls
            colors->setImagePreview(workspaceArea->commitImageForPreview());
            colors->resetSliders();

            // Commit changes to version control.
            commitChanges(img, "Original Image");

        }
    }
    else if (type == "nameRepeat")
    {
        QMessageBox::information(this, QString("Unable to use name"), QString("Please choose a different name."));
        isConnected = false;
        client->disconnectFromHost();
        client->deleteLater();
        room->setJoinRoom();
    }
    else if (type == "hostDisconnected")
    {
        destroyConnection();
    }
    else if (type == "applyFilter")
    {
        QJsonValue data = json.value(QString("data"));
        QString name = data["name"].toString();
        int size = data["size"].toInt();
        double strength = data["strength"].toDouble();
        if (!name.isEmpty()) {
            handleFilterBroadcast(name, size, strength);
        }
    }
    else if (type == "applyFilterWithMask")
    {
        QJsonValue data = json.value(QString("data"));
        QString name = data["name"].toString();
        int size = data["size"].toInt();
        double strength = data["strength"].toDouble();
        QByteArray ba = QByteArray::fromBase64(data["mask"].toString().toLatin1());
        QImage mask = QImage::fromData(ba, "PNG");
        if (!mask.isNull() && !name.isEmpty()) {
            handleFilterBroadcast(name, size, strength, mask);
        }
    }
    else if (type == "applyResize")
    {
        QJsonValue data = json.value(QString("data"));
        int width = data["width"].toInt();
        int height = data["height"].toInt();
        workspaceArea->resizeImage(width, height, true);
    }
    else if (type == "applyCrop")
    {
        QJsonValue data = json.value(QString("data"));
        int x = data["x"].toInt();
        int y = data["y"].toInt();
        int width = data["width"].toInt();
        int height = data["height"].toInt();
        workspaceArea->cropImage(x, y, width, height, true);
    }
    else if (type == "applyCropWithMagicWand")
    {
        QJsonValue data = json.value(QString("data"));
        int x = data["x"].toInt();
        int y = data["y"].toInt();
        workspaceArea->cropImageWithMagicWand(x, y, true);
    } else if (type == "versionControl") {
        QString action = json.value(QString("action")).toString();
        if (action == "checkoutCommit") {
            handleVersionControlBroadcast(action, json.value(QString("masterNodeNumber")).toInt(), json.value(QString("sideNodeNumber")).toInt());
            return;
        }
        if (!action.isEmpty()) {
            handleVersionControlBroadcast(action);
        }
    }
    else if (type == "applyMoveScribble")
    {
        QJsonValue data = json.value(QString("data"));
        QPointF pos(data["x"].toDouble(), data["y"].toDouble());
        QColor penColor(data["colorHex"].toString());
        int penWidth = data["penWidth"].toInt();
        workspaceArea->onMoveScribble(pos, penColor, penWidth);
    }
    else if (type == "applyReleaseScribble")
    {
        workspaceArea->onReleaseScribble();
    }
    else if (type == "applyClear") {
        clearImage();
        workspaceArea->setModified(false);
    }
}

/**
 * @brief Sending player name
 *
 * @details set the user as player with username as the name
 * send the player name
 */
void MainWindow::sendPlayerName()
{
    QJsonObject playerNameMsg;
    playerNameMsg["type"] = "playerName";
    playerNameMsg["playerName"] = username;

    client->sendJson(playerNameMsg);
}

/**
 * @brief Sending initial image
 *
 * @details If user is hosting the room and image is present,
 * send the initial image
 */
void MainWindow::sendInitialImage()
{
    if (isHost)
    {
        workspaceArea->commitImageAndSet();
        QImage image = workspaceArea->getImage();
        if (!image.isNull())
        {
            QBuffer buffer;
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "PNG");
            QJsonObject json;
            json["type"] = "initialImage";
            json["data"] = QJsonValue(QLatin1String(buffer.data().toBase64()));
            client->sendJson(json);
        }
    }
}

/**
 * @brief Handling viewing room
 *
 * @details If user is not connected, message box will pop up (Room unavailable, create or join room first.)
 * Else, go to server room
 */
void MainWindow::on_actionView_Room_triggered()
{
    if (!isConnected)
    {
        QMessageBox::information(this, QString("Room Unavailable"), QString("Please create or join room first."));
        return;
    }
    goToServerRoom();
}

/**
 * @brief Tells if connection is stopped
 *
 * @details message box will pop up
 */
void MainWindow::onConnectionStopped()
{
    QMessageBox::information(this, QString("Connection Stopped"), QString("There was an error in the connection"));
}

/**
 * @brief Handling desired disconnecting
 *
 * @details message box will pop to ask if the user would like to leave room
 * If yes, connection destroyed
 */
void MainWindow::onDisconnect()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, QString("Leave Room"), QString("Are you sure to leave the room?"));
    if (reply == QMessageBox::Yes)
    {
        if (isHost) {
            if (server != nullptr)
            {
                server->stopServer();
                server->deleteLater();
                server = nullptr;
                return;
            }
        }
        destroyConnection();
    }
}

/**
 * @brief Destroying connection implementation
 *
 * @details Closes room after checking for null client or server
 */
void MainWindow::destroyConnection()
{
    isHost = false;
    isConnected = false;
    if (client != nullptr)
    {
        client->disconnectFromHost();
        client->deleteLater();
        client = nullptr;
    }
    room->close();
    delete room;
}


/**
 * @brief Sending filter implementation
 * @param filterName name of filter that is used
 * @param size size of kernel (if kernel is involved)
 * @param strength strength of filter effect used
 *
 * @details Sending information of which filter used to other users
 */
void MainWindow::sendFilter(const QString& filterName, int size, double strength) {
    if (!isConnected || client == nullptr) {
        return;
    }
    QJsonObject json;
    QJsonObject data;
    data["name"] = filterName;
    data["size"] = size;
    data["strength"] = strength;
    json["type"] = "applyFilter";
    json["data"] = data;
    client->sendJson(json);
}

/**
 * @brief Sending filter with mask implementation
 * @param filterName name of filter that is applied
 * @param size size of kernel (if kernel is involved)
 * @param strength strength of filter effect applied
 * @param mask image mask that is used
 *
 * @details Sending information of which filter used to other users
 * The filter here is special because includes image mask
 */
void MainWindow::sendFilterWithMask(const QString& filterName, int size, double strength, const QImage& mask) {
    if (!isConnected || client == nullptr) {
        return;
    }

    QJsonValue maskBytes;
    if (!mask.isNull())
    {
        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);
        mask.save(&buffer, "PNG");
        maskBytes = QJsonValue(QLatin1String(buffer.data().toBase64()));
    }
    QJsonObject json;
    QJsonObject data;
    data["name"] = filterName;
    data["size"] = size;
    data["strength"] = strength;
    data["mask"] = maskBytes;
    json["type"] = "applyFilterWithMask";
    json["data"] = data;
    client->sendJson(json);
}

/**
 * @brief Sending for version control
 * @param type type to set json's action
 *
 * @details If user is connected and type is valid, set json's action
 */
void MainWindow::sendVersion(const QString& type) {
    if (isConnected) {
        QJsonObject json;
        json["type"] = "versionControl";
        if (!type.isEmpty()) {
            json["action"] = type;
        }
        client->sendJson(json);
    }
}

/**
 * @brief Sending for version control
 * @param type type to set json's action
 * @param masterNodeNumber for branches and nodes
 * @param sideNodeNumber for branch and nodes
 *
 * @details If user is connected and type is valid, set json's action
 * Also set the masterNodeNumber and sideNodeNumber of json
 */
void MainWindow::sendVersion(const QString& type, int masterNodeNumber, int sideNodeNumber) {
    if (isConnected) {
        QJsonObject json;
        json["type"] = "versionControl";
        if (!type.isEmpty()) {
            json["action"] = type;
            json["masterNodeNumber"] = masterNodeNumber;
            json["sideNodeNumber"] = sideNodeNumber;
        }
        client->sendJson(json);
    }
}

void MainWindow::sendClearScreen() {
    if (isConnected) {
        QJsonObject json;
        json["type"] = "applyClear";
        client->sendJson(json);
    }
}

/**
 * @brief Sending for resizing
 * @param width Width of screen
 * @param height Height of screen
 *
 * @details If user is connected, set the width and height data,
 * Also applyResize and data of json
 */
void MainWindow::onSendResize(int width, int height) {
    if (isConnected) {
        QJsonObject json;
        QJsonObject data;
        data["width"] = width;
        data["height"] = height;
        json["type"] = "applyResize";
        json["data"] = data;
        client->sendJson(json);
    }
}

/**
 * @brief Sending for cropping
 * @param x Position from top edge of screen
 * @param y Position from left edge of screen
 * @param width Width of screen
 * @param height Height of screen
 *
 * @details If user is connected, set the width and height data
 * Also applyCrop and data of json
 */
void MainWindow::onSendCrop(int x, int y, int width, int height) {
    if (isConnected) {
        QJsonObject json;
        QJsonObject data;
        data["x"] = x;
        data["y"] = y;
        data["width"] = width;
        data["height"] = height;
        json["type"] = "applyCrop";
        json["data"] = data;
        client->sendJson(json);
    }
}

/**
 * @brief Sending for cropping, special for Magic Wand
 * @param x Position from top edge of screen
 * @param y Position from left edge of screen
 *
 * @details If user is connected, set the x and y data
 * Also applyCropWithMagicWand and data of json
 */
void MainWindow::onSendCropWithMagicWand(int x, int y) {
    if (isConnected) {
        QJsonObject json;
        QJsonObject data;
        data["x"] = x;
        data["y"] = y;
        json["type"] = "applyCropWithMagicWand";
        json["data"] = data;
        client->sendJson(json);
    }
}

/**
 * @brief Sending special during scribble moving
 * @param x Position from top edge of screen
 * @param y Position from left edge of screen
 * @param colorHex color of pen used
 * @param penWidth width of pen tip used
 *
 * @details If user is connected, set the x and y data
 * Also applyMoveScribble and data of used pen (color and width)
 */
void MainWindow::onSendMoveScribble(double x, double y, QString colorHex, int penWidth) {
    if (isConnected) {
        QJsonObject json;
        QJsonObject data;
        data["x"] = x;
        data["y"] = y;
        data["colorHex"] = colorHex;
        data["penWidth"] = penWidth;
        json["type"] = "applyMoveScribble";
        json["data"] = data;
        client->sendJson(json);
    }
}

/**
 * @brief Sending special during scribble release
 *
 * @details If user is connected, set json's type as applyReleaseScribble
 */
void MainWindow::onSendReleaseScribble() {
    if (isConnected) {
        QJsonObject json;
        json["type"] = "applyReleaseScribble";
        client->sendJson(json);
    }
}

/**
 * @brief Handling filter broadcast
 * @param name name of filter
 * @param size size of kernel (if kernel is involved)
 * @param strength strength of filter applied
 *
 * @details applyFilterTransform according to the name of filter applied
 * and according to size and strength
 */
void MainWindow::handleFilterBroadcast(const QString& name, int size, double strength) {
    if (name == "Hue Filter") {
        HueFilter *hueFilter = new HueFilter();
        applyFilterTransform(hueFilter, size, strength, true);
    } else if (name == "Saturation Filter") {
        SaturationFilter *saturationFilter = new SaturationFilter();
        applyFilterTransform(saturationFilter, size, strength, true);
    } else if (name == "Tint Filter") {
        TintFilter *tintFilter = new TintFilter();
        applyFilterTransform(tintFilter, size, strength, true);
    } else if (name == "Temperature Filter") {
        TemperatureFilter *temperatureFilter = new TemperatureFilter();
        applyFilterTransform(temperatureFilter, size, strength, true);
    } else if (name == "Brightness Filter") {
        BrightnessFilter *brightnessFilter = new BrightnessFilter();
        applyFilterTransform(brightnessFilter, size, strength, true);
    } else if (name == "Contrast Filter") {
        ContrastFilter *contrastFilter = new ContrastFilter();
        applyFilterTransform(contrastFilter, size, strength, true);
    } else if (name == "Exposure Filter") {
        ExposureFilter *exposureFilter = new ExposureFilter();
        applyFilterTransform(exposureFilter, size, strength, true);
    } else if (name == "Invert Filter") {
        InvertFilter *invertFilter = new InvertFilter();
        applyFilterTransform(invertFilter, size, strength, true);
    } else if (name == "Exposure Filter") {
        GrayscaleFilter *grayscaleFilter = new GrayscaleFilter();
        applyFilterTransform(grayscaleFilter, size, strength, true);
    } else if (name == "Edge Detection Filter") {
        EdgeDetectionFilter *edgeDetectionFilter = new EdgeDetectionFilter();
        applyFilterTransform(edgeDetectionFilter, size, strength, true);
    } else if (name == "Emboss Filter") {
        EmbossFilter *embossFilter = new EmbossFilter();
        applyFilterTransform(embossFilter, size, strength, true);
    } else if (name == "Gaussian Blur Filter") {
        GaussianBlurFilter *gaussianBlurFilter = new GaussianBlurFilter();
        applyFilterTransform(gaussianBlurFilter, size, strength, true);
    } else if (name == "Mean Blur Filter") {
        MeanBlurFilter *meanBlurFilter = new MeanBlurFilter();
        applyFilterTransform(meanBlurFilter, size, strength, true);
    } else if (name == "Clockwise Rotation") {
        ClockwiseRotationTransform *cwRotation = new ClockwiseRotationTransform();
        applyFilterTransform(cwRotation, size, strength, true);
    } else if (name == "Counter Clockwise Rotation") {
        CounterClockwiseRotationTransform *ccwRotation = new CounterClockwiseRotationTransform();
        applyFilterTransform(ccwRotation, size, strength, true);
    } else if (name == "Flip Horizontal") {
        FlipHorizontalTransform *flipHorizontal = new FlipHorizontalTransform();
        applyFilterTransform(flipHorizontal, size, strength, true);
    } else if (name == "Flip Vertical") {
        FlipVerticalTransform *flipVertical = new FlipVerticalTransform();
        applyFilterTransform(flipVertical, size, strength, true);
    }
}

/**
 * @brief Handling filter broadcast which needs mask (Image Scissors and Image Inpainting)
 * @param name name of filter
 * @param size size of kernel (if kernel is involved)
 * @param strength strength of filter applied
 * @param mask image mask that is used
 *
 * @details applyFilterTransform according to the name of filter applied
 * and according to size and strength, also mask
 */
void MainWindow::handleFilterBroadcast(const QString& name, int size, double strength, const QImage& mask) {
    if (name == "Image Scissors") {
        ImageScissors *imageScissors = new ImageScissors();
        imageScissors->setMask(mask);
        applyFilterTransform(imageScissors, size, strength, true);
    } else if (name == "Image Inpainting") {
        ImageInpainting *imageInpainting = new ImageInpainting(size);
        imageInpainting->setMask(mask);
        applyFilterTransform(imageInpainting, size, strength, true);
    }
}

/**
 * @brief Handling version control broadcast
 * @param action name of action implemented
 *
 * @details performs action according to action name
 */
void MainWindow::handleVersionControlBroadcast(const QString& action) {
    if (action == "undo") {
        undo();
    } else if (action == "redo") {
        redo();
    } else if (action == "revertCommit") {
        revertToLastCommit();
    } else if (action == "commitBranch") {
        commitBranch();
    }
}

/**
 * @brief Handling version control broadcast that needs masterNodeNumber and sideNodeNumber
 * @param action name of action implemented
 * @param masterNodeNumber for branch and nodes
 * @param sideNodeNumber for branch and nodes
 *
 * @details performs action according to action name and masterNodeNumber & sideNodeNumber
 */
void MainWindow::handleVersionControlBroadcast(const QString& action, int masterNodeNumber, int sideNodeNumber) {
    if (action == "checkoutCommit") {
        checkoutCommit(masterNodeNumber, sideNodeNumber);
    }
}

/**
 * @brief Shows Faq dialog when slot is triggered.
 */
void MainWindow::on_actionFAQs_triggered()
{
    Faq faq;
    faq.setModal(true);
    faq.exec();
}
