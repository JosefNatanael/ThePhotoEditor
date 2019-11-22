#include "MagicWand.h"
#include "PixelHelper.h"

MagicWand::MagicWand(QObject* parent): AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString MagicWand::getName() const
{
    return "MagicWand";
}

QImage MagicWand::applyFilter(const QImage &image, double strength) const
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

QImage MagicWand::crop(const QImage &image, int x, int y){
    QImage newImage{image};
    QRgb color = PixelHelper::getPixel(image, x, y);
    recursiveMagic(newImage, x, y, color);
    return newImage;
}

void MagicWand::recursiveMagic(QImage &img, int x, int y, QRgb thisColor)
{
    QRgb currentColor = PixelHelper::getPixel(img, x, y);
    if(currentColor == thisColor){
        PixelHelper::setPixel(img, x, y, Qt::transparent);
    }
    recursiveMagic(img, x+1, y, thisColor);
    recursiveMagic(img, x-1, y, thisColor);
    recursiveMagic(img, x, y-1, thisColor);
    recursiveMagic(img, x, y+1, thisColor);
}
