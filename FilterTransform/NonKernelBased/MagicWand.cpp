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
    QRgb color = PixelHelper::getPixel(image, x, y);
    recursiveMagic(newImage, x, y, color, threshold);
    return newImage;
}

void MagicWand::recursiveMagic(QImage &img, int x, int y, QRgb thisColor, int threshold)
{
    if ((x > 0 && x < img.height()) && (y > 0 && y < img.width()))
    {
        QRgb currentColor = PixelHelper::getPixel(img, x, y);

        double red = qRed(thisColor) - qRed(currentColor);
        double green = qGreen(thisColor) - qGreen(currentColor);
        double blue = qBlue(thisColor) - qBlue(currentColor);
//        double redMean = (qRed(thisColor) + qRed(currentColor)) / 2;

//        double colorDistance = qSqrt((2 + redMean / 256) * red * red + 4 * green * green + (2 + (255 - redMean) / 256) * blue * blue); //color perceive by human eyes
//        if (colorDistance <= 10)
        if (abs(red) < threshold && abs(green) < threshold && abs(blue) < threshold)
        {
            PixelHelper::setPixel(img, x, y, Qt::transparent);
            recursiveMagic(img, x + 1, y, thisColor, threshold);
            recursiveMagic(img, x - 1, y, thisColor, threshold);
            recursiveMagic(img, x, y - 1, thisColor, threshold);
            recursiveMagic(img, x, y + 1, thisColor, threshold);
        }
    }
}
