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

private slots:
    void on_beginCutoutPushButton_clicked();
    void on_cancelCutoutPushButton_clicked();
    void onRadioButtonToggled();

private:
    Ui::BasicControls *ui;
    bool beginButtonClicked = false;

signals:
    void crossCursorChanged(bool);
    void radioButtonToggled(const QString&);

};

#endif // BASICCONTROLS_H
