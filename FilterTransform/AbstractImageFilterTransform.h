#ifndef ABSTRACTIMAGEFILTERTRANSFORM_H
#define ABSTRACTIMAGEFILTERTRANSFORM_H

#include <QObject>
#include <QImage>
#include "../PixelHelper.h"

class AbstractImageFilterTransform : public QObject
{
    Q_OBJECT
public:
    explicit AbstractImageFilterTransform(QObject *parent = nullptr);
    virtual ~AbstractImageFilterTransform() = default;

    virtual QImage applyFilter(const QImage &img, int size, double strength) = 0;
    virtual QString getName() const = 0;

signals:

public slots:
};

#endif // ABSTRACTIMAGEFILTERTRANSFORM_H
