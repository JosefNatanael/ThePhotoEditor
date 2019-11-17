#ifndef GAUSSIANBLURFILTER_H
#define GAUSSIANBLURFILTER_H

#include "../AbstractKernelBasedImageFilterTransform.h"


class GaussianBlurFilter : public AbstractKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit GaussianBlurFilter(int size = 2, double sd = 1.0, QObject *parent = nullptr);
    virtual QImage applyFilter(const QImage &image, int size, double strength) override;
    virtual QImage applyFilter(const QImage &image, int size) override;
    virtual QString getName() const override;

    virtual void setKernel(int size, double sd) override;

};

#endif // GAUSSIANBLURFILTER_H
