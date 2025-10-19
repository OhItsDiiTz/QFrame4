#ifndef QLIBFRAME4_H
#define QLIBFRAME4_H

#include <QNetworkAccessManager>
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QUrl>
#include <QJsonArray>
#include <QByteArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QEventLoop>
#include <QJsonDocument>
#include <QmessageBox>

#define SANITY_CHECK(ret) \
if(ip == "") { \
    QMessageBox::information(0, "Error", "You need to set an ip address!"); \
    return ret; \
} \
if(!hasConnected) { \
    QMessageBox::information(0, "Error", "Please connect to the console first!"); \
    return ret; \
} \
if(current_request != "") \
{ \
    QMessageBox::information(0, "Error", "Previous request has not finished"); \
    return ret; \
}

#define SELECTED_PROCESS \
if(selected_pid == -1) \
{ \
    QMessageBox::information(0, "Error", "Please attach to a process first!"); \
    return false;\
}


struct ProcMap {
    QString name;
    quint64 start;
    quint64 end;
    quint64 offset;
    qint32 prot;
};

struct Process {
    QString name;
    qint32 pid;
};

struct ProcessInfo {
    qint32 pid;
    QString name;
    QString path;
    QString title_id;
    QString content_id;
};

class QLibFrame4
{
private:
    QString ip;
    QVector<Process> processes;
    QVector<ProcMap> processMaps;
    qint32 selected_pid;
    bool hasConnected;

    QString current_request = "";

    QString get(const QString &url);
public:
    QLibFrame4();

    //Gets the current process that was attached to
    qint32 getAttachedProcess();

    //Initializes the connection to the console
    bool Connect(QString ipAddress = "");

    //Attaches to a process
    bool AttachProcess(QString processName = "kernel");

    //Gets the process list that was gotten from the last refresh from the console
    QVector<Process> getProcessList();

    //Gets the process maps from the selected process
    QVector<ProcMap> getProcessMaps();

    //Refreshes the process list from the console
    bool refreshProcessList();

    //Gets the process information from the pid
    ProcessInfo getProcessInfo(qint32 pid);

    //Sends a notify to your console
    bool sendNotify(QString message, qint32 messageType = 222);

    //Reads memory from the current process
    bool readMemory(quint64 address, qint32 length, QByteArray & output);

    //Writes memory to the current process
    bool writeMemory(quint64 address, QByteArray input);

    //Allocates memory in the current process
    quint64 allocateMemory(qint32 length);

    //Frees memory in the current process
    bool freeMemory(quint64 address, qint32 length);


};

#endif // QLIBFRAME4_H
