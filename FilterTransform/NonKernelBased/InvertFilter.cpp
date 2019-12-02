/**
 * @class InvertFilter
 * @brief Invert Filter Non-Kernel Implementation.
 */
#include "InvertFilter.h"

/**
 * @brief Construct a new Invert Filter:: Invert Filter object
 *
 * @param parent Passed to AbstractNonKernelBasedImageFilterTransform() constructor.
 */
InvertFilter::InvertFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString InvertFilter::getName() const
{
    return "Invert Filter";
}

/**
 * @brief This is an overloaded function.
 *
 * @return QImage exact copy of image.
 */
QImage InvertFilter::applyFilter(const QImage &image, double) const
{
    return applyFilter(image);
}

/**
 * @brief Gets new image after filter applied.
 *
 * @param image Original image to get new filter applied image.
 *
 * Construct newImage QImage from the existing image
 * Invert newImage with invertPixels
 *
 * @return QImage Filter applied image.
 */
QImage InvertFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    newImage.invertPixels();
    return newImage;
}
