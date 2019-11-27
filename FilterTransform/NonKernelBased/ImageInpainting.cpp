#include "ImageInpainting.h"

ImageInpainting::ImageInpainting(QObject *parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString ImageInpainting::getName() const
{
    return "ImageInpainting";
}

QImage ImageInpainting::applyFilter(const QImage &image, double) const
{
    return QImage(image);
}

// No change
QImage ImageInpainting::applyFilter(const QImage &image) const
{
    return QImage(image);
}
