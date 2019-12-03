/**
 * @class Effects
 * @brief Provides additional filters and image processing algorithms.
 */

#include "Effects.h"
#include "ui_Effects.h"
#include "FilterTransform/KernelBased/GaussianBlurFilter.h"
#include "FilterTransform/KernelBased/MeanBlurFilter.h"
#include "FilterTransform/KernelBased/EmbossFilter.h"
#include "FilterTransform/KernelBased/EdgeDetectionFilter.h"
#include "FilterTransform/KernelBased/ImageInpainting.h"
#include "FilterTransform/KernelBased/ImageScissors.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

/**
 * @brief Construct a new Effects:: Effects object
 * 
 * @param parent Passed to QWidget() constructor.
 */
Effects::Effects(QWidget *parent) : QWidget(parent),
                                    ui(new Ui::Effects)
{
    ui->setupUi(this);

    QPixmap logoEffect;
    logoEffect.load(":/icons/resources/effects-logo.svg");
    logoEffect = logoEffect.scaled(ui->effectLogo->size(), Qt::KeepAspectRatio);
    ui->effectLogo->setPixmap(logoEffect);
}

/**
 * @brief Destroy the Effects:: Effects object
 */
Effects::~Effects()
{
    delete ui;
}

/**
 * @brief Emits gaussian blur filter signal to mainwindow.
 */
void Effects::on_gaussianPushButton_clicked()
{
    GaussianBlurFilter* gaussianFilter = new GaussianBlurFilter();
    emit applyEffectClicked(gaussianFilter, ui->gaussianSizeSlider->value(), ui->gaussianStrengthSlider->value());
}

/**
 * @brief Emits mean blur filter signal to mainwindow.
 * 
 */
void Effects::on_meanPushButton_clicked()
{
    MeanBlurFilter* meanFilter = new MeanBlurFilter();
    emit applyEffectClicked(meanFilter, ui->meanSizeSlider->value(), 1);
}

/**
 * @brief Emits emboss filter signal to mainwindow.
 * 
 */
void Effects::on_embossPushButton_clicked()
{
    EmbossFilter* embossFilter = new EmbossFilter();
    emit applyEffectClicked(embossFilter, ui->embossSizeSlider->value(), 1);
}

/**
 * @brief Emits edge detection filter signal to mainwindow.
 * 
 */
void Effects::on_edgePushButton_clicked()
{
    EdgeDetectionFilter* edgeDetectionFilter = new EdgeDetectionFilter();
    emit applyEffectClicked(edgeDetectionFilter, ui->edgeSizeSlider->value(), 1);
}

/**
 * @brief Opens get file dialog, loads the mask.
 * 
 */
void Effects::on_inpaintingAddMaskPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    QImage imageLoaded;
    if (!fileName.isEmpty())
    {
        if (!imageLoaded.load(fileName))
        {
            return;
        }
        mask = imageLoaded.copy();
    }
}

/**
 * @brief Emits inpainting signal to mainwindow.
 * 
 */
void Effects::on_inpaintingPushButton_clicked()
{
    if (mask.isNull()) {
        QMessageBox::information(this, QString("No Mask"), QString("Please add mask first."));
        return;
    }
    ImageInpainting* imageInpainting = new ImageInpainting(3);
    imageInpainting->mask = mask;
    emit applyEffectClicked(imageInpainting, 3, 1);
}

/**
 * @brief Opens get file dialog, loads the mask.
 * 
 */
void Effects::on_imageScissorsAddMaskPushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    QImage imageLoaded;
    if (!fileName.isEmpty())
    {
        if (!imageLoaded.load(fileName))
        {
            return;
        }
        mask = imageLoaded.copy();
    }
}

/**
 * @brief Emits image scissors signal to mainwindow.
 * 
 */
void Effects::on_imageScissorsPushButton_clicked()
{
    if (mask.isNull()) {
        QMessageBox::information(this, QString("No Mask"), QString("Please add mask first."));
        return;
    }
    ImageScissors* imageScissors = new ImageScissors(2);
    imageScissors->mask = mask;
    emit applyEffectClicked(imageScissors, 2 , 1);
}
