#ifndef BRUSH_H
#define BRUSH_H

#include <QWidget>
#include <QColorDialog>
#include <QMouseEvent>

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
    void onPenColorChanged(const QColor& color);

public slots:
    void penColorChanged(const QColor& color);

private:
    Ui::Brush *ui;
};

#endif // BRUSH_H
