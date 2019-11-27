#ifndef SERVERROOM_H
#define SERVERROOM_H

#include <QDialog>

namespace Ui {
class ServerRoom;
}

class ServerRoom : public QDialog
{
    Q_OBJECT

    friend class MainWindow;

public:
    explicit ServerRoom(QWidget *parent = nullptr);
    ~ServerRoom();

    void setCreateRoom();
    void setJoinRoom();
    void setServerRoom();
    void addPlayer(QString);

signals:
    void createRoom(QString);
    void joinRoom(QString, QString, quint16);

private slots:
    void on_joinRoomButtonBox_accepted();

    void on_createRoomButtonBox_accepted();

private:
    Ui::ServerRoom *ui;
};

#endif // SERVERROOM_H
