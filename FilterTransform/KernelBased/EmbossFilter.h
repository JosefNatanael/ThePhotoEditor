#ifndef EMBOSSFILTER_H
#define EMBOSSFILTER_H

#include "../AbstractKernelBasedImageFilterTransform.h"

class EmbossFilter : public AbstractKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit EmbossFilter(int size = 2,  QObject *parent = nullptr);
    virtual QImage applyFilter(const QImage &image, int size, double strength = 1.0) override;
    virtual QImage applyFilter(const QImage &image, int size = 2) override;
    virtual QString getName() const override;

    virtual void setKernel(int size, double strength = 1.0) override;
};

#endif // EMBOSSFILTER_H
