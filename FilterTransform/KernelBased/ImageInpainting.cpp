/**
 * @class ImageInpainting
 * @brief Image Inpainting Filter kernel implementation.
 */
#include "ImageInpainting.h"
#include <QDebug>

/**
 * @brief Construct a new Image Inpainting:: Image Inpainting object
 * 
 * @param size Size/Radius of the kernel matrix.
 * @param parent Passed to AbstractKernelBasedImageFilterTransform() constructor.
 */
ImageInpainting::ImageInpainting(int size, QObject *parent) : AbstractKernelBasedImageFilterTransform(size, parent), size(size)
{
    setKernel(size);
}

/**
 * @brief Gets new image after effect applied.
 * 
 * @param image Original image to get new filter applied image.
 * @param size Size/radius of the kernel.
 * @return QImage Filter applied image.
 */
QImage ImageInpainting::applyFilter(const QImage &image, int size, double)
{
    setSize(size);
    setKernel(size);
    return convolution(image);
}

/**
 * @brief This is an overloaded function.
 * 
 * @param image Original image to get new filter applied image.
 * @param size Size/radius of the kernel.
 * @return QImage Filter applied image.
 */
QImage ImageInpainting::applyFilter(const QImage &image, int size)
{
    return applyFilter(image, size, 1.0);
}

/**
 * @brief Returns the name of the filter.
 * 
 * @return QString Name of the filter.
 */
QString ImageInpainting::getName() const
{
    return "Image Inpainting";
}

/**
 * @brief Construct an image inpainting kernel with radius size.
 *
 * An identity kernel with a dimension ( size * 2 - 1) * ( size * 2 - 1) is constructed.
 * The weight of each entry in the kernel is generated by the (2 * size - 1) * (2 * size - 1) - 1.
 *
 * @param size Size/radius of the kernel.
 */
void ImageInpainting::setKernel(int size, double)
{
    const int MULTIPLIER = 1000;
    redefineKernel(size);
    for (int dx = -size + 1; dx < size; ++dx)
        for (int dy = -size + 1; dy < size; ++dy)
        {
//            setEntry(dx, dy, (2 * size - 1) * (2 * size - 1) - 1);
            if((dx+dy)%2 == 0){setEntry(dx, dy, 0.073235*MULTIPLIER);}
            else {setEntry(dx, dy, 0.176765*MULTIPLIER);}
        }              // if size is 2, the kernel would be 3x3, from 2*size -1
    setEntry(0, 0, 0); // middle entry is 0
}

/**
 * @brief Overriden inpainting convolution algorithm.
 * 
 * @param img Image to convolve.
 * @return QImage Convolved image.
 */
QImage ImageInpainting::convolution(const QImage &img) const
{
    QImage newImage{img};    // create new image
    int normalizeFactor = 0; //normalize the kernel
    const int MAXNUMREPEAT = 80;
    int widthThreshold = img.width() > mask.width() ? mask.width() : img.width();
    int heightThreshold = img.height() > mask.height() ? mask.height() : img.height();
    for (int dx = -size + 1; dx < size; ++dx)
    {
        for (int dy = -size + 1; dy < size; ++dy)
        {
            normalizeFactor += getEntry(dx, dy);
        }
    }

    //fill in missing region with input's average color
    long long avgRed = 0, avgGreen = 0, avgBlue = 0;
    for(int i = 0; i<widthThreshold; ++i){
        for(int j = 0; j<heightThreshold; ++j){
            QRgb pixel = PixelHelper::getPixel(img, i, j);
            avgRed += qRed(pixel);
            avgGreen += qGreen(pixel);
            avgBlue += qBlue(pixel);
            }
        }
    avgRed /= widthThreshold * heightThreshold;
    avgGreen /= widthThreshold * heightThreshold;
    avgBlue /= widthThreshold * heightThreshold;

    qDebug()<<avgRed<<" "<<avgGreen<<" "<<avgBlue;
    qDebug()<<img.width()<<" "<<img.height();

    //actual formula: (1 - mask/255) * image + (mask/255) * average
    for(int i = 0; i<widthThreshold; ++i){
        for(int j = 0; j<heightThreshold; ++j){
                QRgb maskPixel = PixelHelper::getPixel(mask, i, j);
                //remove the white part in mask
                if(maskPixel != qRgb(0,0,0)){
                    int thisRed =  (qRed(maskPixel)/255.0) * avgRed;
                    int thisGreen = (qGreen(maskPixel)/255.0) * avgGreen;
                    int thisBlue =  (qBlue(maskPixel)/255.0) * avgBlue;
                    thisRed = qBound(0, thisRed, 255);
                    thisGreen = qBound(0, thisGreen, 255);
                    thisBlue = qBound(0, thisBlue, 255);
                    PixelHelper::setPixel(newImage, i, j, qRgb(thisRed, thisGreen, thisBlue));
                }
            }
        }

    //implement repetition for convolution with kernel
    for(int k = 0; k<MAXNUMREPEAT; ++k){
        for (int i = 0; i < widthThreshold; ++i)
        {
            for (int j = 0; j <heightThreshold; ++j)
            {
                if (PixelHelper::getPixel(mask, i, j) != qRgb(0, 0, 0)) //assume mask has same size as img and monochrome.
                {                                                       //perform operation only for non-black parts of mask
                    QRgb color = qRgb(0, 0, 0); //initialize to black
                    int rTotal = 0, gTotal = 0, bTotal = 0;
                    for (int dx = -size + 1; dx < size; ++dx)
                    {
                        for (int dy = -size + 1; dy < size; ++dy)
                        {
                            int X = i + dx, Y = j + dy;
                            if ((0 < X && X < img.width()) && (0 < Y && Y < img.height()))
                            {
                                QRgb pixel = PixelHelper::getPixel(newImage, X, Y);
                                if(pixel != qRgb(255,255,255)){
                                rTotal += getEntry(dx, dy) * qRed(pixel);
                                gTotal += getEntry(dx, dy) * qGreen(pixel);
                                bTotal += getEntry(dx, dy) * qBlue(pixel);
                                }
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
        }
    }
    return newImage;
}

/**
 * @brief Returns image mask.
 * 
 * @return QImage 
 */
QImage ImageInpainting::getMask() {
    return mask;
}

/**
 * @brief Sets image mask.
 * 
 * @param mask New image to set as inpainting mask.
 */
void ImageInpainting::setMask(const QImage &mask) {
    this->mask = mask;
}
