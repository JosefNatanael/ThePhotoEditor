#include "FlipVerticalTransform.h"


FlipVerticalTransform::FlipVerticalTransform(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString FlipVerticalTransform::getName() const
{
    return "Flip Vertical";
}

QImage FlipVerticalTransform::applyFilter(const QImage &image, double) const
{
    return applyFilter(image);
}

QImage FlipVerticalTransform::applyFilter(const QImage &image) const
{
    return image.mirrored(false, true);
}
