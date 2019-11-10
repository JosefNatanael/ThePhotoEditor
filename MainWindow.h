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

#include "ScribbleArea.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void on_actionAbout_Us_triggered();
    void on_actionPrint_triggered();
    void on_actionOpen_triggered();
    void on_actionExit_triggered();

    void open();
    void save();
    void penColor();
    void penWidth();

    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;

private:
    void addRoot(QTreeWidgetItem* parent, QString name);
    void customAddChild(QTreeWidgetItem* parent, QWidget* widget);

    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    ScribbleArea *scribbleArea;

    QMenu *optionMenu;
    QList<QAction *> saveAsActs;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *clearScreenAct;
};

#endif // MAINWINDOW_H
