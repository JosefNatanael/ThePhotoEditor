#ifndef COMMITIMAGE_H
#define COMMITIMAGE_H

#include <QDialog>

namespace Ui {
class CommitImage;
}

class CommitImage : public QDialog
{
    Q_OBJECT

public:
    explicit CommitImage(QWidget *parent = nullptr);
    ~CommitImage();

private:
    Ui::CommitImage *ui;
};

#endif // COMMITIMAGE_H
