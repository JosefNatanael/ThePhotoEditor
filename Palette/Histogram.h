#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class Histogram;
}

class Histogram : public QWidget
{
    Q_OBJECT

public:
    explicit Histogram(QWidget *parent = nullptr);
    ~Histogram();

    QCPBars* mpHistogramBarsGrey;
    QCPBars* mpHistogramBarsRed;
    QCPBars* mpHistogramBarsGreen;
    QCPBars* mpHistogramBarsBlue;

    void drawHistogram (const QImage& image);

private:
    Ui::Histogram *ui;

public slots:
    void imageLoaded(const QImage& image);
};

#endif // HISTOGRAM_H
