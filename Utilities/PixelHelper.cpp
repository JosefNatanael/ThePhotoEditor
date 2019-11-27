#include "PixelHelper.h"

QRgb PixelHelper::getPixel(const QImage& image, int x, int y)
{
    return *(reinterpret_cast<const QRgb*>(image.scanLine(y)) + x);
}

void PixelHelper::setPixel(QImage& image, int x, int y, QRgb value)
{
    *(reinterpret_cast<QRgb*>(image.scanLine(y)) + x) = value;
}

