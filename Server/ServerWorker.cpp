/**
 * @class ServerWorker
 * @brief about ServerWorker class
 */
#include "ServerWorker.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

/**
 *  @brief  constructor for the ServerWorker object
 *  @param  parent Server, also to be passed to serverSocket
 */
ServerWorker::ServerWorker(QObject *parent):
    QObject(parent),
    serverSocket(new QTcpSocket(this))
{
    qDebug("New ServerWorker");
    connect(serverSocket, &QTcpSocket::readyRead, this, &ServerWorker::receiveJson);
    connect(serverSocket, &QTcpSocket::disconnected, this, &ServerWorker::disconnectFromClient);
}

/**
 *  @brief  setter functions for ServerWorker's Socket Descriptor
 */
bool ServerWorker::setSocketDescriptor(qintptr socketDescriptor) {
    return serverSocket->setSocketDescriptor(socketDescriptor);
}

/**
 *  @brief  signal triggered when disconnected from client
 */
void ServerWorker::disconnectFromClient() {
    serverSocket->disconnectFromHost();
    emit disconnectedFromClient();
}

/**
 *  @brief  triggered when recieving a Json object
 */
void ServerWorker::receiveJson() {
    qDebug("ServerWorker receive Json");
    QByteArray jsonData;
    QDataStream socketStream(serverSocket);
    socketStream.setVersion(QDataStream::Qt_5_11);
    while(true) {
        socketStream.startTransaction();
        socketStream >> jsonData;
        if (socketStream.commitTransaction()) {
            QJsonParseError parseError;
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError) {
                if (jsonDoc.isObject())
                    emit jsonReceived(jsonDoc.object());
                else
                    qDebug("Invalid message");
            } else {
                qDebug("Invalid message");
            }
        } else {
            break;
        }
    }
}

/**
 *  @brief  for sending Json objects
 *  @param  json the Json object to be sent
 */
void ServerWorker::sendJson(const QJsonObject &json) {
    QDataStream socketStream(serverSocket);
    socketStream.setVersion(QDataStream::Qt_5_11);
    socketStream << QJsonDocument(json).toJson(QJsonDocument::Compact);
    qDebug() << "on sendJson";
}

/**
 *  @brief setter of the playerName (name of the player/client)
 *  @param playerName name of player/client
 */
void ServerWorker::setPlayerName(const QString &playerName) {
    this->playerName = playerName;
}


/**
 *  @brief  accessor of playerName(returns the name of the player/client)
 */
QString ServerWorker::getPlayerName() const {
    return playerName;
}
