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
    /**
     * @class MagicWand::Point
     * @brief Helper class about position of pixel (x,y)
     */

    class Point {
    private:
        int x = -1;
        int y = -1;
    public:
        /**
         * @brief Default constructor of MagicWand::Point
         *
         * @details sets x and y to default value of -1
         */
        Point() : x(-1), y(-1) {}
        /**
         * @brief Conversion constructor of MagicWand::Point
         * @param x
         * @param y
         *
         * @details sets x and y with input of x and y
         */
        Point(int x, int y) : x(x), y(y) {}
        /**
         * @brief accessor of private x
         *
         * @details returns the x position of MagicWand::Point object
         */
        int getX() const { return x; }
        /**
         * @brief accessor of private y
         *
         * @details returns the y position of MagicWand::Point object
         */
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
