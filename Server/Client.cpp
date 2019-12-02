/**
 * @class Client
 * @brief about Client class
 */
#include "Server/Client.h"
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDataStream>
#include <QNetworkConfiguration>
#include <QTime>

/**
 * @brief constructor for the Client::Client object
 * @param  parent: MainWindow
 *
 * @details initialize connections
 */
Client::Client(QObject *parent):
    QObject(parent),
    clientSocket(new QTcpSocket(this))
{
    connect(clientSocket, &QTcpSocket::connected, this, &Client::connected);
    connect(clientSocket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(clientSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::onError);
}

/**
 *  @brief Client::connectToServer(const QHostAddress &address, quint16 port)
 *  @param address: adderss of the server,
 *  @param port port of the server
 *
 *  @details  connects the client to the server
 */
void Client::connectToServer(const QHostAddress &address, quint16 port) {
    QTime timer;
    timer.start();
    clientSocket->connectToHost(address, port);

    if (clientSocket->waitForConnected(5000)) {
        qDebug() << "connected in" << timer.elapsed();
        //emit connected();
    } else {

        qDebug() << "not connected in" << timer.elapsed();
        emit connectionFailedBad();
    }
}

/**
 *  @brief Disconnect client from host
 *  @details  disconnect the client from the host
 */
void Client::disconnectFromHost() {
    qDebug() << "Client disconnected GG";
    clientSocket->disconnectFromHost();
}

/**
 *  @brief  sends the Json from the client to the server
 *  @param  json QJsonObject to be sent
 */
void Client::sendJson(const QJsonObject &json) {
    QDataStream clientStream(clientSocket);
    clientStream.setVersion(QDataStream::Qt_5_11);
    clientStream << QJsonDocument(json).toJson(QJsonDocument::Compact);
}

/**
 *  @brief:  slot triggered when Json object has been recieved
 *  @param:  json: QJsonObject recieved
 */
void Client::jsonReceived(const QJsonObject &json) {
    emit receiveJson(json);
}

/**
 *  @brief:  for receiving the Json object from the server
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

/**
 * @brief Connection stopped if error happens
 * @param error Error happened
 *
 * @details emit connectionStopped signal
 */
void Client::onError(QAbstractSocket::SocketError error) {

    if (!(clientSocket->state() == QTcpSocket::ConnectedState)) {
        emit connectionStopped();
        qDebug() << "NOT CONNECTED";
    }
}
