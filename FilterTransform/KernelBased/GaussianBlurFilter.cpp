#include "GaussianBlurFilter.h"

GaussianBlurFilter::GaussianBlurFilter(int size, double sd, QObject* parent) : AbstractKernelBasedImageFilterTransform(size, parent)
{
    setKernel(size, sd);
}

QImage GaussianBlurFilter::applyFilter(const QImage &image, int size, double strength)
{
    setSize(size);
    setKernel(size, strength);
    return convolution(image);
}

QImage GaussianBlurFilter::applyFilter(const QImage &image, int size)
{
    setSize(size);
    setKernel(size, 1.0);
    return applyFilter(image, size, 1.0);
}

QString GaussianBlurFilter::getName() const
{
    return "GaussianBlurFilter";
}

void GaussianBlurFilter::setKernel(int size, double sd)
{
    redefineKernel(size);
    const int MULTIPLIER = 1000;
    for (int dx = -size + 1; dx < size; ++dx)
        for (int dy = -size + 1; dy < size; ++dy) {
            int sqDist = dx * dx + dy * dy; // calculate squared distance to plug in the distribution formula
            double density = 1.0 / qSqrt(2.0 * M_PI * sd * sd) * qExp(-sqDist / 2.0 / sd / sd); // calculate density using gaussian distribution formula
            setEntry(dx, dy, density * MULTIPLIER); // multiply by a large constant to increase precision
    }
}
