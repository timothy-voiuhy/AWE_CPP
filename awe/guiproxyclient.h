#ifndef GUIPROXYCLIENT_H
#define GUIPROXYCLIENT_H

#include <QThread>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QString>
#include <QFile>
#include <QDir>

class GuiProxyClient : public QThread
{
    Q_OBJECT

public:
    GuiProxyClient(const QString& request, bool isCommand = false, quint16 proxyPort = 0, QObject* parent = nullptr);
    ~GuiProxyClient();

protected:
    void run() override;

private:
    QString makeRequestPacket(const QString& request);
    void send();

    bool isCommand;
    QString responseDir;
    QString responseFile;
    QString request;
    quint16 proxyPort;
    QHostAddress proxyAddress;
    QTcpSocket* socket;
};

#endif // GUIPROXYCLIENT_H
