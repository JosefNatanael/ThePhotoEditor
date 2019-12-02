/**
 * @class ContrastFilter
 * @brief Contrast Filter Non-Kernel Implementation.
 */

#include "ContrastFilter.h"

/**
 * @brief Construct a new Contrast Filter:: Contrast Filter object
 *
 * @param parent Passed to AbstractKernelBasedImageFilterTransform() constructor.
 */
ContrastFilter::ContrastFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString ContrastFilter::getName() const
{
    return "Contrast Filter";
}

/**
 * @brief Gets new image after filter applied.
 *
 * @param image Original image to get new filter applied image.
 * @param strength Strength of the contrast to be applied
 * @return QImage Filter applied image.
 */
QImage ContrastFilter::applyFilter(const QImage &image, double strength) const
{
    QImage newImage{image};
    // This filter will be dealing with integer strength values
    if (static_cast<int>(strength) == 0) {
        return newImage;
    }
    // 1. calculate the contrast correction factor
    // 2. perform the actual contrast adjustment with the formula newrgb = factor * (rgb - 128) + 128
    // 3. Bound the resulting rgb between 0 and 255
    double correctionFactor = (259 * (static_cast<int>(strength) + 255)) / (255 * (259 - static_cast<int>(strength)));
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRgb pixel = PixelHelper::getPixel(image, i, j);
            int newRed = qBound(0, static_cast<int>(correctionFactor * (qRed(pixel) - 128) + 128), 255);
            int newGreen = qBound(0, static_cast<int>(correctionFactor * (qGreen(pixel) - 128) + 128), 255);
            int newBlue = qBound(0, static_cast<int>(correctionFactor * (qBlue(pixel) - 128) + 128), 255);
            PixelHelper::setPixel(newImage, i, j, qRgb(newRed, newGreen, newBlue));
        }
    }
    return newImage;
}

/**
 * @brief This is an overloaded function.
 *
 * @return QImage exact copy of image.
 */
QImage ContrastFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}
