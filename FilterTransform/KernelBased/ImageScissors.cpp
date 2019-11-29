#include "ImageScissors.h"
#include <QDebug>
ImageScissors::ImageScissors(int size, QObject *parent) : AbstractKernelBasedImageFilterTransform(size, parent), size(size)
{
    setKernel(size);
}

QImage ImageScissors::applyFilter(const QImage &image, int size, double)
{
    setSize(size);
    setKernel(size);
    return convolution(image);
}

QImage ImageScissors::applyFilter(const QImage &image, int size)
{
    return applyFilter(image, size, 1.0);
}

QString ImageScissors::getName() const
{
    return "Image Scissors";
}

void ImageScissors::setKernel(int size, double)
{
    redefineKernel(size);
    for (int dx = -size + 1; dx < size; ++dx)
        for (int dy = -size + 1; dy < size; ++dy)
        {
            setEntry(dx, dy, 0);
        }              // if size is 2, the kernel would be 3x3, from 2*size -1
    setEntry(0, 0, 1); // middle entry is 1, identity kernel
}

QImage ImageScissors::convolution(const QImage &img) const
{
    QImage newImage{img};    // create new image

    for (int i = 0; i < img.width(); ++i)
    {
        for (int j = 0; j < img.height(); ++j)
        {
          if(PixelHelper::getPixel(mask, i, j) != qRgb(255,255,255)){   //no kernel used, since identity.
                PixelHelper::setPixel(newImage, i, j, Qt::white);
            }
        }
    }
    return newImage;
}
