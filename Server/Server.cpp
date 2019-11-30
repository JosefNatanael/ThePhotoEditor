#include "server.h"
#include "serverworker.h"
#include <algorithm>

#include <QNetworkInterface>
#include <QMessageBox>
#include <QtDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

/*
 *  Server::Server(QObject *parent)
 *  @funct:  constructor for the Server class
 *  @param:  parent: MainWindow
 *  @return: N/A
 */
Server::Server(QObject *parent):
    QTcpServer(parent)
{
    if (!listen()) {
        qDebug("Unable to start the server");
        close();
        return;
    }
    port = serverPort();

    // Find a usable IP address
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ip = ipAddressesList.at(i).toString();
            break;
        }
    }

    if (ip.isEmpty())
        ip = QHostAddress(QHostAddress::LocalHost).toString();
}

/*
 *  Server::get[...]
 *  @funct:  getter functions for the Server object
 */
QString Server::getIP() const {return ip;}
quint16 Server::getPort() const {return port;}
QVector<ServerWorker*> Server::getClients() const {return clients;}

/*
 *  void Server::incomingConnection(qintptr socketDesriptor)
 *  @funct:  handler for when there is a new incoming client
 *  @param:  socketDesriptor: the client that is connecting to the server
 *  @return: N/A
 */
void Server::incomingConnection(qintptr socketDesriptor) {
    qDebug("New Player");
    ServerWorker *worker = new ServerWorker(this);
    if (!worker->setSocketDescriptor(socketDesriptor)) {
        worker->deleteLater();
        return;
    }

    qDebug("Send player list");
    clients.append(worker);
    connect(worker, &ServerWorker::disconnectedFromClient, this, std::bind(&Server::userDisconnected, this, worker));
    connect(worker, &ServerWorker::jsonReceived, this, std::bind(&Server::jsonReceived, this, worker, std::placeholders::_1));

    qDebug() << "new player connected";
    emit newPlayerConnected();
}

/*
 *  void Server::jsonReceived(ServerWorker *sender, const QJsonObject &json)
 *  @funct:  handler for when the server recieves a QJsonObject from a client
 *  @param:  sender: the client who sent the QJsonObject, json: the QJsonObject recieved
 *  @return: N/A
 */
void Server::jsonReceived(ServerWorker *sender, const QJsonObject &json) {
    Q_ASSERT(sender);

    qDebug("Server Receive a Client Json");
    const QString type = json.value(QString("type")).toString();
    if (type == "playerName") {
        const QString playerName = json.value("playerName").toString();

        qDebug() << clients.size();

        if (clients.size() > 2) {
            QJsonObject playerFullMsg;
            playerFullMsg["type"] = "playerFull";
            sendJson(sender, playerFullMsg);
            fullList.append(sender);
            clients.removeAll(sender);
            //sender->deleteLater();
            return;
        }


        // If the player has the same name with existing player, server won't allow the player to enter the room
        for (ServerWorker *worker : clients) {
            if (worker == sender)
                continue;
            if (worker->getPlayerName() == playerName) {
                QJsonObject playerRepeatNameMsg;
                playerRepeatNameMsg["type"] = "nameRepeat";
                sendJson(sender, playerRepeatNameMsg);
                fullList.append(sender);
                clients.removeAll(sender);
                //worker->deleteLater();
                return;
            }
        }

        sender->setPlayerName(playerName);
        QJsonObject playerNamesMsg;
        QJsonArray playerNames;
        for (ServerWorker *worker : clients)
            playerNames.append(worker->getPlayerName());
        playerNamesMsg["type"] = "playerList";
        playerNamesMsg["playerNames"] = playerNames;
        broadcast(playerNamesMsg);

        //sendJson(worker, playerNamesMsg);
//        QJsonObject newPlayerMsg;
//        newPlayerMsg["type"] = "newPlayer";
//        newPlayerMsg["playerName"] = playerName;
//        broadcast(newPlayerMsg);
    } else if (type == "applyFilter" || type == "applyResize" || type == "applyCrop") {
        broadcast(json, sender);
    }

    emit receiveJson(sender, json);
}

/*
 *  void Server::startGameBroadcast()
 *  @funct:  function to tell the clients that the game has been started by the host
 *  @param:  N/A
 *  @return: N/A
 */
void Server::startGameBroadcast() {
//    if (clients.size()>2){
//        QMessageBox::information(nullptr, QString("Too Many Players"), QString("There are too many players."));
//        return;
//    }
//    if (clients.size()==1){
//        QMessageBox::information(nullptr, QString("Not Enough Players"), QString("You need 2-5 players to play."));
//        return;
//    }
//    qDebug("Server Start Game Broadcast");
//    QJsonObject startGameMsg;
//    startGameMsg["type"] = "startGame";
//    broadcast(startGameMsg);
}

/*
 *  void Server::userDisconnected(ServerWorker *sender)
 *  @funct:  handler for when a client disconnects
 *  @param:  worker: client that has disconnected
 *  @return: N/A
 */
void Server::userDisconnected(ServerWorker *sender) {
    qDebug("user disconnect");
//    if (std::find(clients.begin(), clients.end(), sender) == clients.end()) {
//        fullList.removeAll(sender);
//        return;
//    }
    clients.removeAll(sender);
    const QString player = sender->getPlayerName();
    if (!player.isEmpty()) {
//        QJsonObject disconnectedMessage;
//        disconnectedMessage["type"] = QString("playerDisconnected");
//        disconnectedMessage["playerName"] = player;
//        broadcast(disconnectedMessage, nullptr);

        QJsonObject playerNamesMsg;
        QJsonArray playerNames;
        for (ServerWorker *worker : clients)
            playerNames.append(worker->getPlayerName());
        playerNamesMsg["type"] = "playerList";
        playerNamesMsg["playerNames"] = playerNames;
        broadcast(playerNamesMsg);
    }
    sender->deleteLater();
}

/*
 *  void Server::sendJson(ServerWorker *worker, const QJsonObject &json)
 *  @funct:  function for sending a QJsonObject from the server to the client
 *  @param:  worker: client to be sent to, json: QJsonObject to be sent
 *  @return: N/A
 */
void Server::sendJson(ServerWorker *worker, const QJsonObject &json) {
    Q_ASSERT(worker);
    worker->sendJson(json);
}

/*
 *  void Server::broadcast(const QJsonObject &json, ServerWorker *exclude)
 *  @funct:  function for sending a QJsonObject from the server to all the client
 *  @param:  json: QJsonObject to be sent, exclude: optional argument for a client to be not recieve the broadcast
 *  @return: N/A
 */
void Server::broadcast(const QJsonObject &json, ServerWorker *exclude) {
    qDebug("Broadcast Message");
    for (ServerWorker *worker : clients) {
        Q_ASSERT(worker);
        if (worker == exclude)
            continue;
        sendJson(worker, json);
    }
}

/*
 *  void Server::stopServer()
 *  @funct:  function for stopping the server
 *  @param:  N/A
 *  @return: N/A
 */
void Server::stopServer() {
    QJsonObject hostDisconnected;
    hostDisconnected["type"] = "hostDisconnected";
    broadcast(hostDisconnected);
    for (ServerWorker *worker : clients)
        worker->disconnectFromClient();
    close();
}

void Server::sendInitialImage(const QJsonObject& json) {
    for (ServerWorker *worker : clients) {
        Q_ASSERT(worker);
        qDebug() << "on sendInitialImage";
        sendJson(worker, json);
    }
}
