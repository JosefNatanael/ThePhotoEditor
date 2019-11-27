#include "ImageInpainting.h"
#include "Utilities/PixelHelper.h"

ImageInpainting::ImageInpainting(int size, QObject* parent) : AbstractKernelBasedImageFilterTransform(size, parent)
{
    setKernel(size);
}

QImage ImageInpainting::applyFilter(const QImage &image, int size, double)
{
    setSize(size);
    setKernel(size);
    return convolution(image);
}

QImage ImageInpainting::applyFilter(const QImage &image, int size)
{
    setSize(size);
    setKernel(size);
    return applyFilter(image, size, 1.0);
}

QString ImageInpainting::getName() const
{
    return "Image Inpainting";
}

void ImageInpainting::setKernel(int size, double)
{
    redefineKernel(size);
    for (int dx = -size + 1; dx < size; ++dx)
        for (int dy = -size + 1; dy < size; ++dy) {
            setEntry(dx, dy, (2*size - 1)*(2*size - 1)-1);
    }                                                                    // if size is 2, the kernel would be 3x3, from 2*size -1
    setEntry(0, 0, 0);                                                   // middle entry is 0
}

QImage ImageInpainting::convolution(const QImage &img) const
{
    QImage newImage{img}; // create new image
    int normalizeFactor = 0;                                            //normalize the kernel
    for(int dx = -size +1; dx<size; ++dx){
        for(int dy = -size + 1; dy<size; ++dy){
            normalizeFactor += getEntry(dx, dy);
        }
    }

    for (int i = 0; i < img.width(); ++i) {
        for (int j = 0; j < img.height(); ++j) {
            if(PixelHelper::getPixel(mask, i, j) != qRgb(0,0,0)){       //assume mask has same size as img and monochrome.
                QRgb color = qRgb(0, 0, 0);//initialize to black
                int rTotal = 0, gTotal = 0, bTotal = 0;
                for(int dx = -size + 1; dx < size; ++dx) {
                    for(int dy = -size + 1; dy < size; ++dy) {
                        int X = i + dx, Y = j + dy;
                        if((0 < X && X < img.width()) && (0 < Y && Y < img.height())) {
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
    }
    return newImage;
}
