/**
 * @class AbstractKernelBasedImageFilterTransform
 * @brief Kernel based image filter/transform.
 * @details Image filter/transform uses convolution matrix/kernel.
 * 
 */
#include "AbstractKernelBasedImageFilterTransform.h"

/**
 * @brief Construct a new Abstract Kernel Based Image Filter Transform:: Abstract Kernel Based Image Filter Transform object.
 * @details Also constructs an "identity kernel", i.e. the center of the matrix is 1, and the rest is 0. For example of a size 2 kernel.
 *     0 0 0
 *     0 1 0
 *     0 0 0
 * 
 * @param size Sets the size of the class to this size argument. Size refers to the radius of the matrix. E.g. size 3 = 3*2-1 = 5. A 5x5 matrix.
 * @param parent Passed to AbstractImageFilterTransform() constructor.
 */
AbstractKernelBasedImageFilterTransform::AbstractKernelBasedImageFilterTransform(int size, QObject* parent)
    : AbstractImageFilterTransform(parent)
    , size(size)
    , kernel(size * 2 - 1, QVector<double>(size * 2 - 1, 0))
{
    setEntry(0, 0, 1);
}

/**
 * @brief Applies filter/transform to img. 
 * @details Here double is unused/dummy. An implementation of applyFilter will be given by child class.
 * 
 * @param img Image to be transformed/filtered.
 * @param size Kernel size.
 * @return QImage Filtered/Transformed image.
 */
QImage AbstractKernelBasedImageFilterTransform::applyFilter(const QImage &img, int size, double)
{
    return applyFilter(img, size);
}

/**
 * @brief Convolve img with kernel.
 * 
 * @param img Image to convolve.
 * @return QImage Convolved image.
 */
QImage AbstractKernelBasedImageFilterTransform::convolution(const QImage &img) const
{
    QImage newImage{img}; // create new image
    int normalizeFactor = 0;                    //normalize the kernel
    for(int dx = -size +1 ; dx<size; ++dx){
        for(int dy = -size + 1; dy<size; ++dy){
            normalizeFactor += getEntry(dx, dy);
        }
    }

    for (int i = 0; i < img.width(); ++i) {
        for (int j = 0; j < img.height(); ++j) {
            QRgb color = qRgb(0, 0, 0);//initialize to black

            int rTotal = 0, gTotal = 0, bTotal = 0;
            for(int dx = -size + 1; dx < size; ++dx) {
                for(int dy = -size + 1; dy < size; ++dy) {
                    int X = i + dx, Y = j + dy;
                    if((0 <= X && X < img.width()) && (0 <= Y && Y < img.height())) {
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

/**
 * @brief Gets kernel entry at position x, y.
 * @details 0, 0 is the center of the matrix.
 * 
 * @param x 0 is the center of the matrix in x direction.
 * @param y 0 is the center of the matrix in y direction.
 * @return double Value of the kernel at entry x, y.
 */
double AbstractKernelBasedImageFilterTransform::getEntry(int x, int y) const
{
    return kernel[x + size - 1][y + size - 1]; // add back the offset for storage or access
}

/**
 * @brief Sets kernel entry at position x, y.
 * @details 0, 0 is the center of the matrix.
 * 
 * @param x 0 is the center of the matrix in x direction.
 * @param y 0 is the center of the matrix in y direction.
 * @param value Value to set to entry x, y.
 */
void AbstractKernelBasedImageFilterTransform::setEntry(int x, int y, double value)
{
    kernel[x + size - 1][y + size - 1] = value;
}

/**
 * @brief Sets the size of the kernel. Size is the radius of the matrix.
 * 
 * @param newSize 
 */
void AbstractKernelBasedImageFilterTransform::setSize(int newSize)
{
    size = newSize;
}

/**
 * @brief Recreate the kernel with specified size.
 * 
 * @param size Size/radius of the new kernel.
 */
void AbstractKernelBasedImageFilterTransform::redefineKernel(int size)
{
    QVector<QVector<double>> newVector{size * 2 - 1, QVector<double>(size * 2 - 1, 0)};
    kernel = newVector;
}
