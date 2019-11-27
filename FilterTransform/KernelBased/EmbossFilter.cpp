#include "EmbossFilter.h"

EmbossFilter::EmbossFilter(int size, QObject* parent) : AbstractKernelBasedImageFilterTransform(size, parent)
{
    setKernel(size);
}

QImage EmbossFilter::applyFilter(const QImage &image, int size, double)
{
    setSize(size);
    setKernel(size);
    return convolution(image);
}

QImage EmbossFilter::applyFilter(const QImage &image, int size)
{
    setSize(size);
    setKernel(size);
    return applyFilter(image, size, 1.0);
}

QString EmbossFilter::getName() const
{
    return "EmbossFilter";
}

void EmbossFilter::setKernel(int size, double)
{
    redefineKernel(size);
    for (int dx = -size + 1; dx < size; ++dx)
        for (int dy = -size + 1; dy < size; ++dy) {
            setEntry(dx, dy, dx+dy);                                     // dx+dy and distance to center
    }                                                                    // if size is 2, the kernel would be 3x3, from 2*size -1
    setEntry(0,0,1);                                                     // middle entry is 1
}
