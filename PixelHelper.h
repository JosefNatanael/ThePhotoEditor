#ifndef PIXELHELPER_H
#define PIXELHELPER_H

#include <QColor>
#include <QImage>

class PixelHelper
{
public:
    PixelHelper() = delete;
    static QRgb getPixel(const QImage& img, int x, int y);
    static void setPixel(QImage& img, int x, int y, QRgb value);
};

#endif // PIXELHELPER_H
