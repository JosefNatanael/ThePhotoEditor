#include "SaturationFilter.h"

SaturationFilter::SaturationFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString SaturationFilter::getName() const
{
    return "SaturationFilter";
}

QImage SaturationFilter::applyFilter(const QImage &image, double strength) const
{
    QImage newImage{image};
    // This filter will be dealing with integer strength values
    if (static_cast<int>(strength) == 0) {
        return newImage;
    }
    // 1. we turn from rgb to saturation
    // 2. simple hsv filter, adds the strength directly to the saturation value, but with upper and lower bounds
    // 3. we turn saturation back to rgb
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            QRgb pixel = PixelHelper::getPixel(image, i, j);
            QColor pixelColor = QColor::fromRgb(pixel);
            int hsvSaturation = qBound(0, pixelColor.hsvSaturation() + static_cast<int>(strength), 255);
            pixelColor.setHsv(pixelColor.hsvHue(), hsvSaturation, pixelColor.value());
            PixelHelper::setPixel(newImage, i, j, pixelColor.rgba());
        }
    }
    return newImage;
}

// No change
QImage SaturationFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}
