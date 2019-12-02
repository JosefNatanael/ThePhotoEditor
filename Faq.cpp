#include "Faq.h"
#include "ui_Faq.h"

Faq::Faq(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Faq)
{
    ui->setupUi(this);
}

Faq::~Faq()
{
    delete ui;
}
