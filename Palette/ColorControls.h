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
    void onSliderValueChanged(int, int);

private:
    Ui::ColorControls *ui;

public:
    QPixmap imagePreview;

    void setImagePreview(const QImage&);
    void resetSliders();

signals:
    void applyColorFilterClicked(AbstractImageFilterTransform* transform, int size, double strength, bool fromServer = false);
    void applyColorFilterOnPreview(AbstractImageFilterTransform* transform, int size, double strength);

};

#endif // COLORCONTROLS_H
