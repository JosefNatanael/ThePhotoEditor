#include "ClockwiseRotationTransform.h"
#include <QTransform>

ClockwiseRotationTransform::ClockwiseRotationTransform(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString ClockwiseRotationTransform::getName() const
{
    return "Clockwise Rotation";
}

QImage ClockwiseRotationTransform::applyFilter(const QImage &image, double) const
{
    return applyFilter(image);
}

QImage ClockwiseRotationTransform::applyFilter(const QImage &image) const
{
    QTransform cwRotation;
    cwRotation.rotate(90);
    QImage newImage{image};
    newImage = newImage.transformed(cwRotation);
    return newImage;
}
