#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTreeWidgetItem>
#include <QToolBar>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
#include <QVector>

#include "Utilities/WindowHelper.h"
#include "Utilities/VersionControl.h"
#include "Utilities/CommitDialog.h"

#include "WorkspaceArea.h"

#include "FilterTransform/AbstractImageFilterTransform.h"
#include "Palette/Histogram.h"
#include "Palette/Brush.h"
#include "Palette/ColorControls.h"
#include "Palette/BasicControls.h"
#include "Palette/Effects.h"
#include "AboutUs.h"
#include "ServerRoom.h"
#include "Server/Server.h"
#include "Server/Client.h"

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
    virtual bool                eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void                        on_actionNew_triggered();
    void                        on_actionOpen_triggered();
    void                        on_actionPrint_triggered();
    void                        on_actionAbout_Us_triggered();
    void                        on_actionExit_triggered();
    void                        on_actionUndo_triggered();
    void                        on_actionRedo_triggered();
    void                        on_actionRevert_to_Last_Commit_triggered();
    void                        on_actionSave_triggered();

    // Saving, workspace related slots.
    void                        saveAs();
    void                        onImageDrawn();
    void                        clearImage();
    void                        onZoom(const QString&);
    void                        onCrossCursorChanged(WorkspaceArea::CursorMode, int data);
    void                        rerenderWorkspaceArea(const QImage&, int width, int height);
    void                        applyFilterTransform(AbstractImageFilterTransform* filterTransform, int size, double strength, bool fromServer = false);
    void                        applyFilterTransformOnPreview(AbstractImageFilterTransform* filterTransform, int size, double strength);
    void                        onUpdateImagePreview();

    // Server related slots.
    void                        on_actionCreate_Room_triggered();
    void                        on_actionJoin_Room_triggered();
    void                        on_actionView_Room_triggered();
    void                        onCreateRoom(QString);
    void                        onJoinRoom(QString, QString, quint16);
    void                        sendPlayerName();
    void                        clientJsonReceived(const QJsonObject&);
    void                        goToServerRoom();
    void                        onConnectionFailed();

    // Version control related slots.
    void                        on_actionCommit_Changes_triggered();
    void                        onCommitChanges(QString changes);
    
    // Server related slots.
    void                        onSendPathItem(QGraphicsPathItem*);
    void                        onConnectionStopped();
    void                        onDisconnect();
    void                        onSendResize(int, int);
    void                        onSendCrop(int, int, int, int);
    void                        onSendCropWithMagicWand(int, int);

private:
    // Workspace transformation related member functions. 
    void                        resizeGraphicsViewBoundaries(int newWidth, int newHeight);
    void                        reconnectConnection();
    void                        reconstructWorkspaceArea(int imageWidth, int imageHeight);
    void                        handleWheelEvent(QGraphicsSceneWheelEvent* event);
    void                        resetGraphicsViewScale();

    // Menu bar related member functions.
    void                        addRoot(QTreeWidgetItem* parent, QString name);
    void                        customAddChild(QTreeWidgetItem* parent, QWidget* widget);
    void                        createActions();
    void                        createMenus();

    // Version control related member functions.
    void                        generateHistoryMenu();
    void                        checkoutCommit(int masterNodeNumber, int sideNodeNumber);
    void                        commitChanges(QImage changedImage, QString changes);

    // Saving related member functions.
    bool                        maybeSave();
    bool                        saveAsFile(const QByteArray &fileFormat);
    void                        fitImageToScreen(int, int);

    // Server related member functions.
    void                        joinRoom();
    void                        sendInitialImage();  
    void                        destroyConnection();
    void                        sendFilter(QString, int, double);
    void                        sendFilterWithMask(QString, int, double, const QImage&);
    void                        handleFilterBroadcast(QString, int, double);
    void                        handleFilterBroadcast(QString, int, double, const QImage&);

private:
    Ui::MainWindow*             ui;

    WorkspaceArea*              workspaceArea;              //!< Main workspace, where the image lives.
	WorkspaceArea*				temporaryArea = nullptr;    //!< Temporary workspaceArea, used in case a second workspaceArea is needed.
    QGraphicsView*              graphicsView;               //!< Store the workspaceArea(graphicsScene) on a graphicsView.

    VersionControl              imageHistory;               //!< Version Control data structure, commits and merges, image history.
    QVector<QMenu*>             imageHistoryMenu;           //!< Stores the QMenus used for displaying the imageHistory.
    int                         masterNodeNumber = 0;       //!< Saves current checkout master node number (0: latest, 1: previous, etc).
    int                         sideNodeNumber = 0;         //!< Saves current checkout node number in a master node.

    QMenu*                      optionMenu;                 //!< optionMenu is generated during runtime.
    QList<QAction*>             saveAsActs;                 //!< all possible image format that can be used to save the image.
    QAction*                    clearScreenAct;             //!< an action to clear the workspaceArea.

    QTreeWidgetItem*            histogram;                  //!< The parent wrapper of the histogram widget.
    QTreeWidgetItem*            basicControls;              //!< The parent wrapper of the basicControls widget.
    QTreeWidgetItem*            colorControls;              //!< The parent wrapper of the colorControls widget.
    QTreeWidgetItem*            brushControls;              //!< The parent wrapper of the brushControls widget.
    QTreeWidgetItem*            effects;                    //!< The parent wrapper of the effects widget.

    Histogram*                  histo;                      //!< Histogram widget.
    Brush*                      brush;                      //!< Brush widget.
    ColorControls*              colors;                     //!< ColorControls widget.
    BasicControls*              basics;                     //!< BasicControls widget.
    Effects*                    effect;                     //!< Effects widget.

    QString                     fileName = "";              //!< filename of the loaded image.
    QByteArray                  fileFormat;                 //!< the file format of our loaded image.
    bool                        fileSaved = false;          //!< the state that saves whether our current project been saved.
    double                      currentZoom = 1.0;          //!< Saves the current zoom level.
    QComboBox*                  comboBox;                   //!< comboBox to store zoom level options.
    double                      resizedImageHeight = WorkspaceArea::SCENE_HEIGHT;
    double                      resizedImageWidth = WorkspaceArea::SCENE_WIDTH;

    ServerRoom*                 room = nullptr;             //!< ServerRoom instance.
    QString                     username;                   //!< Username name.
    Server*                     server = nullptr;           //!< TODO COMMENT.
    Client*                     client = nullptr;           //!< TODO COMMENT.
    QString                     ip;                         //!< TODO COMMENT.
    quint16                     port;                       //!< TODO COMMENT.
    bool                        isHost = false;             //!< TODO COMMENT.
    bool                        isConnected = false;        //!< TODO COMMENT.
    

};

/**
 * @brief Close Application slot.
 * @details Closes the application when actionExit is triggered.
 */
inline void MainWindow::on_actionExit_triggered()
{
    MainWindow::close();
}

#endif // MAINWINDOW_H
