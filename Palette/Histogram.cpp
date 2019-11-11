#include "Histogram.h"
#include "ui_Histogram.h"

Histogram::Histogram(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Histogram)
{
    ui->setupUi(this);
}

Histogram::~Histogram()
{
    delete ui;
}
