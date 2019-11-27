#include "TintFilter.h"

TintFilter::TintFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString TintFilter::getName() const
{
    return "Tint Filter";
}

QImage TintFilter::applyFilter(const QImage &image, double strength) const
{
    QImage newImage{image};
    // This filter will be dealing with integer strength values
    if (static_cast<int>(strength) == 0) {
        return newImage;
    }
    // Add integer strength to green
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRgb pixel = PixelHelper::getPixel(image, i, j);
            int newGreen = qBound(0, qGreen(pixel) + static_cast<int>(strength), 255);
            PixelHelper::setPixel(newImage, i, j, qRgb(qRed(pixel), newGreen, qBlue(pixel)));
        }
    }
    return newImage;
}

// No change
QImage TintFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}
