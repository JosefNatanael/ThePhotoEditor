/**
 * @class HueFilter
 * @brief Hue Filter Non-Kernel Implementation.
 */
#include "HueFilter.h"

/**
 * @brief Construct a new Hue Filter:: Hue Filter object
 *
 * @param parent Passed to AbstractNonKernelBasedImageFilterTransform() constructor.
 */
HueFilter::HueFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString HueFilter::getName() const
{
    return "Hue Filter";
}

/**
 * @brief Gets new image after filter applied.
 *
 * @param image Original image to get new filter applied image.
 * @param strength Strength of Hue level to be applied
 *
 * Get qRgb of each pixel with PixelHelper::getPixel
 * Convert qRgb into QColor format
 * Change the Hue Value for each pixel and set with setHsv
 * Use the value to set the new image with PixelHelper::setPixel
 *
 * @return QImage Filter applied image.
 */
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
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRgb pixel = PixelHelper::getPixel(image, i, j);
            QColor pixelColor = QColor::fromRgb(pixel);
            int hsvHue = qBound(0, pixelColor.hsvHue() + static_cast<int>(strength), 359);
            pixelColor.setHsv(hsvHue, pixelColor.hsvSaturation(), pixelColor.value());
            PixelHelper::setPixel(newImage, i, j, pixelColor.rgba());
        }
    }
    return newImage;
}

/**
 * @brief This is an overloaded function.
 *
 * @return QImage exact copy of image.
 */
QImage HueFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}
