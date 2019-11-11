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
#include <QVector>

#include "WorkspaceArea.h"

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
    void                        closeEvent(QCloseEvent* event) override;

private slots:
    void                        on_actionNew_triggered();
    void                        on_actionOpen_triggered();
    void                        on_actionPrint_triggered();
    void                        on_actionAbout_Us_triggered();
    void                        on_actionExit_triggered();
    void                        on_actionUndo_triggered();

    void                        open();
    void                        save();
    void                        on_edit(QGraphicsPathItem*);


private:
    Ui::MainWindow*             ui;

private:
    void                        addRoot(QTreeWidgetItem* parent, QString name);
    void                        customAddChild(QTreeWidgetItem* parent, QWidget* widget);

    void                        createActions();
    void                        createMenus();
    bool                        maybeSave();
    bool                        saveFile(const QByteArray &fileFormat);

    WorkspaceArea*              workspaceArea;
    QGraphicsView*              graphicsView;
    QVector<QGraphicsPathItem*> history;

    QMenu*                      optionMenu;
    QList<QAction *>            saveAsActs;
    QAction*                    clearScreenAct;
};

#endif // MAINWINDOW_H
