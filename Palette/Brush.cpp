#include "Brush.h"
#include "ui_Brush.h"

Brush::Brush(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Brush)
{
    ui->setupUi(this);

    // Dynamically add a color dialog in our layout
    QColorDialog* colorDialog = new QColorDialog(Qt::blue);
    colorDialog->setWindowFlags(Qt::Widget);
    colorDialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
    ui->colorLayout->addWidget(colorDialog);

    connect(colorDialog, &QColorDialog::currentColorChanged, this, &Brush::penColorChanged);

}

Brush::~Brush()
{
    delete ui;
}

void Brush::onPenColorChanged(const QColor& color)
{
    emit penColorChanged(color);
}

void Brush::on_brushWidthSlider_valueChanged(int value)
{
    emit penWidthChanged(value);
}

void Brush::on_brushWidthSpinBox_valueChanged(int value)
{
    emit penWidthChanged(value);
}
