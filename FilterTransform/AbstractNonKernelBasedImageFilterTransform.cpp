#include "AbstractNonKernelBasedImageFilterTransform.h"

AbstractNonKernelBasedImageFilterTransform::AbstractNonKernelBasedImageFilterTransform(QObject *parent) : AbstractImageFilterTransform(parent)
{

}

QImage AbstractNonKernelBasedImageFilterTransform::applyFilter(const QImage &img, int, double strength)
{
    return applyFilter(img, strength);
}
