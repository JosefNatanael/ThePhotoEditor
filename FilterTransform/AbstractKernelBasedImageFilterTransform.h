#ifndef ABSTRACTKERNELBASEDIMAGEFILTERTRANSFORM_H
#define ABSTRACTKERNELBASEDIMAGEFILTERTRANSFORM_H

#include "AbstractImageFilterTransform.h"

#include <QVector>
#include <QColor>
#include <QtMath>

class AbstractKernelBasedImageFilterTransform : public AbstractImageFilterTransform
{
    Q_OBJECT
public:
    explicit AbstractKernelBasedImageFilterTransform(int size = 1, QObject *parent = nullptr);
    virtual QImage applyFilter(const QImage &img, int size, double strength) override;
    virtual QImage applyFilter(const QImage &img, int size) = 0;

    QImage convolution(const QImage& image) const;
    virtual void setKernel(int size, double strength) = 0;

protected:
    double getEntry(int x, int y) const;
    void setEntry(int x, int y, double value);
    void setSize(int newSize);
    void redefineKernel(int size);

private:
    int size;
    QVector<QVector<double>> kernel;

};

#endif // ABSTRACTKERNELBASEDIMAGEFILTERTRANSFORM_H
