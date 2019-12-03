/**
 * @class Server
 * @brief about Server class
 */
#include "Server.h"
#include "ServerWorker.h"
#include <algorithm>

#include <QNetworkInterface>
#include <QMessageBox>
#include <QtDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

/**
 * @brief  constructor for the Server class
 * @param  parent: MainWindow, to be passed to QTcpServer
 * @details checks if starting server is possible
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

/**
 *  @brief  getter functions for the Server's IP Address
 */
QString Server::getIP() const {return ip;}

/**
 *  @brief  getter functions for the Server's port
 */
quint16 Server::getPort() const {return port;}

/**
 *  @brief  getter functions for the Server's client(s)
 */
QVector<ServerWorker*> Server::getClients() const {return clients;}

/**
 *  @brief  handler for when there is a new incoming client
 *  @param  socketDesriptor the client that is connecting to the server
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

/**
 *  @brief  handler for when the server recieves a QJsonObject from a client
 *  @param  sender the client who sent the QJsonObject
 *  @param  json the QJsonObject recieved
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

    } else if (type == "applyFilter" || type == "applyFilterWithMask" || type == "applyResize" ||
               type == "applyCrop" || type == "applyCropWithMagicWand" || type == "initialImage" ||
               type == "versionControl" || type == "applyMoveScribble" || type == "applyReleaseScribble") {
        broadcast(json, sender);
    }

    emit receiveJson(sender, json);
}

/**
 *  @brief  handler for when a client disconnects
 *  @param  sender client that has disconnected
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

/**
 *  @brief  function for sending a QJsonObject from the server to the client
 *  @param  worker client to be sent to
 *  @param  json QJsonObject to be sent
 */
void Server::sendJson(ServerWorker *worker, const QJsonObject &json) {
    Q_ASSERT(worker);
    worker->sendJson(json);
}

/**
 *  @brief  function for sending a QJsonObject from the server to all the client
 *  @param  json: QJsonObject to be sent
 *  @param  exclude: optional argument for a client to be not recieve the broadcast
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

/**
 *  @brief  function for stopping the server
 */
void Server::stopServer() {
    for (ServerWorker *worker : clients)
        worker->disconnectFromClient();
    close();
}

