#include "WorkspaceArea.h"

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

static const int SCENE_WIDTH = 720;    // The default width of the workspace
static const int SCENE_HEIGHT = 480;   // The default height of the workspace

WorkspaceArea::WorkspaceArea(QObject *parent)
    : QGraphicsScene (0, 0, SCENE_WIDTH, SCENE_HEIGHT, parent)
{
    // Set default workspace resolution
    imageWidth = SCENE_WIDTH;
    imageHeight = SCENE_HEIGHT;
    // Set default values for the monitored variables
    modified = false;
    myPenWidth = 5;
    myPenColor = Qt::blue;
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
}

WorkspaceArea::WorkspaceArea(int width, int height, QObject *parent)
    : QGraphicsScene (0, 0, width, height, parent)
{
    imageWidth = width;
    imageHeight = height;
    // Set default values for the monitored variables
    modified = false;
    myPenWidth = 5;
    myPenColor = Qt::blue;
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
}

// Used to load the image and place it in the widget
bool WorkspaceArea::openImage(const QImage& loadedImage, int imageWidth, int imageHeight)
{
    image = loadedImage;
    imageLoaded = true;
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;

    QImage &&scaledImage = image.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    if (pixmapGraphics) {
        removeItem(pixmapGraphics);
        delete pixmapGraphics;
    }
    pixmapGraphics = addPixmap(QPixmap::fromImage(scaledImage));
    pixmapGraphics->setTransformationMode(Qt::SmoothTransformation);
    pixmapGraphics->setPos({imageWidth / 2.0 - scaledImage.width() / 2.0, imageHeight / 2.0 - scaledImage.height() / 2.0});
    pixmapGraphics->setZValue(-2000);

    emit onImageLoaded(image);

    modified = false;
    update();
    return true;
}

QImage WorkspaceArea::commitImage()
{
    if (imageLoaded) {
        QImage commitImage(imageWidth, imageHeight, QImage::Format_ARGB32_Premultiplied);
        QPainter painter;
        painter.begin(&commitImage);
        render(&painter);                                                               // Renders the Workspace area to the image
        painter.end();
        return commitImage;
    }
    else {  // We generate a white canvas
        clearSelection();                                                               // Selections would also render to the file
        setSceneRect(this->itemsBoundingRect());                                        // Re-shrink the scene to it's bounding contents
        QImage commitImage(this->sceneRect().size().toSize(), QImage::Format_ARGB32);   // Create the image with the exact size of the shrunk scene
        if (commitImage.isNull()) {
            return commitImage;
        }
        commitImage.fill(Qt::white);                                                    // Start all pixels white color

        QPainter painter(&commitImage);
        render(&painter);
        return commitImage;
    }
}

// Save the current image
bool WorkspaceArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage&& snap = commitImage();
    image = snap;
    if (snap.isNull()) {
        return false;
    }
    if (snap.save(fileName, fileFormat)) {
        modified = false;
        return true;
    }
    else {
        return false;
    }
}

void WorkspaceArea::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}

void WorkspaceArea::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) == 0) {
        QGraphicsScene::mouseMoveEvent(event);
        return;
    }

    QPainterPath path;
    if(pathItem == nullptr) {
        path.moveTo(event->scenePos());					// move path to event scene position
        pathItem = new QGraphicsPathItem();
        pen.setColor(myPenColor);
        pen.setWidth(myPenWidth);
        pathItem->setPen(pen);                          // set sticker pen to default pen
        pathItem->setPath(path);						// set sticker path to the new path
        this->addItem(pathItem);
        emit edit(pathItem);
    }

    path = pathItem->path();							// get path from sticker
    path.lineTo(event->scenePos());						// draw line from last event pos to current pos
    pathItem->setPath(path);
    pathItem->update();

    QGraphicsScene::mouseMoveEvent(event);
    modified = true;
}

void WorkspaceArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    pathItem = nullptr;
    QGraphicsScene::mouseReleaseEvent(event);
}

// Print the image
void WorkspaceArea::print()
{
    // Check for print dialog availability
#if QT_CONFIG(printdialog)

    QPrinter printer(QPrinter::HighResolution);

    // Open printer dialog and print if asked
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}

void WorkspaceArea::resize(int width, int height)
{
    //pixmapGraphics->setPos({-width*0.5, -height*0.5});
    this->imageWidth = width;
    this->imageHeight = height;
}

