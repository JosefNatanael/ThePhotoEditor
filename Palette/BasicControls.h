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
    explicit BasicControls(QWidget *parent = nullptr, int imageWidth = 0, int imageHeight = 0);
    ~BasicControls();

public slots:
    void on_cancelCutoutPushButton_clicked();

private slots:
    void on_beginCutoutPushButton_clicked();
    void on_applyPushButton_clicked();
    void on_sliderChange(int);
    void on_resizePushButton_clicked();

signals:
    void crossCursorChanged(WorkspaceArea::CursorMode, int data = 0);
    void applyTransformClicked(AbstractImageFilterTransform* transform, int size, double strength, bool fromServer = false);
    void resizeButtonClicked(int, int, bool fromServer = false);

private:
    Ui::BasicControls *ui;
    bool beginButtonClicked = false;    //!< It signifies begin use of magic crop or rectangle crop.
    int imageWidth = 0;                 //!< Saves image width, for aspect ratio calculation, image resize.
    int imageHeight = 0;                //!< Saves image height, for aspect ratio calculation, image resize.

public:
    void setImageDimensions(int, int);
};

#endif // BASICCONTROLS_H
