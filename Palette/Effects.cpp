#include "Effects.h"
#include "ui_Effects.h"
#include "FilterTransform/KernelBased/GaussianBlurFilter.h"
#include "FilterTransform/KernelBased/MeanBlurFilter.h"
#include <QDebug>

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
}

void Effects::on_meanPushButton_clicked()
{
}

void Effects::on_inpaintingPushButton_clicked()
{
    //TODO
}
