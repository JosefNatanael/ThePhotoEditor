#ifndef GRAYSCALEFILTER_H
#define GRAYSCALEFILTER_H

#include "../AbstractNonKernelBasedImageFilterTransform.h"

class GrayscaleFilter : public AbstractNonKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    explicit GrayscaleFilter(QObject *parent = nullptr);
    virtual QString getName() const override;
    virtual QImage applyFilter(const QImage &img, double strength) const override;
    virtual QImage applyFilter(const QImage &img) const override;
};

#endif // GRAYSCALEFILTER_H
