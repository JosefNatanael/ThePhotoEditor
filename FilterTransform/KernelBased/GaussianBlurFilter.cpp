#include "GaussianBlurFilter.h"

GaussianBlurFilter::GaussianBlurFilter(QObject* parent) : AbstractKernelBasedImageFilterTransform(parent)
{

}

QImage GaussianBlurFilter::applyFilter(const QImage &image, int size, double strength) const
{
    // TODO
    return image;
}

QString GaussianBlurFilter::getName() const
{
    return "GaussianBlurFilter";
}
