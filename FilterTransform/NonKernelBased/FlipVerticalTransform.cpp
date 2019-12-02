/**
 * @class FlipVerticalTransform
 * @brief Vertical Flip Transformation Non-kernel implementation.
 */
#include "FlipVerticalTransform.h"

/**
 * @brief Construct a new Flip Vertical Transform:: Flip Vertical Transform object
 *
 * @param parent Passed to AbstractNonKernelBasedImageFilterTransform() constructor.
 */
FlipVerticalTransform::FlipVerticalTransform(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString FlipVerticalTransform::getName() const
{
    return "Flip Vertical";
}

/**
 * @brief This is an overloaded function.
 *
 * @return QImage exact copy of image.
 */
QImage FlipVerticalTransform::applyFilter(const QImage &image, double) const
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
 * horizontal is false and vertical is true in this case
 *
 * @return QImage Filter applied image.
 */
QImage FlipVerticalTransform::applyFilter(const QImage &image) const
{
    return image.mirrored(false, true);
}
