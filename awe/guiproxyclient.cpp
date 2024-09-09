#include "guiproxyclient.h"
#include <QDebug>
#include <QException>
#include <QDir>

GuiProxyClient::GuiProxyClient(const QString& request, bool isCommand, quint16 proxyPort, QObject* parent)
    : QThread(parent), isCommand(isCommand), proxyPort(proxyPort), request(makeRequestPacket(request))
{
    setObjectName("GuiProxyClient");
    responseDir = QDir::homePath() + "/tmp/";
    responseFile = responseDir + "response.txt";

    proxyAddress = QHostAddress("127.0.0.1");

    socket = new QTcpSocket(this);
    socket->connectToHost(proxyAddress, proxyPort);

    if (!socket->waitForConnected(10000)) {
        qWarning() << "Connection error:" << socket->errorString();
        exit();
    }
}

GuiProxyClient::~GuiProxyClient()
{
}

QString GuiProxyClient::makeRequestPacket(const QString& request)
{
    QStringList requestLines = request.split("\n");
    QString newRequest;
    for (int i = 0; i < requestLines.size(); ++i) {
        if (i == 0) {
            newRequest = requestLines.at(i);
        } else {
            newRequest += "\r\n" + requestLines.at(i);
        }
    }
    return newRequest;
}

void GuiProxyClient::send()
{
    try {
        socket->write(request.toUtf8());
        socket->flush();
        if (!isCommand) {
            if (socket->waitForReadyRead()) {
                QByteArray response = socket->readAll();
                QDir dir(responseDir);
                if (!dir.exists()) {
                    dir.mkpath(responseDir);
                }
                QFile file(responseFile);
                if (file.open(QIODevice::WriteOnly)) {
                    file.write(response);
                    file.close();
                }
            }
        }
    } catch (const QException& e) {
        qWarning() << "Encountered error:" << e.what();
    }
    exit();
}

void GuiProxyClient::run()
{
    send();
}
