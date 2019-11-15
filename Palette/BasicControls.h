#ifndef BASICCONTROLS_H
#define BASICCONTROLS_H

#include <QWidget>

namespace Ui {
class BasicControls;
}

class BasicControls : public QWidget
{
    Q_OBJECT

public:
    explicit BasicControls(QWidget *parent = nullptr);
    ~BasicControls();

private:
    Ui::BasicControls *ui;
};

#endif // BASICCONTROLS_H
