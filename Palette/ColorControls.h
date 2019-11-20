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

private slots:
    void on_bnwButton_clicked();

    void on_invertButton_clicked();

private:
    Ui::ColorControls *ui;

signals:
    void blackAndWhiteApplied();
    void invertApplied();
};

#endif // COLORCONTROLS_H
