/**
 * @class ServerRoom
 * @brief about the ServerRoom class
 */

#include "ServerRoom.h"
#include "ui_serverroom.h"

/**
 * @brief Constructor for ServerRoom::ServerRoom
 * @param parent Passed to QDialog constructor.
 */
ServerRoom::ServerRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerRoom)
{
    ui->setupUi(this);
    ui->userList->hide();
    ui->roomIpLabel->hide();
    ui->roomPortLabel->hide();
    ui->disconnectButton->hide();
    ui->collaboratorsLabel->hide();
}

/**
 * @brief Destructor for ServerRoom::~ServerRoom
 */
ServerRoom::~ServerRoom()
{
    delete ui;
}

/**
 * @brief set-up for Create Room
 */
void ServerRoom::setCreateRoom() {
    ui->addressLine->hide();
    ui->addressLabel->hide();
    ui->portLine->hide();
    ui->portLabel->hide();
    ui->joinRoomButtonBox->hide();
    ui->titleLabel->setText("Create Room");
}

/**
 * @brief set-up for Join Room
 */
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

    ui->userList->hide();
    ui->roomIpLabel->hide();
    ui->ipContent->hide();
    ui->roomPortLabel->hide();
    ui->portContent->hide();
    ui->disconnectButton->hide();
    ui->collaboratorsLabel->hide();
}

/**
 * @brief set-up for Server Room
 * @param ip IP Address
 * @param port Port
 */
void ServerRoom::setServerRoom(QString ip, quint16 port) {
    ui->nameLine->hide();
    ui->nameLabel->hide();
    ui->addressLine->hide();
    ui->addressLabel->hide();
    ui->portLine->hide();
    ui->portLabel->hide();
    ui->joinRoomButtonBox->hide();
    ui->createRoomButtonBox->hide();
    ui->userList->show();
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

/**
 * @brief Handling join room
 * @details emit joinRoom signal with name, address, and port
 */
void ServerRoom::on_joinRoomButtonBox_accepted()
{
    QString name = ui->nameLine->text().simplified();
    QString address = ui->addressLine->text().simplified();
    quint16 port = ui->portLine->text().simplified().toUShort();
    emit joinRoom(name, address, port);
}

/**
 * @brief Handling create room
 * @details emit createRoom signal with name (slot at MainWindow)
 */
void ServerRoom::on_createRoomButtonBox_accepted()
{
    QString name = ui->nameLine->text().simplified();
    emit createRoom(name);
}

/**
 * @brief adds User to user list.
 */
void ServerRoom::addUser(QString name) {
    ui->userList->addItem(name);
}

/**
 * @brief clears all users from the user list.
 */
void ServerRoom::emptyUsers() {
    ui->userList->clear();
}

/**
 * @brief close if joinRoomButtonBox is rejected
 */
void ServerRoom::on_joinRoomButtonBox_rejected()
{
    close();
}

/**
 * @brief close if createRoomButtonBox is rejected
 */
void ServerRoom::on_createRoomButtonBox_rejected()
{
    close();
}

/**
 * @brief If disconnectButton is clicked, emit disconnect signal (slot at MainWindow)
 */
void ServerRoom::on_disconnectButton_clicked()
{
    emit disconnect();
}
