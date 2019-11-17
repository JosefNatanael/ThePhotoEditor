#ifndef FLIPVERTICALTRANSFORM_H
#define FLIPVERTICALTRANSFORM_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class FlipVerticalTransform : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit FlipVerticalTransform(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};
#endif // FLIPVERTICALTRANSFORM_H
