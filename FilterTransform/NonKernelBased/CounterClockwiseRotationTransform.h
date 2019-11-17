#ifndef COUNTERCLOCKWISEROTATIONTRANSFORM_H
#define COUNTERCLOCKWISEROTATIONTRANSFORM_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class CounterClockwiseRotationTransform : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit CounterClockwiseRotationTransform(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // COUNTERCLOCKWISEROTATIONTRANSFORM_H
