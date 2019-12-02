/**
 * @class GrayscaleFilter
 * @brief Grayscale Filter Non-Kernel Implementation.
 */
#include "GrayscaleFilter.h"

/**
 * @brief Construct a new Grayscale Filter:: Grayscale Filter object
 *
 * @param parent Passed to AbstractNonKernelBasedImageFilterTransform() constructor.
 */
GrayscaleFilter::GrayscaleFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString GrayscaleFilter::getName() const
{
    return "Grayscale Filter";
}

/**
 * @brief This is an overloaded function.
 *
 * @param image Original image to get new filter applied image.
 * @return QImage Filter applied image.
 */
QImage GrayscaleFilter::applyFilter(const QImage &image, double) const
{
    return applyFilter(image);
}

/**
 * @brief Gets new image after filter applied.
 *
 * @param image Original image to get new filter applied image.
 *
 * Get qRgb of each pixel with PixelHelper::getPixel
 * Average R, G, and B of each pixel
 * Use the value to set the new image with PixelHelper::setPixel
 *
 * @return QImage Filter applied image.
 */
QImage GrayscaleFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRgb pixel = PixelHelper::getPixel(image, i, j);
            int avg = (qRed(pixel) + qGreen(pixel) + qBlue(pixel)) / 3;
            PixelHelper::setPixel(newImage, i, j, qRgb(avg, avg, avg));
        }
    }
    return newImage;
}
