#include "InvertFilter.h"

InvertFilter::InvertFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString InvertFilter::getName() const
{
    return "InvertFilter";
}

QImage InvertFilter::applyFilter(const QImage &image, double) const
{
    return applyFilter(image);
}

QImage InvertFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    newImage.invertPixels();
    return newImage;
}
