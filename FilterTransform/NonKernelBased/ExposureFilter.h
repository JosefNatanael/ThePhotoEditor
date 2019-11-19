#ifndef EXPOSUREFILTER_H
#define EXPOSUREFILTER_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class ExposureFilter : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit ExposureFilter(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // EXPOSUREFILTER_H
