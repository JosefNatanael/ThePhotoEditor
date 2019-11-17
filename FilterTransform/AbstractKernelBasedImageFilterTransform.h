#ifndef ABSTRACTKERNELBASEDIMAGEFILTERTRANSFORM_H
#define ABSTRACTKERNELBASEDIMAGEFILTERTRANSFORM_H

#include "AbstractImageFilterTransform.h"

class AbstractKernelBasedImageFilterTransform : public AbstractImageFilterTransform
{
    Q_OBJECT
public:
    explicit AbstractKernelBasedImageFilterTransform(QObject *parent = nullptr);
    virtual QImage applyFilter(const QImage &img, int size, double strength) const override;
    virtual QImage applyFilter(const QImage &img, int size) const = 0;

};

#endif // ABSTRACTKERNELBASEDIMAGEFILTERTRANSFORM_H
