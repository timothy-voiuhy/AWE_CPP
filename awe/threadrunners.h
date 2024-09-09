#ifndef THREADRUNNERS_H
#define THREADRUNNERS_H

#include <QThread>
#include <QMainWindow>

class AtomProxy : public QThread{
    Q_OBJECT
    public:
        explicit AtomProxy(int proxy_port, QMainWindow *parent);
        ~AtomProxy();
        int processId();

    protected:
        void run() override;
};

class SessionHandlerRunner : public QThread{
    Q_OBJECT
    public:
        explicit SessionHandlerRunner(QMainWindow *parent);
        ~SessionHandlerRunner();
        int processId();

    protected:
        void run() override;
};

#endif
