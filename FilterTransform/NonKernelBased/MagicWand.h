#ifndef MAGICWAND_H
#define MAGICWAND_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"
#include <QQueue>

class MagicWand: public AbstractNonKernelBasedImageFilterTransform
{
public:
    Q_OBJECT

public:
    explicit MagicWand(QObject* parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;

public:
    QImage crop(const QImage& img, int x, int y, int threshold);
    void forestFire(QImage& img, int x, int y);

private:
    class Point {
        int x = -1;
        int y = -1;
    public:
        Point() : x(-1), y(-1) {}
        Point(int x, int y) : x(x), y(y) {}
        int getX() const { return x; }
        int getY() const { return y; }
    };

private:
    bool colorWithinThreshold(QRgb colorToCheck);

private:
    int originalColorRed;
    int originalColorGreen;
    int originalColorBlue;
    int threshold;
    QQueue<Point> forestFireQueue;
};

#endif // MAGICWAND_H
