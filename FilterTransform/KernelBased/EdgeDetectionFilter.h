#ifndef EDGEDETECTIONFILTER_H
#define EDGEDETECTIONFILTER_H

#include "../AbstractKernelBasedImageFilterTransform.h"

class EdgeDetectionFilter : public AbstractKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit EdgeDetectionFilter(int size = 2,  QObject *parent = nullptr);
    virtual QImage applyFilter(const QImage &image, int size, double strength = 1.0) override;
    virtual QImage applyFilter(const QImage &image, int size = 2) override;
    virtual QString getName() const override;

    virtual void setKernel(int size, double strength = 1.0) override;
};

#endif // EDGEDETECTIONFILTER_H
