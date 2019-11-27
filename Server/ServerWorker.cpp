#include "serverworker.h"
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

/*
 *  ServerWorker::ServerWorker(QObject *parent)
 *  @funct:  constructor for the ServerWorker object
 *  @param:  parent: Server
 *  @return: N/A
 */
ServerWorker::ServerWorker(QObject *parent):
    QObject(parent),
    serverSocket(new QTcpSocket(this))
{
    qDebug("New ServerWorker");
    connect(serverSocket, &QTcpSocket::readyRead, this, &ServerWorker::receiveJson);
    connect(serverSocket, &QTcpSocket::disconnected, this, &ServerWorker::disconnectFromClient);
}

/*
 *  ServerWorker::set[...]()
 *  @funct:  setter functions for ServerWorker
 */
bool ServerWorker::setSocketDescriptor(qintptr socketDescriptor) {
    return serverSocket->setSocketDescriptor(socketDescriptor);
}

/*
 *  QString ServerWorker::getPlayerName()
 *  @funct:  returns the name of the player/client
 *  @param:  N/A
 *  @return: N/A
 */
/*
 *  void ServerWorker::disconnectFromClient()
 *  @funct:  signal triggered when disconnected from client
 *  @param:  N/A
 *  @return: N/A
 */
void ServerWorker::disconnectFromClient() {
    serverSocket->disconnectFromHost();
    emit disconnectedFromClient();
}

/*
 *  void ServerWorker::receiveJson()
 *  @funct:  triggerd when recieving a Json object
 *  @param:  N/A
 *  @return: N/A
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

/*
 *  void ServerWorker::sendJson(const QJsonObject &json)
 *  @funct:  for sending Json objects
 *  @param:  json: the Json object to be sent
 *  @return: N/A
 */
void ServerWorker::sendJson(const QJsonObject &json) {
    QDataStream socketStream(serverSocket);
    socketStream.setVersion(QDataStream::Qt_5_11);
    socketStream << QJsonDocument(json).toJson(QJsonDocument::Compact);
}

void ServerWorker::setPlayerName(const QString &playerName) {
    this->playerName = playerName;
}


/*
 *  QString ServerWorker::getPlayerName()
 *  @funct:  returns the name of the player/client
 *  @param:  N/A
 *  @return: N/A
 */
QString ServerWorker::getPlayerName() const {
    return playerName;
}
