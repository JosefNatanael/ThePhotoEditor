/**
 * @class TemperatureFilter
 * @brief Saturation Filter Non-Kernel Implementation.
 */
#include "TemperatureFilter.h"

/**
 * @brief Construct a new Temperature Filter:: Temperature Filter object
 *
 * @param parent Passed to AbstractNonKernelBasedImageFilterTransform() constructor.
 */
TemperatureFilter::TemperatureFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString TemperatureFilter::getName() const
{
    return "Temperature Filter";
}

/**
 * @brief Gets new image after filter applied.
 *
 * @param image Original image to get new filter applied image.
 * @param strength Strength of Saturation level to be applied
 *
 * Get qRgb of each pixel with PixelHelper::getPixel
 * Increase the R value with the strength value
 * Keep the G value
 * Decrease the B value with the strength value
 * Use the value to set the new image with PixelHelper::setPixel
 *
 * @return QImage Filter applied image.
 */
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

/**
 * @brief This is an overloaded function.
 *
 * @return QImage exact copy of image.
 */
QImage TemperatureFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}
