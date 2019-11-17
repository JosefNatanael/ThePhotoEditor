#include "AbstractNonKernelBasedImageFilterTransform.h"

AbstractNonKernelBasedImageFilterTransform::AbstractNonKernelBasedImageFilterTransform(QObject *parent) : AbstractImageFilterTransform(parent)
{

}

QImage AbstractNonKernelBasedImageFilterTransform::applyFilter(const QImage &img, int, double strength) const
{
    return applyFilter(img, strength);
}
