#ifndef FLIPHORIZONTALTRANSFORM_H
#define FLIPHORIZONTALTRANSFORM_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class FlipHorizontalTransform : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit FlipHorizontalTransform(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // FLIPHORIZONTALTRANSFORM_H
