#include "AbstractKernelBasedImageFilterTransform.h"

AbstractKernelBasedImageFilterTransform::AbstractKernelBasedImageFilterTransform(QObject *parent) : AbstractImageFilterTransform(parent)
{

}

QImage AbstractKernelBasedImageFilterTransform::applyFilter(const QImage &img, int size, double) const
{
    return applyFilter(img, size);
}
