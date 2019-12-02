/**
 * @class Faq
 * @brief For Frequently Asked Questions (FAQ) about The Photo Editor
 */
#include "Faq.h"
#include "ui_Faq.h"

/**
 * @brief Constructor for Faq::Faq
 * @param parent Passed to QDialog constructor.
 */
Faq::Faq(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Faq)
{
    ui->setupUi(this);
}

/**
 * @brief Destructor for Faq::~Faq
 */
Faq::~Faq()
{
    delete ui;
}
