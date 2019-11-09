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

private:
    Ui::Brush *ui;
};

#endif // BRUSH_H
