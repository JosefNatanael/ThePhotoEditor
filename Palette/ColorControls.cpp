#include "ColorControls.h"
#include "ui_ColorControls.h"

ColorControls::ColorControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorControls)
{
    ui->setupUi(this);
}

ColorControls::~ColorControls()
{
    delete ui;
}

void ColorControls::on_bnwButton_clicked()
{

}
