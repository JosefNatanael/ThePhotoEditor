/**
 * @class AboutUs
 * @brief About us dialog class. 
 */

#include "AboutUs.h"
#include "ui_AboutUs.h"

/**
 * @brief Constructor for AboutUs::AboutUs
 * @param parent Passed to QDialog constructor.
 */
AboutUs::AboutUs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutUs)
{
    ui->setupUi(this);
}

/**
 * @brief Destructor for AboutUs::~AboutUs
 */
AboutUs::~AboutUs()
{
    delete ui;
}
