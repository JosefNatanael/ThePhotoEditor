#ifndef FAQ_H
#define FAQ_H

#include <QDialog>

namespace Ui {
class Faq;
}

class Faq : public QDialog
{
    Q_OBJECT

public:
    explicit Faq(QWidget *parent = nullptr);
    ~Faq();

private:
    Ui::Faq *ui;
};

#endif // FAQ_H
