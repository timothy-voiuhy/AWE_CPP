#include "threadrunners.h"
#include <QMainWindow>

SessionHandlerRunner::SessionHandlerRunner(QMainWindow *parent)
{
}

SessionHandlerRunner::~SessionHandlerRunner()
{
}

int SessionHandlerRunner::processId()
{
    return 0;
}

void SessionHandlerRunner::run()
{
}

AtomProxy::AtomProxy(int proxy_port, QMainWindow *parent)
{
}

AtomProxy::~AtomProxy()
{
}

int AtomProxy::processId()
{
    return 0;
}

void AtomProxy::run()
{
}
