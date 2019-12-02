/**
 * @class ClockwiseRotationTransform
 * @brief Clockwise Rotation Transformation Non-kernel implementation.
 */

#include "ClockwiseRotationTransform.h"
#include <QTransform>

/**
 * @brief Construct a new ClockwiseRotationTransform:: Clockwise Rotation object
 *
 * @param parent Passed to AbstractNonKernelBasedImageFilterTransform() constructor.
 */
ClockwiseRotationTransform::ClockwiseRotationTransform(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString ClockwiseRotationTransform::getName() const
{
    return "Clockwise Rotation";
}

/**
 * @brief This is an overloaded function.
 *
 * @return QImage exact copy of image.
 */
QImage ClockwiseRotationTransform::applyFilter(const QImage &image, double) const
{
    return applyFilter(image);
}

/**
 * @brief Gets new image after filter applied.
 *
 * @param image Original image to get new filter applied image.
 *
 * Construct newImage QImage from the existing image
 * Rotate newImage by 90 degrees with rotate and transformed
 *
 * @return QImage Filter applied image.
 */
QImage ClockwiseRotationTransform::applyFilter(const QImage &image) const
{
    QTransform cwRotation;
    cwRotation.rotate(90);
    QImage newImage{image};
    newImage = newImage.transformed(cwRotation);
    return newImage;
}
