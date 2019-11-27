#include "serverroom.h"
#include "ui_serverroom.h"

ServerRoom::ServerRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerRoom)
{
    ui->setupUi(this);
    ui->playerList->hide();
}

ServerRoom::~ServerRoom()
{
    delete ui;
}

void ServerRoom::setCreateRoom() {
    ui->addressLine->hide();
    ui->addressLabel->hide();
    ui->portLine->hide();
    ui->portLabel->hide();
    ui->joinRoomButtonBox->hide();
}

void ServerRoom::setJoinRoom() {
    ui->createRoomButtonBox->hide();
}

void ServerRoom::setServerRoom() {
    ui->nameLine->hide();
    ui->nameLabel->hide();
    ui->addressLine->hide();
    ui->addressLabel->hide();
    ui->portLine->hide();
    ui->portLabel->hide();
    ui->joinRoomButtonBox->hide();
    ui->createRoomButtonBox->hide();
    ui->playerList->show();
}

void ServerRoom::on_joinRoomButtonBox_accepted()
{
    QString name = ui->nameLine->text().simplified();
    QString address = ui->addressLine->text().simplified();
    quint16 port = ui->portLine->text().simplified().toUShort();
    emit joinRoom(name, address, port);
}

void ServerRoom::on_createRoomButtonBox_accepted()
{
    QString name = ui->nameLine->text().simplified();
    emit createRoom(name);
}

void ServerRoom::addPlayer(QString name) {
    ui->playerList->addItem(name);
}
