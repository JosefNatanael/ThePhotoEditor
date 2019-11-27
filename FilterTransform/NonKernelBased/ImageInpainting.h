#ifndef IMAGEINPAINTING_H
#define IMAGEINPAINTING_H

#include "FilterTransform/AbstractNonKernelBasedImageFilterTransform.h"

class ImageInpainting : public AbstractNonKernelBasedImageFilterTransform
{
public:
    Q_OBJECT

public:
    explicit ImageInpainting(QObject* parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // IMAGEINPAINTING_H
