#ifndef BRIGHTNESSFILTER_H
#define BRIGHTNESSFILTER_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class BrightnessFilter : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit BrightnessFilter(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // BRIGHTNESSFILTER_H
