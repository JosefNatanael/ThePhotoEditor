/**
 * @class TintFilter
 * @brief Tint Filter Non-Kernel Implementation.
 */
#include "TintFilter.h"

/**
 * @brief Construct a new Tint Filter:: Tint Filter object
 *
 * @param parent Passed to AbstractNonKernelBasedImageFilterTransform() constructor.
 */
TintFilter::TintFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString TintFilter::getName() const
{
    return "Tint Filter";
}

/**
 * @brief Gets new image after filter applied.
 *
 * @param image Original image to get new filter applied image.
 * @param strength Strength of Saturation level to be applied
 *
 * Get qRgb of each pixel with PixelHelper::getPixel
 * Keep the R value
 * Increase the G value with the strength value
 * Keep the B value
 * Use the value to set the new image with PixelHelper::setPixel
 *
 * @return QImage Filter applied image.
 */
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

/**
 * @brief This is an overloaded function.
 *
 * @return QImage exact copy of image.
 */
QImage TintFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}
