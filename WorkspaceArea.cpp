#include "WorkspaceArea.h"

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

static const int SCENE_WIDTH = 1080;    // The width of the workspace
static const int SCENE_HEIGHT = 1920;   // The height of the workspace

WorkspaceArea::WorkspaceArea(QObject *parent)
    : QGraphicsScene (0, 0, SCENE_WIDTH, SCENE_HEIGHT, parent)
{
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
    // Set default values for the monitored variables
    modified = false;
    myPenWidth = 5;
    myPenColor = Qt::blue;
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
}

// Used to load the image and place it in the widget
bool WorkspaceArea::openImage(const QString &fileName)
{
    // Holds the image
    QImage loadedImage;

    if (!loadedImage.load(fileName)) {
        return false;
    }

    image = loadedImage;

    QImageReader reader(fileName);
    QSize sizeOfImage = reader.size();
    imageHeight = sizeOfImage.height();
    imageWidth = sizeOfImage.width();

    QImage &&scaledImage = image.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    pixmapGraphics = addPixmap(QPixmap::fromImage(scaledImage));
    pixmapGraphics->setTransformationMode(Qt::SmoothTransformation);
    pixmapGraphics->setPos({imageWidth / 2.0 - scaledImage.width() / 2.0, imageHeight / 2.0 - scaledImage.height() / 2.0});
    pixmapGraphics->setZValue(-1000);

    emit onImageLoaded(loadedImage);

    modified = false;
    update();
    return true;
}

QImage WorkspaceArea::commitImage()
{
    QImage image(imageWidth, imageHeight, QImage::Format_ARGB32_Premultiplied);
    QPainter painter;
    painter.begin(&image);
    render(&painter);       // Renders the Workspace area to the image
    painter.end();
    return image;
}

// Save the current image
bool WorkspaceArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage&& snap = commitImage();
    if (snap.save(fileName, fileFormat)) {
        modified = false;
        return true;
    }
    else {
        return false;
    }
}

// Color the image area with white
void WorkspaceArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
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

    // Can be used to print
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
