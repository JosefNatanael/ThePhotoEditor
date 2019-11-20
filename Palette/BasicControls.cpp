#include "BasicControls.h"
#include "ui_BasicControls.h"
#include <QPixmap>
#include <QDebug>

BasicControls::BasicControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BasicControls)
{
    ui->setupUi(this);

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

    QPixmap logoRectangle;
    logoRectangle.load(":/icons/resources/cutout-rectangle.svg");
    logoRectangle = logoRectangle.scaled(ui->rectangleLabel->size(), Qt::KeepAspectRatio);
    ui->rectangleLabel->setPixmap(logoRectangle);

    QPixmap logoMagic;
    logoMagic.load(":/icons/resources/cutout-magic-wand.svg");
    logoMagic = logoMagic.scaled(ui->magicLabel->size(), Qt::KeepAspectRatio);
    ui->magicLabel->setPixmap(logoMagic);

    QPixmap logoLasso;
    logoLasso.load(":/icons/resources/cutout-lasso.svg");
    logoLasso = logoLasso.scaled(ui->lassoLabel->size(), Qt::KeepAspectRatio);
    ui->lassoLabel->setPixmap(logoLasso);


    connect(ui->rectangleCutRadioButton, SIGNAL(clicked()), this, SLOT(onRadioButtonToggled()));
}

BasicControls::~BasicControls()
{
    delete ui;
}

void BasicControls::on_beginCutoutPushButton_clicked()
{
    beginButtonClicked = true;
    //all push button in basic controls is disabled, except cancel
    ui->applyPushButton->setEnabled(false);
    ui->cropPushButton->setEnabled(false);
    ui->beginCutoutPushButton->setEnabled(false);
    emit crossCursorChanged(true);
}

void BasicControls::on_cancelCutoutPushButton_clicked()
{
    beginButtonClicked = false;
    ui->applyPushButton->setEnabled(true);
    ui->cropPushButton->setEnabled(true);
    ui->beginCutoutPushButton->setEnabled(true);
    emit crossCursorChanged(false);
}
