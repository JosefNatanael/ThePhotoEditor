#include "MeanBlurFilter.h"

MeanBlurFilter::MeanBlurFilter(int size, QObject* parent) : AbstractKernelBasedImageFilterTransform(size, parent)
{
    setKernel(size);
}

QImage MeanBlurFilter::applyFilter(const QImage &image, int size, double)
{
    setSize(size);
    setKernel(size);
    return convolution(image);
}

QImage MeanBlurFilter::applyFilter(const QImage &image, int size)
{
    setSize(size);
    setKernel(size);
    return applyFilter(image, size, 1.0);
}

QString MeanBlurFilter::getName() const
{
    return "MeanBlurFilter";
}

void MeanBlurFilter::setKernel(int size, double)
{
    redefineKernel(size);
    for (int dx = -size + 1; dx < size; ++dx)
        for (int dy = -size + 1; dy < size; ++dy) {
            setEntry(dx, dy, 1.0 / (size + 1) * (size + 1));               // set the entry to be 1/(size+1)^2
    }                                                                    // if size is 2, the kernel would be 3x3, from 2*size -1
}
