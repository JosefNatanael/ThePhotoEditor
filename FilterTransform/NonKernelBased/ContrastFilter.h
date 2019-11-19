#ifndef CONTRASTFILTER_H
#define CONTRASTFILTER_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class ContrastFilter : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit ContrastFilter(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // CONTRASTFILTER_H
