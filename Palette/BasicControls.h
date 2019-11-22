#ifndef BASICCONTROLS_H
#define BASICCONTROLS_H

#include <QWidget>

#include "../FilterTransform/AbstractImageFilterTransform.h"

namespace Ui {
class BasicControls;
}

class BasicControls : public QWidget
{
    Q_OBJECT

public:
    explicit BasicControls(QWidget *parent = nullptr);
    ~BasicControls();

public:
    enum class CursorMode {
        SCRIBBLE,
        RECTANGLECROP,
        MAGICWAND,
        LASSO
    };

private slots:
    void on_beginCutoutPushButton_clicked();
    void on_cancelCutoutPushButton_clicked();
    void on_applyPushButton_clicked();

private:
    Ui::BasicControls *ui;
    bool beginButtonClicked = false;

signals:
    void crossCursorChanged(bool, CursorMode);
    void applyTransformClicked(AbstractImageFilterTransform* transform, int size, double strength);
};

#endif // BASICCONTROLS_H
