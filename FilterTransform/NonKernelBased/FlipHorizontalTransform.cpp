#include "FlipHorizontalTransform.h"

FlipHorizontalTransform::FlipHorizontalTransform(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString FlipHorizontalTransform::getName() const
{
    return "FlipHorizontalTransform";
}

QImage FlipHorizontalTransform::applyFilter(const QImage &image, double) const
{
    return applyFilter(image);
}

QImage FlipHorizontalTransform::applyFilter(const QImage &image) const
{
    return image.mirrored(true, false);
}
