#ifndef COLORCONTROLS_H
#define COLORCONTROLS_H

#include <QWidget>

namespace Ui {
class ColorControls;
}

class ColorControls : public QWidget
{
    Q_OBJECT

public:
    explicit ColorControls(QWidget *parent = nullptr);
    ~ColorControls();

private:
    Ui::ColorControls *ui;
};

#endif // COLORCONTROLS_H
