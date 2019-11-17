#ifndef INVERTFILTER_H
#define INVERTFILTER_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class InvertFilter : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit InvertFilter(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};
#endif // INVERTFILTER_H
