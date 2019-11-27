#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtWidgets>
#include <QRubberBand>
#include "FilterTransform/NonKernelBased/MagicWand.h"

#include "WorkspaceArea.h"
#include "Palette/BasicControls.h"
#include "Palette/Brush.h"
#include "Palette/Histogram.h"
#include "Palette/ColorControls.h"
#include "Palette/Effects.h"

#include "serverroom.h"

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
    ui->toolBar->addAction(ui->actionCommit_Image);

    // Setup our actions shortcuts
    ui->actionNew->setShortcuts(QKeySequence::New);
    ui->actionOpen->setShortcuts(QKeySequence::Open);
    ui->actionSave->setShortcuts(QKeySequence::Save);
    ui->actionUndo->setShortcuts(QKeySequence::Undo);
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

MainWindow::~MainWindow()
{
    delete ui;
    if (temporaryArea != nullptr)
    {
        delete temporaryArea;
        temporaryArea = nullptr;
    }
}

// Setup all connections related to the workspaceArea
// This is needed in order to reestablish connection after workspaceArea is recreated
void MainWindow::reconnectConnection()
{
    connect(workspaceArea, &WorkspaceArea::edited, this, &MainWindow::on_edit);
    connect(workspaceArea, &WorkspaceArea::imageLoaded, histo, &Histogram::onImageLoaded);
    connect(brush, &Brush::penColorChanged, workspaceArea, &WorkspaceArea::setPenColor);
    connect(brush, &Brush::penWidthChanged, workspaceArea, &WorkspaceArea::setPenWidth);
    connect(workspaceArea, &WorkspaceArea::imageCropped, this, &MainWindow::rerenderWorkspaceArea);
    connect(basics, &BasicControls::resizeButtonClicked, workspaceArea, &WorkspaceArea::resizeImage);
    connect(workspaceArea, &WorkspaceArea::imageResized, this, &MainWindow::rerenderWorkspaceArea);
    connect(workspaceArea, &WorkspaceArea::updateImagePreview, this, &MainWindow::onUpdateImagePreview);
}

/*
 * Delete the current workspaceArea and recreate it with the new image dimensions
 * Then resetup our graphicsView
 */
void MainWindow::reconstructWorkspaceArea(int imageWidth, int imageHeight)
{
    // Before reconstructing new workspaceArea, we need to clean stroke history
    while (!strokeHistory.isEmpty()) {
        on_actionUndo_triggered();
    }

    delete workspaceArea;
    workspaceArea = nullptr;

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
    graphicsView->setSceneRect(0, 0, newWidth - 2, newHeight - 2); // Allow for extra 2px boundaries, 1px on the left/top and 1px on the right/bottom
}

// Adds a root into a parent widget, in the palette treeWidget
void MainWindow::addRoot(QTreeWidgetItem *parent, QString name)
{
    parent->setText(0, name);
    ui->palette->addTopLevelItem(parent);
}

/*
 * Adds a child to a parent, this is a custom implementation of the addChild function
 * This child is a QWidget
 */
void MainWindow::customAddChild(QTreeWidgetItem *parent, QWidget *widget)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    parent->addChild(item);
    ui->palette->setItemWidget(item, 0, widget);
}

// When user is closing the app, spawn a "do you want to save" widget
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

/*
 * Called when the user clicks Save As in the menu,
 * then this function will call the saveAsFile function to actually save the image with the right format
 */
void MainWindow::saveAs()
{
    QAction *action = qobject_cast<QAction *>(sender()); // Represents the action of the user clicking
    QByteArray defaultFormat("jpg", 3);                  // Sets default
    fileFormat = action->data().toByteArray();           // Stores the array of bytes of the users data
    if (fileFormat.isEmpty())
    {
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
    workspaceArea->setImageLoaded(false);
}

// Define menu actions for SaveAs and clearScreen
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

    // Create clear screen action and tie to ScribbleWindow::clearImage()
    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), this, SLOT(clearImage()));
}

void MainWindow::generateHistoryMenu()
{
    // Clean image history menu
    for (QMenu* i : imageHistoryMenu) {
        delete imageHistoryMenu[0];
        imageHistoryMenu.pop_front();
    }

    // Traverse the imageHistory (Version Control)
    QLinkedList<VersionControl::MasterNode>::iterator it = imageHistory.masterBranch.begin();
    // 1a. Traverse nodes in master branch
    for (int masterNodeNumber = 0; it != imageHistory.masterBranch.end(); ++it, ++masterNodeNumber) {

        // 1b. Create a menu for each master node
        QMenu* masterNodeMenu = new QMenu(it->changes, this);

        // 2a. Traverse images in side branches
        QLinkedList<VersionControl::SideNode>::iterator i = it->sideBranch.begin();
        for (int sideNodeNumber = 0; i != it->sideBranch.end(); ++i, ++sideNodeNumber) {

            // 2b. Create an action for each image
            QAction* action = new QAction(i->changes, masterNodeMenu);
            masterNodeMenu->addAction(action);
            connect(action, &QAction::triggered, this, [=](){ checkoutCommit(masterNodeNumber, sideNodeNumber); });
        }

        // 3. Add the menu to our menuHistory
        ui->menuHistory->addMenu(masterNodeMenu);
        imageHistoryMenu.append(masterNodeMenu);
    }
}

void MainWindow::checkoutCommit(int masterNodeNumber, int sideNodeNumber)
{
    // Traverse the imageHistory (Version Control)
    QLinkedList<VersionControl::MasterNode>::iterator it = imageHistory.masterBranch.begin();
    // 1. Traverse nodes in master branch
    for (int i = 0; i < masterNodeNumber; ++i, ++it) {
    }
    // 2. Traverse images in the side branch
    QLinkedList<VersionControl::SideNode>::iterator it2 = it->sideBranch.begin();
    for (int j = 0; j < sideNodeNumber; ++j, ++it2) {
    }
    rerenderWorkspaceArea(it2->currentImage, it2->currentImage.width(), it2->currentImage.height());
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

    if (fileName.isEmpty())
    {
        return false;
    }
    else
    {
        bool saved = workspaceArea->saveImage(fileName, fileFormat.constData()); // Call for the file to be saved
        if (saved)
        {
            imageHistory.commitImage(workspaceArea->getImage(), "Save Image");
            fileSaved = true;
        }
        else
        {
            fileSaved = false;
        }
        return saved;
    }
}

// Clears the workspaceArea
void MainWindow::on_actionNew_triggered()
{
    if (maybeSave())
    {
        clearImage();
    }
    workspaceArea->setModified(false);
}

// Check if the current image has been changed and then open a dialog to open a file
void MainWindow::on_actionOpen_triggered()
{
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

            QImageReader reader(fileName);
            QSize sizeOfImage = reader.size();
            int imageHeight = sizeOfImage.height();
            int imageWidth = sizeOfImage.width();

            // Recontruct our graphicsView and workspaceArea, according to the new image dimensions
            resetGraphicsViewScale();
            reconstructWorkspaceArea(imageWidth, imageHeight);

            resizedImageWidth = imageWidth;
            resizedImageHeight = imageHeight;

            basics->setImageDimensions(resizedImageWidth, resizedImageHeight);

            workspaceArea->openImage(loadedImage, imageWidth, imageHeight);
            resizeGraphicsViewBoundaries(imageWidth, imageHeight);

            fitImageToScreen(imageWidth, imageHeight);

            colors->setImagePreview(workspaceArea->commitImageForPreview());
            colors->resetSliders();
        }
    }
}

// Tries to save the file instead of saveAs the file, if it was once saved.
void MainWindow::on_actionSave_triggered()
{
    if (fileSaved)
    {
        bool saved = workspaceArea->saveImage(fileName, fileFormat.constData()); // Call for the file to be saved
        if (saved)
        {
            imageHistory.commitImage(workspaceArea->getImage(), "Save Image");
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

// Undo brush stroke
void MainWindow::on_actionUndo_triggered()
{
    if (strokeHistory.isEmpty())
    {
        return;
    }
    QGraphicsPathItem *toBeDeleted = strokeHistory.back();
    strokeHistory.pop_back();
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

// Filter mouse scroll event to not propagate to the whole mainwindow, instead only to the workspaceArea
// Mouse scroll/wheel event will be handled by MainWindow::handleWheelEvent(QGraphicsSceneWheelEvent*)
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

// Zooms in/out the workspaceArea/graphicsView on mouse scroll/wheel event
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
    int originalResizedImageWidth = resizedImageWidth;
    int originalResizedImageHeight = resizedImageHeight;

    /*
     * Register a scroll if the scroll is vertical, within 7 (or -7) degree
     * with maximum workspaceArea dimensions equal to the upperbound
     * and minimum workspaceArea dimensions to the lowerbound
     */
    if (event->orientation() == Qt::Vertical)
    {
        if (event->delta() > 7 && resizedImageWidth < upperBound  && resizedImageHeight < upperBound)
        {
            // Update resizedImage dimensions
            resizedImageWidth = static_cast<int>(resizedImageWidth * scaleFactor);
            resizedImageHeight = static_cast<int>(resizedImageHeight * scaleFactor);
//            graphicsView->scale(scaleFactor, scaleFactor);
            graphicsView->scale((double)resizedImageWidth / (double) originalResizedImageWidth, (double)resizedImageHeight / (double)originalResizedImageHeight);
            resizeGraphicsViewBoundaries(resizedImageWidth, resizedImageHeight);
            currentZoom *= scaleFactor;
        }
        else if (event->delta() < -7 && resizedImageWidth > lowerBound && resizedImageHeight > lowerBound)
        {
            resizedImageWidth = static_cast<int>(resizedImageWidth * (1.0 / scaleFactor));
            resizedImageHeight = static_cast<int>(resizedImageHeight * (1.0 / scaleFactor));
//            graphicsView->scale(1.0/ scaleFactor, 1.0/ scaleFactor);
            graphicsView->scale((double) resizedImageWidth/ (double)originalResizedImageWidth, (double) resizedImageHeight / (double)originalResizedImageHeight);
            resizeGraphicsViewBoundaries(resizedImageWidth, resizedImageHeight);
            currentZoom /= scaleFactor;
        }
    }

    // Scrolls the contents of the view to ensure the item(workspaceArea) will always be fully visible in the view
    graphicsView->centerOn(0, 0);
    graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void MainWindow::onZoom(const QString &level)
{
    if (level == "Fit to screen")
    {
        fitImageToScreen(resizedImageWidth, resizedImageHeight);
        return;
    }

    double originalFactor = 1.0 / currentZoom;
    double a = resizedImageWidth * originalFactor;
    double b = resizedImageHeight * originalFactor;
    graphicsView->scale(a / resizedImageWidth, b / resizedImageHeight);
    resizeGraphicsViewBoundaries(resizedImageWidth * originalFactor, resizedImageHeight * originalFactor);
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

    a = resizedImageWidth * scaleFactor;
    b = resizedImageHeight * scaleFactor;
    graphicsView->scale(a / resizedImageWidth, b / resizedImageHeight);
    resizeGraphicsViewBoundaries(resizedImageWidth * scaleFactor, resizedImageHeight * scaleFactor);
    resizedImageWidth *= scaleFactor;
    resizedImageHeight *= scaleFactor;

    graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

// This slot will receive basic controls' cursor change signals.
// Drawing/Cropping/cutting an image will be signaled by the change of cursor state
void MainWindow::onCrossCursorChanged(WorkspaceArea::CursorMode cursor, int data)
{
    graphicsView->setCursor(Qt::CrossCursor);
    switch(cursor) {
    case WorkspaceArea::CursorMode::RECTANGLECROP:
        workspaceArea->setCursorMode(WorkspaceArea::CursorMode::RECTANGLECROP);
        break;
    case WorkspaceArea::CursorMode::MAGICWAND:
        workspaceArea->setCursorMode(WorkspaceArea::CursorMode::MAGICWAND);
        workspaceArea->setMagicWandThreshold(data);
        break;
    case WorkspaceArea::CursorMode::LASSO:
        workspaceArea->setCursorMode(WorkspaceArea::CursorMode::LASSO);
        break;
    case WorkspaceArea::CursorMode::SCRIBBLE:
        graphicsView->setCursor(Qt::ArrowCursor);
        workspaceArea->setCursorMode(WorkspaceArea::CursorMode::SCRIBBLE);
        break;
    }
}

void MainWindow::fitImageToScreen(int currentImageWidth, int currentImageHeight)
{
    const QRect screenRect = WindowHelper::screenFromWidget(qApp->desktop())->geometry();
    int screenWidth = screenRect.width();
    int screenHeight = screenRect.height();

    double ratio;
    /*
     * If any of the currentImage dimensions is larger than the screen dimensions, we would like to scale down
     * Else if any of the currentImage dimensions is smaller than the screen dimensions, we would like to scale up
     * Otherwise, both dimensions are equal, we can leave the function.
     */
    if (currentImageWidth > screenWidth || currentImageHeight > screenHeight)
    {
        ratio = qMax((double)screenWidth / currentImageWidth, (double)screenHeight / currentImageHeight) * 0.5;
    }
    else if (currentImageWidth < screenWidth || currentImageHeight < screenHeight)
    {
        ratio = qMin((double)screenWidth / currentImageWidth, (double)screenHeight / currentImageHeight) * 0.5;
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

/*
 * Mechanism to view the cropped/filtered/transformed image:
 1. store current workspaceArea to a temporaryArea
 2. create a new workspaceArea to hold the new cropped image, and set the graphicsView to contain this new workspaceArea
 3. the temporaryArea will then be destroyed when a) destroyed by the destructor, or b) there is a new signal to crop the image.
 * We cannot destroy/reconstruct the workspaceArea in this slot, as this slot is connected (by signal) to the caller workspaceArea.
 */
void MainWindow::rerenderWorkspaceArea(const QImage& image, int imageWidth, int imageHeight)
{
    // Before rerenderingWorkspaceArea, we make sure our stroke history is clean
    while (!strokeHistory.isEmpty()) {
        on_actionUndo_triggered();
    }

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

void MainWindow::resetGraphicsViewScale()
{
    // Resize image dimensions properties by reverting horizontal/vertical scaling factor changes
    resizedImageWidth /= graphicsView->matrix().m11();
    resizedImageHeight /= graphicsView->matrix().m22();

    // Reset graphics view scaling factor
    graphicsView->resetMatrix();

    resizeGraphicsViewBoundaries(resizedImageWidth, resizedImageHeight);
    graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    //    currentZoom = 1 / totalScaleX;        // is this needed?????
}

void MainWindow::applyFilterTransform(AbstractImageFilterTransform *filterTransform, int size, double strength)
{    
    // Commit all brush strokes before applying transform
    workspaceArea->commitImageAndSet();

    // Get filtered image and rerender the workspaceArea with the new image.
    QImage&& result = filterTransform->applyFilter(workspaceArea->getImage(), size, strength);
    rerenderWorkspaceArea(result, result.width(), result.height());

    // Add after-filter-applied image to our image history version control, generate our history menu
    imageHistory.commitImage(workspaceArea->getImage(), filterTransform->getName());
    generateHistoryMenu();

    delete filterTransform;
}

void MainWindow::applyFilterTransformOnPreview(AbstractImageFilterTransform *filterTransform, int size, double strength)
{
    QImage&& previewImageNoFilter = workspaceArea->commitImageForPreview();
    QImage&& result = filterTransform->applyFilter(previewImageNoFilter, size, strength);
    colors->setImagePreview(result);
    delete filterTransform;
}

void MainWindow::onUpdateImagePreview() {
     QImage&& previewImage = workspaceArea->commitImageForPreview();
     colors->setImagePreview(previewImage);
}

void MainWindow::onCreateRoom(QString name) {
    username = name;
    if (name.isEmpty()) {
        QMessageBox::information(this, QString("Empty Name"), QString("Name cannot be empty"));
        return;
    }

    server = new Server(this);
    ip = server->getIP();
    port = server->getPort();
    isHost = true;
    qDebug() << ip << port;
    joinRoom();

}

void MainWindow::onJoinRoom(QString name, QString address, quint16 port) {
    ip = address;
    if (ip.isEmpty()) {
        QMessageBox::information(this, QString("Empty Server Name"), QString("Server name cannot be empty"));
        return;
    }

    this->port = port;
    if (!port) {
        QMessageBox::information(this, QString("Invalid Port"), QString("Invalid Port"));
        return;
    }

    username = name;
    if (username.isEmpty()) {
        QMessageBox::information(this, QString("Empty Name"), QString("Name cannot be empty"));
        return;
    }

    isHost = false;
    joinRoom();
}

void MainWindow::joinRoom() {
    client = new Client();
    client->connectToServer(QHostAddress(ip), port);
    room->setServerRoom();
    connect(client, &Client::receiveJson, this, &MainWindow::clientJsonReceived);
    connect(client, &Client::connected, this, &MainWindow::sendPlayerName);
    //connect(client, &Client::disconnected, this, &MainWindow::forceLeaveRoom);
}

void MainWindow::on_actionCreate_Room_triggered()
{
    room = new ServerRoom();
    room->setCreateRoom();
    connect(room, &ServerRoom::createRoom, this, &MainWindow::onCreateRoom);
    room->setModal(true);
    room->exec();

}

void MainWindow::on_actionJoin_Room_triggered()
{
    room = new ServerRoom();
    room->setJoinRoom();
    connect(room, &ServerRoom::joinRoom, this, &MainWindow::onJoinRoom);
    room->setModal(true);
    room->exec();
}

void MainWindow::clientJsonReceived(const QJsonObject &json) {
    qDebug() << json;
    const QString type = json.value(QString("type")).toString();
    if (type == "newPlayer") {
        //addPlayer(json.value(QString("playerName")).toString());
        room->addPlayer(json.value(QString("playerName")).toString());
    }
}

void MainWindow::sendPlayerName() {
    QJsonObject playerNameMsg;
    playerNameMsg["type"] = "playerName";
    playerNameMsg["playerName"] = username;
    client->sendJson(playerNameMsg);
}



void MainWindow::on_actionCommit_Image_triggered()
{

}
