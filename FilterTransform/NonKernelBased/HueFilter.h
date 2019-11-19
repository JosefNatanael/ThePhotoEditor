#ifndef HUEFILTER_H
#define HUEFILTER_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class HueFilter : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit HueFilter(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // HUEFILTER_H
