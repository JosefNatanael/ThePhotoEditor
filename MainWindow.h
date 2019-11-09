#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AboutUs.h"

#include <QTreeWidgetItem>
#include <QToolBar>

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>


namespace Ui {
class MainWindow;
}

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
    void addRoot(QTreeWidgetItem* parent, QString name);
    void customAddChild(QTreeWidgetItem* parent, QWidget* widget);
};

#endif // MAINWINDOW_H
