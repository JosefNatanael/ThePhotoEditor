#include "ServerRoom.h"
#include "ui_serverroom.h"

ServerRoom::ServerRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerRoom)
{
    ui->setupUi(this);
    ui->playerList->hide();
    ui->roomIpLabel->hide();
    ui->roomPortLabel->hide();
    ui->disconnectButton->hide();
    ui->collaboratorsLabel->hide();
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
    ui->titleLabel->setText("Create Room");
}

void ServerRoom::setJoinRoom() {
    ui->createRoomButtonBox->hide();
    ui->titleLabel->setText("Join Room");
    ui->nameLine->show();
    ui->nameLabel->show();
    ui->addressLine->show();
    ui->addressLabel->show();
    ui->portLine->show();
    ui->portLabel->show();
    ui->joinRoomButtonBox->show();

    ui->playerList->hide();
    ui->roomIpLabel->hide();
    ui->ipContent->hide();
    ui->roomPortLabel->hide();
    ui->portContent->hide();
    ui->disconnectButton->hide();
    ui->collaboratorsLabel->hide();
}

void ServerRoom::setServerRoom(QString ip, quint16 port) {
    ui->nameLine->hide();
    ui->nameLabel->hide();
    ui->addressLine->hide();
    ui->addressLabel->hide();
    ui->portLine->hide();
    ui->portLabel->hide();
    ui->joinRoomButtonBox->hide();
    ui->createRoomButtonBox->hide();
    ui->playerList->show();
    ui->roomIpLabel->show();
    ui->roomPortLabel->show();
    ui->ipContent->show();
    ui->ipContent->setText(ip);
    ui->portContent->show();
    ui->portContent->setNum(port);
    ui->disconnectButton->show();
    ui->collaboratorsLabel->show();
    ui->titleLabel->setText("The Room");
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

void ServerRoom::emptyPlayers() {
    ui->playerList->clear();
}

void ServerRoom::on_joinRoomButtonBox_rejected()
{
    close();
}

void ServerRoom::on_createRoomButtonBox_rejected()
{
    close();
}

void ServerRoom::on_disconnectButton_clicked()
{
    emit disconnect();
}
