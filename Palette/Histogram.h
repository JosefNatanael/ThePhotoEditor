#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>

namespace Ui {
class Histogram;
}

class Histogram : public QWidget
{
    Q_OBJECT

public:
    explicit Histogram(QWidget *parent = nullptr);
    ~Histogram();

private:
    Ui::Histogram *ui;
};

#endif // HISTOGRAM_H
