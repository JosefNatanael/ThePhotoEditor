#ifndef COLORCONTROLS_H
#define COLORCONTROLS_H

#include <QWidget>
#include "../FilterTransform/AbstractImageFilterTransform.h"

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
    void on_applyColorButton_clicked();
    void on_applyLightingButton_clicked();

private:
    Ui::ColorControls *ui;

signals:
    void applyColorFilterClicked(AbstractImageFilterTransform* transform, int size, double strength);

};

#endif // COLORCONTROLS_H
