#ifndef SATURATIONFILTER_H
#define SATURATIONFILTER_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class SaturationFilter : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit SaturationFilter(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // SATURATIONFILTER_H
