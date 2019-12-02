/**
 * @class SaturationFilter
 * @brief Saturation Filter Non-Kernel Implementation.
 */
#include "SaturationFilter.h"

/**
 * @brief Construct a new Saturation Filter:: Saturation Filter object
 *
 * @param parent Passed to AbstractNonKernelBasedImageFilterTransform() constructor.
 */
SaturationFilter::SaturationFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString SaturationFilter::getName() const
{
    return "Saturation Filter";
}

/**
 * @brief Gets new image after filter applied.
 *
 * @param image Original image to get new filter applied image.
 * @param strength Strength of Saturation level to be applied
 *
 * Get qRgb of each pixel with PixelHelper::getPixel
 * Convert qRgb into QColor format
 * Change the Saturation Value for each pixel and set with setHsv
 * Use the value to set the new image with PixelHelper::setPixel
 *
 * @return QImage Filter applied image.
 */
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
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRgb pixel = PixelHelper::getPixel(image, i, j);
            QColor pixelColor = QColor::fromRgb(pixel);
            int hsvSaturation = qBound(0, pixelColor.hsvSaturation() + static_cast<int>(strength), 255);
            pixelColor.setHsv(pixelColor.hsvHue(), hsvSaturation, pixelColor.value());
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
QImage SaturationFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}
