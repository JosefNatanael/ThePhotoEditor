#include "MagicWand.h"
#include <QtMath>

MagicWand::MagicWand(QObject *parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{
}

QString MagicWand::getName() const
{
    return "MagicWand";
}

QImage MagicWand::applyFilter(const QImage &image, double) const
{
    return QImage(image);
}

// No change
QImage MagicWand::applyFilter(const QImage &image) const
{
    return QImage(image);
}

QImage MagicWand::crop(const QImage &image, int x, int y, int threshold)
{
    QImage newImage{image};
    QRgb originalColor = PixelHelper::getPixel(image, x, y);
    originalColorRed = qRed(originalColor);
    originalColorGreen = qGreen(originalColor);
    originalColorBlue = qBlue(originalColor);
    this->threshold = threshold;
    forestFire(newImage, x, y);
    return newImage;
}

bool MagicWand::colorWithinThreshold(QRgb colorToCheck)
{
    if (colorToCheck == Qt::transparent) {
        return false;
    }
    if (abs(originalColorRed - qRed(colorToCheck)) <= threshold
            && abs(originalColorGreen - qGreen(colorToCheck)) <= threshold
            && abs(originalColorBlue - qBlue(colorToCheck)) <= threshold) {
        return true;
    }
    return false;
}

void MagicWand::forestFire(QImage &img, int x, int y)
{
    // If pixel out of bound, return
    if (x < 0 || x >= img.width() || y < 0 || y >= img.height())
        return;

    // If pixel is not within threshold, we do not need to edit the pixel, thus we return
    if (!colorWithinThreshold(PixelHelper::getPixel(img, x, y)))
        return;

    // Set the color of the current pixel to transparent
    PixelHelper::setPixel(img, x, y, Qt::transparent);

    // Add node to the end of our queue
    forestFireQueue.enqueue(Point(x, y));

    Point n;
    while (!forestFireQueue.empty()) {
        n = forestFireQueue.dequeue();

        // If the color of adjacent pixels is within the threshold,
        // set the adjacent pixels to transparent, and add them to the end of the queue.
        if (n.getX() + 1 < img.width()) {
            if (colorWithinThreshold(PixelHelper::getPixel(img, n.getX() + 1, n.getY()))) {
                PixelHelper::setPixel(img, n.getX() + 1, n.getY(), Qt::transparent);
                forestFireQueue.enqueue(Point(n.getX() + 1, n.getY()));
            }
        }
        if (n.getX() - 1 >= 0) {
            if (colorWithinThreshold(PixelHelper::getPixel(img, n.getX() - 1, n.getY()))) {
                PixelHelper::setPixel(img, n.getX() - 1, n.getY(), Qt::transparent);
                forestFireQueue.enqueue(Point(n.getX() - 1, n.getY()));
            }
        }
        if (n.getY() + 1 < img.height()) {
            if (colorWithinThreshold(PixelHelper::getPixel(img, n.getX(), n.getY() + 1))) {
                PixelHelper::setPixel(img, n.getX(), n.getY() + 1, Qt::transparent);
                forestFireQueue.enqueue(Point(n.getX(), n.getY() + 1));
            }
        }
        if (n.getY() - 1 >= 0) {
            if (colorWithinThreshold(PixelHelper::getPixel(img, n.getX(), n.getY() - 1))) {
                PixelHelper::setPixel(img, n.getX(), n.getY() - 1, Qt::transparent);
                forestFireQueue.enqueue(Point(n.getX(), n.getY() - 1));
            }
        }
    }

}
