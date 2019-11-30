/**
 * @class CommitDialog
 * @brief Merge to HEAD dialog.
 */

#include "CommitDialog.h"
#include "ui_CommitDialog.h"

/**
 * @brief Construct a new Commit Dialog:: Commit Dialog object
 * 
 * @param parent Passed to QDialog() constructor.
 */
CommitDialog::CommitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommitDialog)
{
    ui->setupUi(this);
}

/**
 * @brief Destroy the Commit Dialog:: Commit Dialog object
 */
CommitDialog::~CommitDialog()
{
    delete ui;
}
