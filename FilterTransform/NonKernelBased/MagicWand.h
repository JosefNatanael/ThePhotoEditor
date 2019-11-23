#ifndef MAGICWAND_H
#define MAGICWAND_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"
#include "PixelHelper.h"

class MagicWand: public AbstractNonKernelBasedImageFilterTransform
{
public:
    Q_OBJECT
    //function from AbstractNonKernelBasedImageFilterTransform
public:
    explicit MagicWand(QObject* parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
public:
    QImage crop(const QImage& img, int x, int y, int threshold);
    //helper with PixelHelper
    void recursiveMagic(QImage& img, int x, int y, QRgb, int threshold);
};

#endif // MAGICWAND_H
