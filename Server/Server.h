#ifndef SERVER_H
#define SERVER_H


#include <QTcpServer>
#include <QVector>

#include "ServerWorker.h"

class Server : public QTcpServer
{
    Q_OBJECT
    Q_DISABLE_COPY(Server)
public:
    explicit Server(QObject *parent = nullptr);
    QString getIP() const;
    quint16 getPort() const;
    QVector<ServerWorker*> getClients() const;

protected:
    void incomingConnection(qintptr socketDescriptor) override;
signals:
    void receiveJson(ServerWorker *sender, const QJsonObject &json);
    void newUserConnected();
public slots:
    void stopServer();
    void broadcast(const QJsonObject &json, ServerWorker *exclude = nullptr);
private slots:
    void jsonReceived(ServerWorker *sender, const QJsonObject &json);
    void userDisconnected(ServerWorker *sender);
private:

    quint16 port;
    QString ip;
    QVector<ServerWorker*> clients;
    void sendJson(ServerWorker *worker, const QJsonObject &json);
};

#endif // SERVER_H
