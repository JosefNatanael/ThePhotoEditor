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

    QCPBars* mpHistogramBarsGrey;   //!< Bar for grey color
    QCPBars* mpHistogramBarsRed;    //!< Bar for red color
    QCPBars* mpHistogramBarsGreen;  //!< Bar for green color
    QCPBars* mpHistogramBarsBlue;   //!< Bar for blue color

    void drawHistogram (const QImage& image);

private:
    Ui::Histogram *ui;

public slots:
    void onImageLoaded(const QImage& image);
};

#endif // HISTOGRAM_H
