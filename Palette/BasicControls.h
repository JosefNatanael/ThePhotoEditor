#ifndef BASICCONTROLS_H
#define BASICCONTROLS_H

#include <QWidget>
#include "../WorkspaceArea.h"

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

public slots:
    void on_cancelCutoutPushButton_clicked();

private slots:
    void on_beginCutoutPushButton_clicked();
    void on_applyPushButton_clicked();

private:
    Ui::BasicControls *ui;
    bool beginButtonClicked = false;

signals:
    void crossCursorChanged(WorkspaceArea::CursorMode, int data = 0);
    void applyTransformClicked(AbstractImageFilterTransform* transform, int size, double strength);
};

#endif // BASICCONTROLS_H
