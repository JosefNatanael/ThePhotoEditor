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
 *  @brief constructor for the ServerWorker object
 *  @param parent Passed to QObject() constructor, instantiate serversocket with "this" QTcpSocket
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
 *  @brief Sets socket descriptor of server socket.
 */
bool ServerWorker::setSocketDescriptor(qintptr socketDescriptor) {
    return serverSocket->setSocketDescriptor(socketDescriptor);
}

/**
 *  @brief signal triggered when disconnected from client. Emits disconnectedFromClient signal, received in Server class
 */
void ServerWorker::disconnectFromClient() {
    serverSocket->disconnectFromHost();
    emit disconnectedFromClient();
}

/**
 *  @brief Receives json and emits jsonReceived signal with parameter the json received.
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
}

/**
 *  @brief setter of the username (name of the user/client)
 *  @param username name of user/client
 */
void ServerWorker::setUsername(const QString &username) {
    this->username = username;
}


/**
 *  @brief  accessor of username(returns the name of the user/client)
 */
QString ServerWorker::getUsername() const {
    return username;
}
