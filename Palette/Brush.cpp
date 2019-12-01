/**
 * @class Brush
 * @brief Provides scribbling feature.
 */

#include "Brush.h"
#include "ui_Brush.h"

/**
 * @brief Construct a new Brush:: Brush object
 * 
 * @param parent Passed to QWidget() constructor.
 */
Brush::Brush(QWidget *parent) : QWidget(parent),
                                ui(new Ui::Brush)
{
    ui->setupUi(this);

    // Dynamically add a color dialog in our layout
    QColorDialog *colorDialog = new QColorDialog(Qt::blue);
    colorDialog->setWindowFlags(Qt::Widget);
    colorDialog->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
    ui->colorLayout->addWidget(colorDialog);

    connect(colorDialog, &QColorDialog::currentColorChanged, this, &Brush::penColorChanged);
}

/**
 * @brief Destroy the Brush:: Brush object
 */
Brush::~Brush()
{
    delete ui;
}

/**
 * @brief Emits a signal to main window to update pen color.
 * 
 * @param color Pen color to update.
 */
void Brush::onPenColorChanged(const QColor &color)
{
    emit penColorChanged(color);
}

/**
 * @brief Emits a signal to main window to update pen width based on value.
 * 
 * @param value Pen pixel size.
 */
void Brush::on_brushWidthSlider_valueChanged(int value)
{
    emit penWidthChanged(value);
}

/**
 * @brief Emits a signal to main window to update pen width based on value.
 * 
 * @param value Pen pixel size.
 */
void Brush::on_brushWidthSpinBox_valueChanged(int value)
{
    emit penWidthChanged(value);
}
