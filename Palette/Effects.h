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
    void on_inpaintingAddMaskPushButton_clicked();
    void on_inpaintingPushButton_clicked();
    void on_imageScissorsAddMaskPushButton_clicked();
    void on_imageScissorsPushButton_clicked();

private:
    Ui::Effects *ui;
    QString maskFileName;   //!< Mask file name. Masks are used for image inpainting and image scissors.
    QImage mask;            //!< Mask image.

signals:
    void applyEffectClicked(AbstractImageFilterTransform* transform, int size, double strength, bool fromServer = false);
};

#endif // EFFECTS_H
