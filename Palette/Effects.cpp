#include "Effects.h"
#include "ui_Effects.h"
#include "FilterTransform/KernelBased/GaussianBlurFilter.h"
#include "FilterTransform/KernelBased/MeanBlurFilter.h"
#include "FilterTransform/KernelBased/EmbossFilter.h"
#include "FilterTransform/KernelBased/EdgeDetectionFilter.h"
#include "FilterTransform/KernelBased/ImageInpainting.h"

#include <QDebug>
#include <QFileDialog>

Effects::Effects(QWidget *parent) : QWidget(parent),
                                    ui(new Ui::Effects)
{
    ui->setupUi(this);

    QPixmap logoEffect;
    logoEffect.load(":/icons/resources/effects-logo.svg");
    logoEffect = logoEffect.scaled(ui->effectLogo->size(), Qt::KeepAspectRatio);
    ui->effectLogo->setPixmap(logoEffect);
}

Effects::~Effects()
{
    delete ui;
}

void Effects::on_gaussianPushButton_clicked()
{
    GaussianBlurFilter* gaussianFilter = new GaussianBlurFilter();
    emit applyEffectClicked(gaussianFilter, ui->gaussianSizeSlider->value(), ui->gaussianStrengthSlider->value());
}

void Effects::on_meanPushButton_clicked()
{
    MeanBlurFilter* meanFilter = new MeanBlurFilter();
    emit applyEffectClicked(meanFilter, ui->meanSizeSlider->value(), 1);
}

void Effects::on_embossPushButton_clicked()
{
    EmbossFilter* embossFilter = new EmbossFilter();
    emit applyEffectClicked(embossFilter, ui->embossSizeSlider->value(), 1);
}

void Effects::on_edgePushButton_clicked()
{
    EdgeDetectionFilter* edgeDetectionFilter = new EdgeDetectionFilter();
    emit applyEffectClicked(edgeDetectionFilter, ui->edgeSizeSlider->value(), 1);
}

void Effects::on_inpaintingPushButton_clicked()
{
    //TODO
    //dummy:
    ImageInpainting* imageInpainting = new ImageInpainting(5);
    imageInpainting->mask = mask;
    emit applyEffectClicked(imageInpainting, 5 , 1);
}

void Effects::on_inpaintingAddMaskPushButton_clicked()
{
    //TODO
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    QImage imageLoaded;
    if (!fileName.isEmpty())
    {
        if (!imageLoaded.load(fileName))
        {
            return;
        }
        mask = imageLoaded.copy();
        //mask.save("mask.jpg");//works well here
    }
}
