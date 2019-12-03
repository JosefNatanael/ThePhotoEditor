#ifndef IMAGEINPAINTING_H
#define IMAGEINPAINTING_H

#include "../AbstractKernelBasedImageFilterTransform.h"

class ImageInpainting : public AbstractKernelBasedImageFilterTransform
{
    Q_OBJECT
public:
    friend class Effects;

    explicit ImageInpainting(int size = 2,  QObject *parent = nullptr);
    virtual QImage applyFilter(const QImage &image, int size, double strength = 1.0) override;
    virtual QImage applyFilter(const QImage &image, int size = 2) override;
    virtual QString getName() const override;

    virtual void setKernel(int size, double strength = 1.0) override;
    virtual QImage convolution(const QImage& image) const override;

    virtual QImage getMask();
    virtual void setMask(const QImage& mask);

private:
    QImage mask;    //!< Inpainting mask.
    int size;       //!< Kernel size.
};

#endif // IMAGEINPAINTING_H
