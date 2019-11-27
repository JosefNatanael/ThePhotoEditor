#include "client.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDataStream>

/*
 *  Client::Client(QObject *parent)
 *  @funct:  constructor for the Client object
 *  @param:  parent: MainWindow
 *  @return: N/A
 */
Client::Client(QObject *parent):
    QObject(parent),
    clientSocket(new QTcpSocket(this))
{
    connect(clientSocket, &QTcpSocket::connected, this, &Client::connected);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
}

/*
 *  void Client::connectToServer(const QHostAddress &address, quint16 port)
 *  @funct:  connects the client to the server
 *  @param:  address: adderss of the server, port: port of the server
 *  @return: N/A
 */
void Client::connectToServer(const QHostAddress &address, quint16 port) {
    clientSocket->connectToHost(address, port);
}

/*
 *  void Client::disconnectFromHost()
 *  @funct:  disconnect the client from the host
 *  @param:  N/A
 *  @return: N/A
 */
void Client::disconnectFromHost() {
    qDebug("client Disconnect");
    clientSocket->disconnectFromHost();
}

/*
 *  void Client::sendJson(const QJsonObject &json)
 *  @funct:  sends the Json from the client to the server
 *  @param:  json: QJsonObject to be sent
 *  @return: N/A
 */
void Client::sendJson(const QJsonObject &json) {
    QDataStream clientStream(clientSocket);
    clientStream.setVersion(QDataStream::Qt_5_11);
    clientStream << QJsonDocument(json).toJson(QJsonDocument::Compact);
}

/*
 *  void Client::jsonReceived(const QJsonObject &json)
 *  @funct:  slot triggered when Json object has been recieved
 *  @param:  json: QJsonObject recieved
 *  @return: N/A
 */
void Client::jsonReceived(const QJsonObject &json) {
    emit receiveJson(json);
}

/*
 *  void Client::onReadyRead()
 *  @funct:  for receiving the Json object from the server
 *  @param:  N/A
 *  @return: N/A
 */
void Client::onReadyRead() {
    qDebug("Client ready read");
    QByteArray jsonData;
    QDataStream socketStream(clientSocket);
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
