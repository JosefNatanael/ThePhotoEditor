#ifndef EFFECTS_H
#define EFFECTS_H

#include <QWidget>
#include "../FilterTransform/AbstractImageFilterTransform.h"

namespace Ui {
class Effects;
}

class Effects : public QWidget
{
    Q_OBJECT

public:
    explicit Effects(QWidget *parent = nullptr);
    ~Effects();

private slots:
    void on_gaussianPushButton_clicked();
    void on_meanPushButton_clicked();
    void on_embossPushButton_clicked();
    void on_edgePushButton_clicked();
    void on_inpaintingPushButton_clicked();

private:
    Ui::Effects *ui;

signals:
    void applyEffectClicked(AbstractImageFilterTransform* transform, int size, double strength);
};

#endif // EFFECTS_H
