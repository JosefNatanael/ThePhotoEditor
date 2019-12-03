/**
 * @class WorkspaceArea
 * @brief Stores and handles image related functions, viewing.
 * This class inherits the QGraphicsScene parent class.
 * Thus, the instance of this class will be handled by a QGraphicsView.
 */

#include "WorkspaceArea.h"
#include "Utilities/PixelHelper.h"
#include "FilterTransform/NonKernelBased/MagicWand.h"

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

/**
 * @brief Construct a new Workspace Area:: Workspace Area object.
 * 
 * @param parent To be passed to QGraphicScene constructor.
 */
WorkspaceArea::WorkspaceArea(QObject *parent)
	: QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT, parent)
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

/**
 * @brief Construct a new Workspace Area:: Workspace Area object.
 * 
 * @details Workspace area will have dimensions width and height.
 * 
 * @param width Workspace area width.
 * @param height Workspace area height.
 * @param parent To be passed to QGraphicScene constructor.
 */
WorkspaceArea::WorkspaceArea(int width, int height, QObject *parent)
	: QGraphicsScene(0, 0, width, height, parent)
{
	// Set workspace resolution
	imageWidth = width;
	imageHeight = height;
	// Set default values for the monitored variables
	modified = false;
	myPenWidth = 5;
	myPenColor = Qt::blue;
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
}

/**
 * @brief Place image in the workspace area.
 * 
 * @param loadedImage Image to load.
 * @param imageWidth Width of thhe image
 * @param imageHeight height of the image
 */
void WorkspaceArea::openImage(const QImage &loadedImage, int imageWidth, int imageHeight)
{
    image = loadedImage.copy();
	isImageLoaded = true;
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;

    QImage &&scaledImage = loadedImage.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

	// Loads the image to the workspace area.
	if (pixmapGraphics)
	{
		removeItem(pixmapGraphics);
		delete pixmapGraphics;
	}
    pixmapGraphics = addPixmap(QPixmap::fromImage(scaledImage));
	pixmapGraphics->setTransformationMode(Qt::SmoothTransformation);
	pixmapGraphics->setPos({imageWidth / 2.0 - scaledImage.width() / 2.0, imageHeight / 2.0 - scaledImage.height() / 2.0});
	pixmapGraphics->setZValue(-2000);

	emit imageLoaded(image);

	modified = false;
	update();
}

/**
 * @brief Makes the brush strokes/drawing permanent, i.e. fused into the image.
 * 
 * @return QImage Fused image (fused with brush strokes).
 */
QImage WorkspaceArea::commitImage()
{
	if (isImageLoaded)
	{
		QImage commitImage(imageWidth, imageHeight, QImage::Format_ARGB32_Premultiplied);
		QPainter painter;
		painter.begin(&commitImage);
		render(&painter); // Renders the Workspace area to the image
		painter.end();
		return commitImage;
	}
	else
	{
		// We generate a white canvas
		clearSelection();															  // Selections would also render to the file
		setSceneRect(this->itemsBoundingRect());									  // Re-shrink the scene to it's bounding contents
		QImage commitImage(this->sceneRect().size().toSize(), QImage::Format_ARGB32); // Create the image with the exact size of the shrunk scene
		if (commitImage.isNull())
		{
			return commitImage;
		}
		commitImage.fill(Qt::white); // Start all pixels white color

		QPainter painter(&commitImage);
		render(&painter);
		return commitImage;
	}
}

/**
 * @brief Saves the current image.
 * 
 * @param fileName 
 * @param fileFormat 
 * @return true image saved
 * @return false image not saved
 */
bool WorkspaceArea::saveImage(const QString &fileName, const char *fileFormat)
{
	QImage &&snap = commitImage();
	image = snap;
	if (snap.isNull())
	{
		return false;
	}
	if (snap.save(fileName, fileFormat))
	{
		modified = false;
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * @brief Mouse press event override.
 * 
 * @details Press event will be based on which cursor is selected.
 * 
 * @param event Mouse event.
 */
void WorkspaceArea::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (cursorMode == CursorMode::RECTANGLECROP)
	{
		cropOriginScreen = event->screenPos();
		cropOrigin = event->scenePos().toPoint();
		if (!rubberBand)
		{
			rubberBand = new QRubberBand(QRubberBand::Rectangle, nullptr);
			rubberBand->setGeometry(QRect(cropOrigin, QSize()));
			rubberBand->show();
		}
	}
	else if (cursorMode == CursorMode::MAGICWAND)
	{
		cropOriginScreen = event->screenPos();
		cropOrigin = event->scenePos().toPoint();
	}
	QGraphicsScene::mousePressEvent(event);
}

/**
 * @brief Mouse move event override.
 * 
 * @details Move event will be based on which cursor is selected.
 * 
 * @param event Mouse event.
 */
void WorkspaceArea::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) == 0)
	{
		QGraphicsScene::mouseMoveEvent(event);
		return;
	}

	switch (cursorMode)
	{
	case CursorMode::SCRIBBLE:
	{
        onMoveScribble(event->scenePos(), myPenColor, myPenWidth);
        sendMoveScribble(event->scenePos().x(), event->scenePos().y(), myPenColor.name(), myPenWidth);
		break;
	}
	case CursorMode::RECTANGLECROP:
	{
		if (cropOrigin.x() + (event->scenePos().toPoint().x() - cropOrigin.x()) <= width() &&
			cropOrigin.x() + (event->scenePos().toPoint().x() - cropOrigin.x()) > 0)
		{
			cropX = event->scenePos().toPoint().x() - cropOrigin.x();
			dx = event->screenPos().x() - cropOriginScreen.x();
		}
		if (cropOrigin.y() + (event->scenePos().toPoint().y() - cropOrigin.y()) <= height() &&
			cropOrigin.y() + (event->scenePos().toPoint().y() - cropOrigin.y()) > 0)
		{
			cropY = event->scenePos().toPoint().y() - cropOrigin.y();
			dy = event->screenPos().y() - cropOriginScreen.y();
		}

		QRect rect = QRect(cropOriginScreen.x(), cropOriginScreen.y(), dx, dy).normalized();
		rubberBand->setGeometry(rect);
		break;
	}
	}
	QGraphicsScene::mouseMoveEvent(event);
	modified = true;
}

void WorkspaceArea::onMoveScribble(QPointF pos, QColor penColor, int penWidth) {
    QPainterPath path;
    if (pathItem == nullptr)
    {
        path.moveTo(pos); // move path to event scene position
        pathItem = new QGraphicsPathItem();
        pen.setColor(penColor);
        pen.setBrush(penColor);
        pen.setWidth(penWidth);
        pathItem->setPen(pen);   // set sticker pen to default pen
        pathItem->setPath(path); // set sticker path to the new path
        this->addItem(pathItem);
    }

    path = pathItem->path();		// get path from sticker
    path.lineTo(pos); // draw line from last event pos to current pos
    pathItem->setPath(path);
    pathItem->update();
}

/**
 * @brief Mouse release event override.
 * 
 * @details Release event will be based on which cursor is selected.
 * 
 * @param event Mouse event.
 */
void WorkspaceArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	switch (cursorMode)
	{
	case CursorMode::SCRIBBLE:
        onReleaseScribble();
        emit sendReleaseScribble();
		break;
	case CursorMode::RECTANGLECROP:
	{
		if (cropX < 0)
		{
			cropOrigin.setX(cropOrigin.x() + cropX);
            cropX = -cropX;
		}

		if (cropY < 0)
		{
			cropOrigin.setY(cropOrigin.y() + cropY);
            cropY = -cropY;
		}

        cropImage(cropOrigin.x(), cropOrigin.y(), cropX, cropY);
		break;
	}
	case CursorMode::MAGICWAND:
        cropImageWithMagicWand(cropOrigin.x(), cropOrigin.y());
		break;
	}
	QGraphicsScene::mouseReleaseEvent(event);
}

void WorkspaceArea::onReleaseScribble() {
    emit updateImagePreview();
    emit imageDrawn();
    pathItem = nullptr;
}

void WorkspaceArea::cropImage(int x, int y, int width, int height, bool fromServer) {
    QRect cropRect = QRect(x, y, width, height);

    QImage &&uncroppedImage = commitImage();
    QImage &&croppedImage = uncroppedImage.copy(cropRect);

    if (rubberBand != nullptr) {
        delete rubberBand;
        rubberBand = nullptr;
    }

    emit commitChanges("Rectangle Crop");

    if (!fromServer) {

        emit sendCrop(x, y, width, height);
    }

    emit imageCropped(croppedImage, width, height);
}

/**
 * @brief Resizes image to width and height.
 * 
 * @param width resized image width.
 * @param height resized image height.
 */
void WorkspaceArea::resizeImage(int width, int height, bool fromServer)
{
	QImage &&unresizedImage = commitImage();
	QImage &&resizedImage = unresizedImage.scaled(width, height, Qt::AspectRatioMode::KeepAspectRatio);

    if (!fromServer) {
        emit sendResize(width, height);
    }
    
    emit commitChanges("Image Resized");
    
    emit imageResized(resizedImage, width, height);
}

void WorkspaceArea::cropImageWithMagicWand(int x, int y, bool fromServer) {
    thisColor = PixelHelper::getPixel(image, x, y);
    MagicWand m;
    commitImageAndSet();
    QImage &&newImage = m.crop(image, x, y, magicWandThreshold);
    emit imageCropped(newImage, newImage.width(), newImage.height());
    emit commitChanges("Magic Removal");
    if (!fromServer) {
        emit sendCropWithMagicWand(x, y);
    }
}

/**
 * @brief Commits the image. Passed to color controls' image previewer.
 * @details check commitImage() for how this works.
 * 
 * @return QImage Comitted image.
 */
QImage WorkspaceArea::commitImageForPreview()
{
	if (isImageLoaded)
	{
		// The image previewer will have dimensions 200x200
		// We sample a small portion of current image for previewing.
		int width = 200;
		int height = 200;
		int x = imageWidth / 2;
		int y = imageHeight / 2;
		if (imageWidth < 400 || imageHeight < 400)
		{
			width = height = qMin(imageWidth, imageHeight);
			x = y = 0;
		}
		QRect previewRect = QRect(x, y, width, height);
		// We get a committed image ready.
		QImage commitImage(imageWidth, imageHeight, QImage::Format_ARGB32_Premultiplied);
		QPainter painter;
		painter.begin(&commitImage);
		render(&painter); // Renders the Workspace area to the image
		painter.end();
		return commitImage.copy(previewRect).scaled(200, 200, Qt::KeepAspectRatio);
	}
	else
	{
		// Return a 200x200 white image if there is no image loaded in the workspace area.
		QImage commitImage(200, 200, QImage::Format_ARGB32_Premultiplied);
		commitImage.fill(Qt::white);
		return commitImage;
	}
}

/**
 * @brief Prints the image. Opens a printer dialog.
 */
void WorkspaceArea::print()
{
	// Check for print dialog availability
#if QT_CONFIG(printdialog)

	QPrinter printer(QPrinter::HighResolution);

	// Open printer dialog and print if asked
	QPrintDialog printDialog(&printer);
	if (printDialog.exec() == QDialog::Accepted)
	{
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
