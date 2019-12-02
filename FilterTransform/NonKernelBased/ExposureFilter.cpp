/**
 * @class ExposureFilter
 * @brief Exposure Filter Non-Kernel Implementation.
 */
#include "ExposureFilter.h"
#include <QtMath>

/**
 * @brief Construct a new Exposure Filter:: Brightness Filter object
 *
 * @param parent Passed to AbstractNonKernelBasedImageFilterTransform() constructor.
 */
ExposureFilter::ExposureFilter(QObject* parent) : AbstractNonKernelBasedImageFilterTransform(parent)
{

}

/**
 * @brief Returns the name of the filter.
 *
 * @return QString Name of the filter.
 */
QString ExposureFilter::getName() const
{
    return "Exposure Filter";
}

/**
 * @brief Gets new image after filter applied.
 *
 * @param image Original image to get new filter applied image.
 * @param strength Strength of the exposure to be applied
 * @return QImage Filter applied image.
 */
QImage ExposureFilter::applyFilter(const QImage &image, double strength) const
{
    QImage newImage{image};
    // This filter will be dealing with integer strength values
    if (static_cast<int>(strength) == 0) {
        return newImage;
    }
    // 1. we turn from rgb to hsl
    // 2. modification of luminance, with formula newLight = oldLight * 2 ^ exposure compensation. Exposure compensation is simply strength/100
    // 3. we turn hsl back to rgb
    for (int i = 0; i < image.width(); ++i) {
        for (int j = 0; j < image.height(); ++j) {
            QRgb pixel = PixelHelper::getPixel(image, i, j);
            QColor pixelColor = QColor::fromRgb(pixel);
            double exposureCompensation = strength / 100;
            int hsvLightness = qBound(0, static_cast<int>(pixelColor.value() * qPow(2, exposureCompensation)), 255);
            pixelColor.setHsv(pixelColor.hsvHue(), pixelColor.hsvSaturation(), hsvLightness);
            PixelHelper::setPixel(newImage, i, j, pixelColor.rgba());
        }
    }
    return newImage;
}

/**
 * @brief This is an overloaded function.
 *
 * @return QImage exact copy of image.
 */
QImage ExposureFilter::applyFilter(const QImage &image) const
{
    QImage newImage{image};
    return newImage;
}
