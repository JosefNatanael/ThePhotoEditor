#include "CommitDialog.h"
#include "ui_CommitDialog.h"

CommitDialog::CommitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommitDialog)
{
    ui->setupUi(this);
}

CommitDialog::~CommitDialog()
{
    delete ui;
}
