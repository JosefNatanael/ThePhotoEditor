#include "Histogram.h"
#include "ui_Histogram.h"
#include "qcustomplot.cpp"
#include "WorkspaceArea.h"

Histogram::Histogram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Histogram)
{
    ui->setupUi(this);
    //grey
    mpHistogramBarsGrey = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
    QColor gray70 = Qt::gray;
    gray70.setAlphaF( 0.7 );
    mpHistogramBarsGrey->setPen(QPen(gray70));
    mpHistogramBarsGrey->setBrush(QBrush(gray70));

    //red
    mpHistogramBarsRed = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
    QColor red70 = Qt::red;
    red70.setAlphaF( 0.5 );
    mpHistogramBarsRed->setPen(QPen(red70));
    mpHistogramBarsRed->setBrush(QBrush(red70));

    //green
    mpHistogramBarsGreen = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
    QColor green70 = Qt::green;
    green70.setAlphaF( 0.3 );
    mpHistogramBarsGreen->setPen(QPen(green70));
    mpHistogramBarsGreen->setBrush(QBrush(green70));

    //blue
    mpHistogramBarsBlue = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
    QColor blue70 = Qt::blue;
    blue70.setAlphaF( 0.3 );
    mpHistogramBarsBlue->setPen(QPen(blue70));
    mpHistogramBarsBlue->setBrush(QBrush(blue70));
}

Histogram::~Histogram()
{
    delete mpHistogramBarsGrey;
    delete mpHistogramBarsRed;
    delete mpHistogramBarsGreen;
    delete mpHistogramBarsBlue;
    delete ui;
}

void Histogram::drawHistogram(const QImage &image){
    QVector<double> keys(256);
    QVector<double> valuesGrey(256);
    QVector<double> valuesRed(256);
    QVector<double> valuesGreen(256);
    QVector<double> valuesBlue(256);

    for(int i = 0; i < keys.size(); i++){
        keys[i] = i;
        valuesGrey[i] = 0;
        valuesRed[i] = 0;
        valuesGreen[i] = 0;
        valuesBlue[i] = 0;
    }
    for(int j = 0; j < image.height(); j++){
        for(int i = 0; i < image.width(); i++){
            QRgb rgb = image.pixel(i, j);// RGB -> Y
            int yGrey = qBound(0, static_cast<int>(0.299 * qRed(rgb) + 0.587 * qGreen(rgb) + 0.114 * qBlue(rgb) + 0.5), 255);
            int yRed = qBound(0, static_cast<int>(1.0 * qRed(rgb)+ 0.5), 255);
            int yGreen = qBound(0, static_cast<int>(1.0 * qGreen(rgb)+ 0.5), 255);
            int yBlue = qBound(0, static_cast<int>(1.0 * qBlue(rgb) + 0.5), 255);
            valuesGrey[yGrey] += 1.0;
            valuesRed[yRed] += 1.0;
            valuesGreen[yGreen] += 1.0;
            valuesBlue[yBlue] += 1.0;
        }
    }
    mpHistogramBarsGrey->setData(keys, valuesGrey);
    mpHistogramBarsRed->setData(keys, valuesRed);
    mpHistogramBarsGreen->setData(keys, valuesGreen);
    mpHistogramBarsBlue->setData(keys, valuesBlue);
    ui->plot->rescaleAxes();
    ui->plot->replot();
}

void Histogram::imageLoaded(const QImage& image) {
    drawHistogram(image);
}
