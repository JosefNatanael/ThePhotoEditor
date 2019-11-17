#ifndef ABSTRACTNONKERNELBASEDIMAGEFILTERTRANSFORM_H
#define ABSTRACTNONKERNELBASEDIMAGEFILTERTRANSFORM_H

#include "AbstractImageFilterTransform.h"

class AbstractNonKernelBasedImageFilterTransform : public AbstractImageFilterTransform
{
    Q_OBJECT
public:
    explicit AbstractNonKernelBasedImageFilterTransform(QObject *parent = nullptr);
    virtual QImage applyFilter(const QImage &img, int size, double strength) override;
    virtual QImage applyFilter(const QImage &img, double strength) const = 0;
    virtual QImage applyFilter(const QImage &img) const = 0;
};

#endif // ABSTRACTNONKERNELBASEDIMAGEFILTERTRANSFORM_H
