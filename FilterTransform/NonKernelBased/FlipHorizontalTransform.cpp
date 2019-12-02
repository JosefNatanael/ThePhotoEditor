/**
 * @class FlipHorizontalTransform
 * @brief Horizontal Flip Transformation Non-kernel implementation.
 */
#include "FlipHorizontalTransform.h"

/**
 * @brief Construct a new Flip Horizontal Transform:: Flip Horizontal Transform object
 *
 * @param parent Passed to AbstractNonKernelBasedImageFilterTransform() constructor.
 */
FlipHorizontalTransform::FlipHorizontalTransform(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString FlipHorizontalTransform::getName() const
{
    return "Flip Horizontal";
}

/**
 * @brief This is an overloaded function.
 *
 * @return QImage exact copy of image.
 */
QImage FlipHorizontalTransform::applyFilter(const QImage &image, double) const
{
    return applyFilter(image);
}

/**
 * @brief Gets new image after filter applied.
 *
 * @param image Original image to get new filter applied image.
 *
 * Construct newImage QImage from the existing image
 * Flip newImage with mirrored(bool horizontal, bool vertical)
 * horizontal is true and vertical is false in this case
 *
 * @return QImage Filter applied image.
 */
QImage FlipHorizontalTransform::applyFilter(const QImage &image) const
{
    return image.mirrored(true, false);
}
