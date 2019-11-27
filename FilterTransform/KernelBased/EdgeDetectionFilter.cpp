#include "EdgeDetectionFilter.h"

EdgeDetectionFilter::EdgeDetectionFilter(int size, QObject* parent) : AbstractKernelBasedImageFilterTransform(size, parent)
{
    setKernel(size);
}

QImage EdgeDetectionFilter::applyFilter(const QImage &image, int size, double)
{
    setSize(size);
    setKernel(size);
    return convolution(image);
}

QImage EdgeDetectionFilter::applyFilter(const QImage &image, int size)
{
    setSize(size);
    setKernel(size);
    return applyFilter(image, size, 1.0);
}

QString EdgeDetectionFilter::getName() const
{
    return "Edge Detection Filter";
}

void EdgeDetectionFilter::setKernel(int size, double)
{
    redefineKernel(size);
    for (int dx = -size + 1; dx < size; ++dx)
        for (int dy = -size + 1; dy < size; ++dy) {
            setEntry(dx, dy, -1);                                        // margin to be much less significant
    }                                                                    // if size is 2, the kernel would be 3x3, from 2*size -1
    setEntry(0,0,(2*size - 1)*(2*size - 1));                             // middle entry is really significant
}
