/**
 * @class EdgeDetectionFilter
 * @brief Edge Detection Filter kernel implementation.
 */

#include "EdgeDetectionFilter.h"

/**
 * @brief Construct a new Edge Detection Filter:: Edge Detection Filter object
 * 
 * @param size Size/Radius of the kernel matrix.
 * @param parent Passed to AbstractKernelBasedImageFilterTransform() constructor.
 */
EdgeDetectionFilter::EdgeDetectionFilter(int size, QObject *parent) : AbstractKernelBasedImageFilterTransform(size, parent)
{
    setKernel(size);
}

/**
 * @brief Gets new image after filter applied.
 * 
 * @param image Original image to get new filter applied image.
 * @param size Size of the kernel.
 * @return QImage Filter applied image.
 */
QImage EdgeDetectionFilter::applyFilter(const QImage &image, int size, double)
{
    setSize(size);
    setKernel(size);
    return convolution(image);
}

/**
 * @brief This is an overloaded function.
 * 
 * @param image Original image to get new filter applied image.
 * @param size Size of the kernel.
 * @return QImage Filter applied image.
 */
QImage EdgeDetectionFilter::applyFilter(const QImage &image, int size)
{
    return applyFilter(image, size, 1.0);
}

/**
 * @brief Returns the name of the filter.
 * 
 * @return QString Name of the filter.
 */
QString EdgeDetectionFilter::getName() const
{
    return "Edge Detection Filter";
}

/**
 * @brief Construct an edge detection kernel with radius size.
 *
 * An identity kernel with a dimension ( size * 2 - 1) * ( size * 2 - 1) is constructed.
 * The center kernel pixel weight will be the total number of cells minus 1 with all other weights -1.
 *
 * Example of an edge detection kernel with size 2.
 *
 *     -1 -1 -1
 *     -1  8 -1
 *     -1 -1 -1
 *
 * @param size Size/radius of the kernel.
 */
void EdgeDetectionFilter::setKernel(int size, double)
{
    redefineKernel(size);
    for (int dx = -size + 1; dx < size; ++dx)
        for (int dy = -size + 1; dy < size; ++dy)
        {
            setEntry(dx, dy, -1);                    // margin to be much less significant
        }                                            // if size is 2, the kernel would be 3x3, from 2*size -1
    setEntry(0, 0, (2 * size - 1) * (2 * size - 1)); // middle entry is really significant
}
