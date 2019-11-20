#ifndef EFFECTS_H
#define EFFECTS_H

#include <QWidget>

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

    void on_inpaintingPushButton_clicked();

signals:
    void gaussianBlurOnClicked();
    void meanBlurOnClicked();

private:
    Ui::Effects *ui;
};

#endif // EFFECTS_H
