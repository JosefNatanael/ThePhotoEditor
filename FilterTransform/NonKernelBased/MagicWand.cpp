/**
 * @class MagicWand
 * @brief Magic Wand Non-Kernel Implementation.
 */
#include "MagicWand.h"
#include <QtMath>

/**
 * @brief Construct a new Magic Wand:: Magic Wand object
 *
 * @param parent Passed to AbstractNonKernelBasedImageFilterTransform() constructor.
 */
MagicWand::MagicWand(QObject *parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{
}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString MagicWand::getName() const
{
    return "Magic Wand";
}

/**
 * @brief This is an overloaded function.
 *
 * @return QImage exact copy of image.
 */
QImage MagicWand::applyFilter(const QImage &image, double) const
{
    return QImage(image);
}

/**
 * @brief This is an overloaded function.
 *
 * @return QImage exact copy of image.
 */
QImage MagicWand::applyFilter(const QImage &image) const
{
    return QImage(image);
}

/**
 * @brief Gets new image after filter applied.
 *
 * @param image Original image to get new filter applied image.
 * @param x Position of pixel from the top edge
 * @param y Position of pixel from the left edge
 * @param threshold Tolerance for Magic Wand
 *
 * Construct newImage from image
 * Apply the forestFire algorithm starting from the (x,y) pixel to newImage
 * @return QImage Filter applied image - newImage
 */
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

/**
 * @brief Helper function to check tolerance of pixel
 *
 * @param colorToCheck the QRgb value as the sample color
 *
 * Check whether colorToCheck is within the threshold range
 * Compared to originalColorRed, originalColorGreen, and originalColorBlue
 * @return bool true if within tolerance, else false
 */
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

/**
 * @brief Implementation of Forest Fire Algorithm for Magic Wand Implementation
 *
 * @param img Original image to get new filter applied image.
 * @param x Position of pixel from the top edge
 * @param y Position of pixel from the left edge
 *
 * Check if (x,y) pixel is within the image and within the color threshold
 * Set pixel to transparent and enqueue QPoint (x,y) into forestFireQueue
 * Recursively check the top, left, right, and down neighbouring pixels in the same way
 * @return void
 */
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
