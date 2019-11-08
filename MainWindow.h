#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AboutUs.h"

#include <QTreeWidgetItem>

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>


namespace Ui {
class MainWindow;
}

class ScribbleArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_Us_triggered();
    void on_actionPrint_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";

private:
    void addRoot(QString name);
    void addChild(QTreeWidgetItem* parent, QString name);
};

#endif // MAINWINDOW_H
