/**
 * @class BasicControls
 * @brief Provides basic photo editing controls.
 * @details Features include: iamge resizing, image cropping, image transformations.
 */
#include "BasicControls.h"
#include "ui_BasicControls.h"
#include <QPixmap>

#include "../FilterTransform/NonKernelBased/CounterClockwiseRotationTransform.h"
#include "../FilterTransform/NonKernelBased/ClockwiseRotationTransform.h"
#include "../FilterTransform/NonKernelBased/FlipHorizontalTransform.h"
#include "../FilterTransform/NonKernelBased/FlipVerticalTransform.h"

/**
 * @brief Construct a new Basic Controls:: Basic Controls object
 * 
 * @param parent Passed to QWidget() constructor.
 * @param imageWidth Sets the image width.
 * @param imageHeight Sets the image height.
 */
BasicControls::BasicControls(QWidget *parent, int imageWidth, int imageHeight) : QWidget(parent),
                                                ui(new Ui::BasicControls), imageWidth(imageWidth), imageHeight(imageHeight)
{
    ui->setupUi(this);

    // Image resize setup
    ui->cropWidthSpinBox->setSuffix(" %");
    ui->cropWidthSpinBox->setRange(1, 100);
    ui->resizeSlider->setRange(1, 100);
    connect(ui->cropWidthSpinBox, SIGNAL(valueChanged(int)), ui->resizeSlider, SLOT(setValue(int)));
    connect(ui->resizeSlider, SIGNAL(valueChanged(int)), ui->cropWidthSpinBox, SLOT(setValue(int)));
    connect(ui->cropWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_sliderChange(int)));
    connect(ui->resizeSlider, SIGNAL(valueChanged(int)), this, SLOT(on_sliderChange(int)));
    ui->cropWidthSpinBox->setValue(100);
    ui->originalSizeLabel->setText(QString("Original : %1 x %2").arg(imageWidth).arg(imageHeight));
    ui->modifiedSizeLabel->setText(
                QString("Modified : %1 x %2").arg((int) (ui->cropWidthSpinBox->value() / 100.0 * imageWidth)).arg((int) (ui->cropWidthSpinBox->value() / 100.0 * imageHeight)));

    // Image transformation setup
    QPixmap logoCcw;
    logoCcw.load(":/icons/resources/rotate-left.svg");
    logoCcw = logoCcw.scaled(ui->ccwLabel->size(), Qt::KeepAspectRatio);
    ui->ccwLabel->setPixmap(logoCcw);

    QPixmap logoCw;
    logoCw.load(":/icons/resources/rotate-right.svg");
    logoCw = logoCw.scaled(ui->cwLabel->size(), Qt::KeepAspectRatio);
    ui->cwLabel->setPixmap(logoCw);

    QPixmap logoFlipHorizontal;
    logoFlipHorizontal.load(":/icons/resources/flip-horizontal.svg");
    logoFlipHorizontal = logoFlipHorizontal.scaled(ui->horizontalLabel->size(), Qt::KeepAspectRatio);
    ui->horizontalLabel->setPixmap(logoFlipHorizontal);

    QPixmap logoFlipVertical;
    logoFlipVertical.load(":/icons/resources/flip-vertical.svg");
    logoFlipVertical = logoFlipVertical.scaled(ui->verticalLabel->size(), Qt::KeepAspectRatio);
    ui->verticalLabel->setPixmap(logoFlipVertical);

    // Image cropping setup
    QPixmap logoRectangle;
    logoRectangle.load(":/icons/resources/cutout-rectangle.svg");
    logoRectangle = logoRectangle.scaled(ui->rectangleLabel->size(), Qt::KeepAspectRatio);
    ui->rectangleLabel->setPixmap(logoRectangle);

    QPixmap logoMagic;
    logoMagic.load(":/icons/resources/cutout-magic-wand.svg");
    logoMagic = logoMagic.scaled(ui->magicLabel->size(), Qt::KeepAspectRatio);
    ui->magicLabel->setPixmap(logoMagic);
}

/**
 * @brief Destroy the Basic Controls:: Basic Controls object
 */
BasicControls::~BasicControls()
{
    delete ui;
}

/**
 * @brief Updates push buttons state, Emits cutout signal to main window.
 */
void BasicControls::on_beginCutoutPushButton_clicked()
{
    beginButtonClicked = true;
    //all push button in basic controls is disabled, except cancel
    ui->applyPushButton->setEnabled(false);
    ui->resizePushButton->setEnabled(false);
    ui->beginCutoutPushButton->setEnabled(false);
    if (ui->rectangleCutRadioButton->isChecked())
    {
        emit crossCursorChanged(WorkspaceArea::CursorMode::RECTANGLECROP);
    }
    else if (ui->magicCutRadioButton->isChecked())
    {
        emit crossCursorChanged(WorkspaceArea::CursorMode::MAGICWAND, ui->magicSpinBox->value());
    }
}

/**
 * @brief Updates push buttons state. Cursor back to default: scribble mode.
 * 
 */
void BasicControls::on_cancelCutoutPushButton_clicked()
{
    beginButtonClicked = false;
    ui->applyPushButton->setEnabled(true);
    ui->resizePushButton->setEnabled(true);
    ui->beginCutoutPushButton->setEnabled(true);
    emit crossCursorChanged(WorkspaceArea::CursorMode::SCRIBBLE);
}

/**
 * @brief Emits apply transformation signal to main window.
 */
void BasicControls::on_applyPushButton_clicked()
{
    if (ui->ccwRadioButton->isChecked())
    {
        CounterClockwiseRotationTransform *ccwRotation = new CounterClockwiseRotationTransform();
        emit applyTransformClicked(ccwRotation, 1, 1);
    }
    else if (ui->cwRadioButton->isChecked())
    {
        ClockwiseRotationTransform *cwRotation = new ClockwiseRotationTransform();
        emit applyTransformClicked(cwRotation, 1, 1);
    }
    else if (ui->horizontalRadioButton->isChecked())
    {
        FlipHorizontalTransform *flipHorizontal = new FlipHorizontalTransform();
        emit applyTransformClicked(flipHorizontal, 1, 1);
    }
    else if (ui->verticalRadioButton->isChecked())
    {
        FlipVerticalTransform *flipVertical = new FlipVerticalTransform();
        emit applyTransformClicked(flipVertical, 1, 1);
    }
}

/**
 * @brief Sets image dimensions according to width and height.
 * 
 * @param width Image width.
 * @param height Image height.
 */
void BasicControls::setImageDimensions(int width, int height)
{
    imageWidth = width;
    imageHeight = height;
    ui->originalSizeLabel->setText(QString("Original : %1 x %2").arg(imageWidth).arg(imageHeight));
    ui->modifiedSizeLabel->setText(
                QString("Modified : %1 x %2").arg((int) (ui->cropWidthSpinBox->value() / 100.0 * imageWidth)).arg((int) (ui->cropWidthSpinBox->value() / 100.0 * imageHeight)));


}

/**
 * @brief Updates dimensions label on slider change.
 * 
 * @param value Updates based on this slider value.
 */
void BasicControls::on_sliderChange(int value)
{
    ui->originalSizeLabel->setText(QString("Original : %1 x %2").arg(imageWidth).arg(imageHeight));
    ui->modifiedSizeLabel->setText(
                QString("Modified : %1 x %2").arg((int) (value / 100.0 * imageWidth)).arg((int) (value / 100.0 * imageHeight)));

}

/**
 * @brief Emits resized image width and height to workspace area.
 */
void BasicControls::on_resizePushButton_clicked()
{
    int newWidth = (ui->cropWidthSpinBox->value() / 100.0) * imageWidth;
    int newHeight = (ui->cropWidthSpinBox->value() / 100.0) * imageHeight;
    if (newWidth != imageWidth && newHeight != imageHeight) {
        emit resizeButtonClicked(newWidth, newHeight);
    }
    ui->cropWidthSpinBox->setValue(100);
}
