#include "CounterClockwiseRotationTransform.h"

CounterClockwiseRotationTransform::CounterClockwiseRotationTransform(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString CounterClockwiseRotationTransform::getName() const
{
    return "CounterClockwiseRotationTransform";
}

QImage CounterClockwiseRotationTransform::applyFilter(const QImage &image, double) const
{
    return applyFilter(image);
}

QImage CounterClockwiseRotationTransform::applyFilter(const QImage &image) const
{
    QTransform cwRotation;
    cwRotation.rotate(-90);
    QImage newImage{image};
    newImage = newImage.transformed(cwRotation);
    return newImage;
}
