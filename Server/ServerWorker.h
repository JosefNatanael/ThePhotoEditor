#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include <QTcpSocket>
class QJsonObject;
class ServerWorker : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ServerWorker)
public:
    explicit ServerWorker(QObject *parent = nullptr);
    virtual bool setSocketDescriptor(qintptr socketDescriptor);
    QString getUsername() const;
    void setUsername(const QString &userName);
    void sendJson(const QJsonObject &json);
signals:
    void jsonReceived(const QJsonObject &jsonDoc);
    void disconnectedFromClient();
public slots:
    void disconnectFromClient();
private slots:
    void receiveJson();
private:
    QTcpSocket *serverSocket;
    QString username;
};

#endif // SERVERWORKER_H
