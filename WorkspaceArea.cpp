#include "WorkspaceArea.h"
#include "PixelHelper.h"
#include "FilterTransform/NonKernelBased/MagicWand.h"

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

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

// Used to load the image and place it in the widget
bool WorkspaceArea::openImage(const QImage &loadedImage, int imageWidth, int imageHeight)
{
	image = loadedImage.copy();
	isImageLoaded = true;
	this->imageWidth = imageWidth;
	this->imageHeight = imageHeight;

	QImage &&scaledImage = image.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

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
	return true;
}

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

// Save the current image
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
		QPainterPath path;
		if (pathItem == nullptr)
		{
			path.moveTo(event->scenePos()); // move path to event scene position
			pathItem = new QGraphicsPathItem();
			pen.setColor(myPenColor);
			pen.setWidth(myPenWidth);
			pathItem->setPen(pen);   // set sticker pen to default pen
			pathItem->setPath(path); // set sticker path to the new path
			this->addItem(pathItem);
			emit edited(pathItem);
		}

		path = pathItem->path();		// get path from sticker
		path.lineTo(event->scenePos()); // draw line from last event pos to current pos
		pathItem->setPath(path);
		pathItem->update();
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

void WorkspaceArea::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	switch (cursorMode)
	{
	case CursorMode::SCRIBBLE:
		pathItem = nullptr;
		break;
	case CursorMode::RECTANGLECROP:
	{
		if (cropX < 0)
		{
			cropOrigin.setX(cropOrigin.x() + cropX);
			cropX = cropOrigin.x() - cropX;
		}

		if (cropY < 0)
		{
			cropOrigin.setY(cropOrigin.y() + cropY);
			cropY = cropOrigin.y() - cropY;
		}

		QRect cropRect = QRect(cropOrigin.x(), cropOrigin.y(), cropX, cropY);

        QImage&& uncroppedImage = commitImage();
        QImage&& croppedImage = uncroppedImage.copy(cropRect);

		delete rubberBand;
		rubberBand = nullptr;
		emit imageCropped(croppedImage, cropX, cropY);
		break;
	}
	case CursorMode::MAGICWAND:
		thisColor = PixelHelper::getPixel(image, cropOrigin.y(), cropOrigin.x());
		MagicWand m;
        commitImageAndSet();
        QImage&& newImage = m.crop(image, cropOrigin.y(), cropOrigin.x(), magicWandThreshold);
        emit imageCropped(newImage, newImage.width(), newImage.height());
		break;
	}
	QGraphicsScene::mouseReleaseEvent(event);
}

void WorkspaceArea::resizeImage(int width, int height)
{
    QImage&& unresizedImage = commitImage();
    QImage&& resizedImage = unresizedImage.scaled(width, height, Qt::AspectRatioMode::KeepAspectRatio);

    emit imageResized(resizedImage, width, height);
}

// Print the image
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
