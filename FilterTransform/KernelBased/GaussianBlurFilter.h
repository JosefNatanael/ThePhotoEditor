#ifndef GAUSSIANBLURFILTER_H
#define GAUSSIANBLURFILTER_H

#include "../AbstractKernelBasedImageFilterTransform.h"


class GaussianBlurFilter : public AbstractKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit GaussianBlurFilter(QObject *parent = nullptr);
    virtual QImage applyFilter(const QImage &img, int size, double strength) const override;
    virtual QString getName() const override;
};

#endif // GAUSSIANBLURFILTER_H
