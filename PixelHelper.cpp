#include "PixelHelper.h"

QRgb PixelHelper::getPixel(const QImage& image, int x, int y)
{
    return *(reinterpret_cast<const QRgb*>(image.scanLine(x)) + y);
}

void PixelHelper::setPixel(QImage& image, int x, int y, QRgb value)
{
    *(reinterpret_cast<QRgb*>(image.scanLine(x)) + y) = value;
}

