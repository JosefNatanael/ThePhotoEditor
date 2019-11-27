#include "CommitImage.h"
#include "ui_CommitImage.h"

CommitImage::CommitImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommitImage)
{
    ui->setupUi(this);
}

CommitImage::~CommitImage()
{
    delete ui;
}
