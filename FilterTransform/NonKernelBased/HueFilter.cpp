#include "HueFilter.h"

HueFilter::HueFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString HueFilter::getName() const
{
    return "HueFilter";
}

QImage HueFilter::applyFilter(const QImage &image, double strength) const
{
    QImage newImage{image};
    // This filter will be dealing with integer strength values
    if (static_cast<int>(strength) == 0) {
        return newImage;
    }
    // 1. we turn from rgb to hue
    // 2. simple hsv filter, adds the strength directly to the hue value, but with upper and lower bounds
    // 3. we turn hue back to rgb
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            QRgb pixel = PixelHelper::getPixel(image, i, j);
            QColor pixelColor = QColor::fromRgb(pixel);
            int hsvHue = qBound(0, pixelColor.hsvHue() + static_cast<int>(strength), 360);
            pixelColor.setHsv(hsvHue, pixelColor.hsvSaturation(), pixelColor.value());
            PixelHelper::setPixel(newImage, i, j, pixelColor.rgba());
        }
    }
    return newImage;
}

// No change
QImage HueFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}
