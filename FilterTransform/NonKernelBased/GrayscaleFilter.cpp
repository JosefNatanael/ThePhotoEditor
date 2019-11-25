#include "GrayscaleFilter.h"

GrayscaleFilter::GrayscaleFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

QString GrayscaleFilter::getName() const
{
    return "GrayscaleFilter";
}

QImage GrayscaleFilter::applyFilter(const QImage &image, double) const
{
    return applyFilter(image);
}

QImage GrayscaleFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRgb pixel = PixelHelper::getPixel(image, i, j);
            int avg = (qRed(pixel) + qGreen(pixel) + qBlue(pixel)) / 3;
            PixelHelper::setPixel(newImage, i, j, qRgb(avg, avg, avg));
        }
    }
    return newImage;
}
