#ifndef BRUSH_H
#define BRUSH_H

#include <QWidget>
#include <QColorDialog>

namespace Ui {
class Brush;
}

class Brush : public QWidget
{
    Q_OBJECT

public:
    explicit Brush(QWidget *parent = nullptr);
    ~Brush();

signals:
    void penColorChanged(const QColor& color);
    void penWidthChanged(int value);

private slots:
    void onPenColorChanged(const QColor& color);
    void on_brushWidthSlider_valueChanged(int value);
    void on_brushWidthSpinBox_valueChanged(int arg1);

private:
    Ui::Brush *ui;
};

#endif // BRUSH_H
