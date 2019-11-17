#ifndef CLOCKWISEROTATIONTRANSFORM_H
#define CLOCKWISEROTATIONTRANSFORM_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class ClockwiseRotationTransform : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit ClockwiseRotationTransform(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // CLOCKWISEROTATIONTRANSFORM_H
