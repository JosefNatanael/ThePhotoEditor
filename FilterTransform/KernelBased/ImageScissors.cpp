/**
 * @class ImageScissors
 * @brief Image Scissors Filter kernel implementation.
 */
#include "ImageScissors.h"

/**
 * @brief Construct a new Image Scissors:: Image Scissors object
 * 
 * @param size Size/Radius of the kernel matrix.
 * @param parent Passed to AbstractKernelBasedImageFilterTransform() constructor.
 */
ImageScissors::ImageScissors(int size, QObject *parent) : AbstractKernelBasedImageFilterTransform(size, parent), size(size)
{
}

/**
 * @brief Gets new image after effect applied.
 * 
 * @param image Original image to get new filter applied image.
 * @param size Size/radius of the kernel.
 * @return QImage Filter applied image.
 */
QImage ImageScissors::applyFilter(const QImage &image, int size, double)
{
    setSize(size);
    return convolution(image);
}

/**
 * @brief This is an overloaded function.
 * 
 * @param image Original image to get new filter applied image.
 * @param size Size/radius of the kernel.
 * @return QImage Filter applied image.
 */
QImage ImageScissors::applyFilter(const QImage &image, int size)
{
    return applyFilter(image, size, 1.0);
}

/**
 * @brief Returns the name of the filter.
 * 
 * @return QString Name of the filter.
 */
QString ImageScissors::getName() const
{
    return "Image Scissors";
}

/**
 * @brief Construct an image inpainting kernel with radius size.
 *
 * An identity kernel with a dimension ( size * 2 - 1) * ( size * 2 - 1) is constructed.
 * The weight of each entry in the kernel is zero except the center, which is one.
 * There is no need to implement this, since by default the constructor has generated such a kernel.
 *
 * @param size Size/radius of the kernel.
 */
void ImageScissors::setKernel(int, double)
{
}

/**
 * @brief Overriden inpainting convolution algorithm.
 * 
 * @param img Image to convolve.
 * @return QImage Convolved image.
 */
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

/**
 * @brief Returns image mask.
 * 
 * @return QImage 
 */
QImage ImageScissors::getMask() {
    return mask;
}

/**
 * @brief Sets image mask.
 * 
 * @param mask New image to set as image scissors mask.
 */
void ImageScissors::setMask(const QImage &mask) {
    this->mask = mask;
}
