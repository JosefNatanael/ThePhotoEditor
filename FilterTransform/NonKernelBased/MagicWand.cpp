#include "MagicWand.h"
#include "PixelHelper.h"
#include <QDebug>
#include <QtMath>

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
    if((x>0&&x<img.height()) && (y>0&&y<img.width())){
        QRgb currentColor = PixelHelper::getPixel(img, x, y);
        double currentRed = qRed(currentColor);
        double thisRed = qRed(thisColor);
        double currentGreen = qGreen(currentColor);
        double thisGreen = qGreen(thisColor);
        double currentBlue = qBlue(currentColor);
        double thisBlue = qBlue(thisColor);

        double red = thisRed - currentRed;
        double green = thisGreen - currentGreen;
        double blue = thisBlue - currentBlue;
        double redMean = (thisRed + currentRed)/2;

        double colorDistance = qSqrt((2+redMean/256)*red*red + 4*green*green + (2+(255-redMean)/256)*blue*blue); //color perceive by human eyes
        if(colorDistance<=50){
            PixelHelper::setPixel(img, x, y, Qt::transparent);
            recursiveMagic(img, x+1, y, thisColor);
            recursiveMagic(img, x-1, y, thisColor);
            recursiveMagic(img, x, y-1, thisColor);
            recursiveMagic(img, x, y+1, thisColor);
        }
    }
}
