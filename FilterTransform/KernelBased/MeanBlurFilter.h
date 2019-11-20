#ifndef MEANBLURFILTER_H
#define MEANBLURFILTER_H

#include "../AbstractKernelBasedImageFilterTransform.h"


class MeanBlurFilter : public AbstractKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit MeanBlurFilter(int size = 2,  QObject *parent = nullptr);
    virtual QImage applyFilter(const QImage &image, int size, double strength) override;
    virtual QImage applyFilter(const QImage &image, int size) override;
    virtual QString getName() const override;

    virtual void setKernel(int size, double strength = 1.0) override;
};

#endif // MEANBLURFILTER_H
