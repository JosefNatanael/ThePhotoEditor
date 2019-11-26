#include "AbstractKernelBasedImageFilterTransform.h"

AbstractKernelBasedImageFilterTransform::AbstractKernelBasedImageFilterTransform(int size, QObject* parent)
    : AbstractImageFilterTransform(parent)
    , size(size)
    , kernel(size * 2 - 1, QVector<double>(size * 2 - 1, 0))
{
    setEntry(0, 0, 1);
}

QImage AbstractKernelBasedImageFilterTransform::applyFilter(const QImage &img, int size, double)
{
    return applyFilter(img, size);
}

QImage AbstractKernelBasedImageFilterTransform::convolution(const QImage &img) const
{
    QImage newImage{img}; // create new image
    for (int i = 0; i < img.width(); ++i) {
        for (int j = 0; j < img.height(); ++j) {
            QRgb color = qRgb(0, 0, 0);//initialize to black
            int normalizeFactor = 0;
            int rTotal = 0, gTotal = 0, bTotal = 0;
            for(int dx = -size + 1; dx < size; ++dx) {
                for(int dy = -size + 1; dy < size; ++dy) {
                    int X = i + dx, Y = j + dy;
                    if((0 < X && X < img.width()) && (0 < Y && Y < img.height())) {
                        normalizeFactor += getEntry(dx, dy);
                        QRgb pixel = PixelHelper::getPixel(img, X,Y);
                        rTotal += getEntry(dx, dy) * qRed(pixel);
                        gTotal += getEntry(dx, dy) * qGreen(pixel);
                        bTotal += getEntry(dx, dy) * qBlue(pixel);
                    }
                }
            }
            rTotal /= normalizeFactor;
            gTotal /= normalizeFactor;
            bTotal /= normalizeFactor;

            rTotal = qBound(0, rTotal, 255);
            gTotal = qBound(0, gTotal, 255);
            bTotal = qBound(0, bTotal, 255);
            color = qRgb(rTotal, gTotal, bTotal);

            PixelHelper::setPixel(newImage, i, j, color);
        }
    }
    return newImage;
}

double AbstractKernelBasedImageFilterTransform::getEntry(int x, int y) const
{
    return kernel[x + size - 1][y + size - 1]; // add back the offset for storage or access
}

void AbstractKernelBasedImageFilterTransform::setEntry(int x, int y, double value)
{
    kernel[x + size - 1][y + size - 1] = value;
}

void AbstractKernelBasedImageFilterTransform::setSize(int newSize)
{
    size = newSize;
}

void AbstractKernelBasedImageFilterTransform::redefineKernel(int size)
{
    QVector<QVector<double>> newVector{size * 2 - 1, QVector<double>(size * 2 - 1, 0)};
    kernel = newVector;
}
