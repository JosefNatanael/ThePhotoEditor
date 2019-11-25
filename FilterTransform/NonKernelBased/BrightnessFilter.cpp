#include "BrightnessFilter.h"

BrightnessFilter::BrightnessFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString BrightnessFilter::getName() const
{
    return "BrightnessFilter";
}

QImage BrightnessFilter::applyFilter(const QImage &image, double strength) const
{
    QImage newImage{image};
    // This filter will be dealing with integer strength values
    if (static_cast<int>(strength) == 0) {
        return newImage;
    }
    // 1. we turn from rgb to hsl
    // 2. linear modification of luminance
    // 3. we turn hsl back to rgb
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRgb pixel = PixelHelper::getPixel(image, i, j);
            QColor pixelColor = QColor::fromRgb(pixel);
            int hsvLightness = qBound(0, pixelColor.value() + static_cast<int>(strength), 255);
            pixelColor.setHsv(pixelColor.hsvHue(), pixelColor.hsvSaturation(), hsvLightness);
            PixelHelper::setPixel(newImage, i, j, pixelColor.rgba());
        }
    }
    return newImage;
}

// No change
QImage BrightnessFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}
