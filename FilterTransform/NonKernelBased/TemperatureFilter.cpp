#include "TemperatureFilter.h"

TemperatureFilter::TemperatureFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString TemperatureFilter::getName() const
{
    return "Temperature Filter";
}

QImage TemperatureFilter::applyFilter(const QImage &image, double strength) const
{
    QImage newImage{image};
    // This filter will be dealing with integer strength values
    if (static_cast<int>(strength) == 0) {
        return newImage;
    }
    // Add integer strength to red and blue
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRgb pixel = PixelHelper::getPixel(image, i, j);
            int newRed = qBound(0, qRed(pixel) + static_cast<int>(strength), 255);
            int newBlue = qBound(0, qBlue(pixel) - static_cast<int>(strength), 255);
            PixelHelper::setPixel(newImage, i, j, qRgb(newRed, qGreen(pixel), newBlue));
        }
    }
    return newImage;
}

// No change
QImage TemperatureFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}
