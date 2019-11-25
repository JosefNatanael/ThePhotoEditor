#include "MagicWand.h"
#include "PixelHelper.h"
#include <QDebug>
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
    QImage newImage{image};
    return newImage;
}

// No change
QImage MagicWand::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}

QImage MagicWand::crop(const QImage &image, int x, int y, int threshold)
{
    QImage newImage{image};
    QRgb originalColor = PixelHelper::getPixel(image, x, y);
    originalColorRed = qRed(originalColor);
    originalColorGreen = qGreen(originalColor);
    originalColorBlue = qBlue(originalColor);
    this->threshold = threshold;
    recursiveMagic(newImage, x, y);
//    forestFire(newImage, x, y);
    return newImage;
}

//void MagicWand::forestFire(QImage &img, int x, int y)
//{
//    // If pixel out of bound, return
//    if (x < 0 || x >= img.width() || y < 0 || y >= img.height())
//        return;

//    QRgb currentColor = PixelHelper::getPixel(img, x, y);
//    double red = originalColorRed - qRed(currentColor);
//    double green = originalColorGreen - qGreen(currentColor);
//    double blue = originalColorBlue - qBlue(currentColor);

//    // If pixel is not within threshold, we do not need to edit the pixel, thus we return
//    if (abs(red) > threshold || abs(green) > threshold || abs(blue) > threshold)
//        return;

//    /*3. Set the color of node to replacement-color.
//      4. Set Q to the empty queue.
//      5. Add node to the end of Q.
//      6. While Q is not empty:
//      7.     Set n equal to the first element of Q.
//      8.     Remove first element from Q.
//      9.     If the color of the node to the west of n is target-color,
//                 set the color of that node to replacement-color and add that node to the end of Q.
//     10.     If the color of the node to the east of n is target-color,
//                 set the color of that node to replacement-color and add that node to the end of Q.
//     11.     If the color of the node to the north of n is target-color,
//                 set the color of that node to replacement-color and add that node to the end of Q.
//     12.     If the color of the node to the south of n is target-color,
//                 set the color of that node to replacement-color and add that node to the end of Q.
//     13. Continue looping until Q is exhausted.
//     14. Return.
//     */

//    // Set the color of the current pixel to transparent
//    PixelHelper::setPixel(img, x, y, Qt::transparent);

//    // Add node to the end of our queue
//    forestFireQueue.enqueue(Point(x, y));

//    Point n;
//    while (!forestFireQueue.empty()) {
//        n = forestFireQueue.dequeue();
//        if (n.getX() + 1 < img.height()) {

//        }
//        if (n.getX() - 1 >= 0) {

//        }
//        if (n.getY() + 1 < img.height())
//    }

//}

void MagicWand::recursiveMagic(QImage &img, int x, int y)
{
    if ((x >= 0 && x < img.width()) && (y >= 0 && y < img.height()))
    {
        QRgb currentColor = PixelHelper::getPixel(img, x, y);

        double red = originalColorRed - qRed(currentColor);
        double green = originalColorGreen - qGreen(currentColor);
        double blue = originalColorBlue - qBlue(currentColor);
//        double redMean = (qRed(thisColor) + qRed(currentColor)) / 2;
//        double colorDistance = qSqrt((2 + redMean / 256) * red * red + 4 * green * green + (2 + (255 - redMean) / 256) * blue * blue); //color perceive by human eyes
//        if (colorDistance <= 10)
        if (abs(red) < threshold && abs(green) < threshold && abs(blue) < threshold)
        {
            PixelHelper::setPixel(img, x, y, Qt::transparent);
            recursiveMagic(img, x + 1, y);
            recursiveMagic(img, x - 1, y);
            recursiveMagic(img, x, y - 1);
            recursiveMagic(img, x, y + 1);
        }
    }
}
