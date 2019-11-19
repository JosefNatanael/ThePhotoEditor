#ifndef TEMPERATUREFILTER_H
#define TEMPERATUREFILTER_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class TemperatureFilter : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit TemperatureFilter(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // TEMPERATUREFILTER_H
