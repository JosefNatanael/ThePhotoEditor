#include "Brush.h"
#include "ui_Brush.h"

Brush::Brush(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Brush)
{
    ui->setupUi(this);

    // Dynamically add a color dialog in our layout
    QColorDialog* colorDialog = new QColorDialog();
    colorDialog->setWindowFlags(Qt::Widget);
    colorDialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
    ui->colorLayout->addWidget(colorDialog);
}

Brush::~Brush()
{
    delete ui;
}
