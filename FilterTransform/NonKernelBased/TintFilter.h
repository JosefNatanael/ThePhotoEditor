#ifndef TINTFILTER_H
#define TINTFILTER_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class TintFilter : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit TintFilter(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // TINTFILTER_H
