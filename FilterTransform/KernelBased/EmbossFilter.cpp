/**
 * @class EmbossFilter
 * @brief Emboss Filter kernel implementation.
 */
#include "EmbossFilter.h"

/**
 * @brief Construct a new Emboss Filter:: Emboss Filter object
 * 
 * @param size Size/Radius of the kernel matrix.
 * @param parent Passed to AbstractKernelBasedImageFilterTransform() constructor.
 */
EmbossFilter::EmbossFilter(int size, QObject *parent) : AbstractKernelBasedImageFilterTransform(size, parent)
{
    setKernel(size);
}

/**
 * @brief Gets new image after filter applied.
 * 
 * @param image Original image to get new filter applied image.
 * @param size Size/radius of the kernel.
 * @return QImage Filter applied image.
 */
QImage EmbossFilter::applyFilter(const QImage &image, int size, double)
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
QImage EmbossFilter::applyFilter(const QImage &image, int size)
{
    return applyFilter(image, size, 1.0);
}

/**
 * @brief Returns the name of the filter.
 * 
 * @return QString Name of the filter.
 */
QString EmbossFilter::getName() const
{
    return "Emboss Filter";
}

/**
 * @brief Construct an emboss kernel with radius size.
 *
 * An identity kernel with a dimension ( size * 2 - 1) * ( size * 2 - 1) is constructed.
 * The center kernel pixel weight will be the total number of cells minus 1 with all other weights the distance to the center.
 *
 * Example of an emboss kernel with size 2.
 *
 *     -2 -1  0
 *     -1  1  1
 *      0  1  2
 *
 * @param size Size/radius of the kernel.
 */
void EmbossFilter::setKernel(int size, double)
{
    redefineKernel(size);
    for (int dx = -size + 1; dx < size; ++dx)
        for (int dy = -size + 1; dy < size; ++dy)
        {
            setEntry(dx, dy, dx + dy); // dx+dy and distance to center
        }                              // if size is 2, the kernel would be 3x3, from 2*size -1
    setEntry(0, 0, 1);                 // middle entry is 1
}
