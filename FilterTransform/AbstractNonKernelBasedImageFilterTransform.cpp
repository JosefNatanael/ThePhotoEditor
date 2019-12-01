/**
 * @class AbstractNonKernelBasedImageFilterTransform
 * @brief Abstract base class implementation. Derived class needs to provide each of their applyFilter functions. 

 * 
 */
#include "AbstractNonKernelBasedImageFilterTransform.h"

/**
 * @brief Construct a new Abstract Non Kernel Based Image Filter Transform:: Abstract Non Kernel Based Image Filter Transform object
 * 
 * @param parent Passed to AbstractImageFilterTransform() constructor.
 */
AbstractNonKernelBasedImageFilterTransform::AbstractNonKernelBasedImageFilterTransform(QObject *parent) : AbstractImageFilterTransform(parent)
{

}

/**
 * @brief Virtual applyFilter class. Size is a stub.
 * @details This function will call the derived class applyFilter() function with arguments image and strength.
 * 
 * @param img Original image, basis of filter/transformation.
 * @param strength Strength of the filter/transform.
 * @return QImage Filtered/transformed image.
 */
QImage AbstractNonKernelBasedImageFilterTransform::applyFilter(const QImage &img, int, double strength)
{
    return applyFilter(img, strength);
}
