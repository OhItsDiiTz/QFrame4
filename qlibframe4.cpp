#include "qlibframe4.h"

QLibFrame4::QLibFrame4() {
    processes = QVector<Process>();
}


QString QLibFrame4::get(const QString &url)
{
    QNetworkAccessManager manager;
    QUrl baseUrl = QUrl(url);
    QNetworkRequest request(baseUrl);

    QNetworkReply *reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response = QString::fromUtf8(reply->readAll());
    reply->deleteLater();
    return response;
}

qint32 QLibFrame4::getAttachedProcess() {
    return selected_pid;
}

bool QLibFrame4::Connect(QString ipAddress) {
    ip = ipAddress;
    current_request = QString("http://%1:2812/process-list").arg(ip);
    QString response = get(current_request);
    current_request = "";
    if(response == "")
        return false;

    refreshProcessList();

    hasConnected = true;
    return true;
}

bool QLibFrame4::AttachProcess(QString processName) {
    for(Process proc : getProcessList()) {
        if(proc.name.toLower() == processName.toLower()) {
            selected_pid = proc.pid;
            return true;
        }
    }
    return false;
}

QVector<Process> QLibFrame4::getProcessList() {
    SANITY_CHECK(QVector<Process>())
    return processes;
}

QVector<ProcMap> QLibFrame4::getProcessMaps() {
    SANITY_CHECK(QVector<ProcMap>())

    processMaps.clear();
    current_request = QString("http://%1:2812/process-maps?pid=%2").arg(ip).arg(getAttachedProcess());
    QString response = get(current_request);
    current_request = "";

    if(response == "")
        return QVector<ProcMap>();

    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonArray root = doc.array();
    processes.reserve(root.size());
    bool test[4] = {false};
    for (const QJsonValue &v : root) {
        QJsonObject o = v.toObject();
        ProcMap p{
            o.value("name").toString(),
            o.value("start").toString().toULongLong(&test[0], 16),
            o.value("end").toString().toULongLong(&test[1], 16),
            o.value("offset").toString().toULongLong(&test[2], 16),
            o.value("prot").toString().toInt(&test[3], 16)
        };
        processMaps.push_back(p);
    }

    return processMaps;
}

bool QLibFrame4::refreshProcessList() {
    SANITY_CHECK(false)

    processes.clear();

    current_request = QString("http://%1:2812/process-list").arg(ip);
    QString response = get(current_request);
    current_request = "";

    if(response == "")
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonArray root = doc.array();
    processes.reserve(root.size());

    for (const QJsonValue &v : root) {
        QJsonObject o = v.toObject();
        Process p{
            o.value("name").toString(),
            o.value("pid").toInt()
        };
        processes.push_back(p);
    }


    return true;
}


ProcessInfo QLibFrame4::getProcessInfo(qint32 pid) {
    SANITY_CHECK(ProcessInfo())

    ProcessInfo procInfo;

    current_request = QString("http://%1:2812/process-info?pid=%2").arg(ip).arg(pid);
    QString response = get(current_request);
    current_request = "";

    if(response == "")
        return ProcessInfo{};

    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject root = doc.object();

    procInfo.pid = root.value("pid").toInt();
    procInfo.name = root.value("name").toString();
    procInfo.path = root.value("path").toString();
    procInfo.title_id = root.value("title_id").toString();
    procInfo.content_id = root.value("content_id").toString();
    return procInfo;
}

bool QLibFrame4::sendNotify(QString message, qint32 messageType) {
    SANITY_CHECK(false)



    current_request = QString("http://%1:2812/notify?messageType=%2&message=%3\0").arg(ip).arg(messageType).arg(message.toUtf8().toBase64(QByteArray::Base64UrlEncoding));
    QString response = get(current_request);
    current_request = "";

    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject root = doc.object();

    return root.value("success").toBool();
}

bool QLibFrame4::readMemory(quint64 address, qint32 length, QByteArray & output) {
    SANITY_CHECK(false)
    SELECTED_PROCESS;

    current_request = QString("http://%1:2812/read-memory?pid=%2&address=%3&length=%4").arg(ip).arg(getAttachedProcess()).arg(address, 16, 16, QLatin1Char('0')).arg(length);
    QString response = get(current_request);
    current_request = "";
    if(response == "")
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject root = doc.object();


    output = QByteArray::fromHex(root.value("bytes").toString().remove(' ').toLatin1());

    return true;
}

bool QLibFrame4::writeMemory(quint64 address, QByteArray input) {
    SANITY_CHECK(false)
    SELECTED_PROCESS;

    current_request = QString("http://%1:2812/write-memory?pid=%2&address=%3&bytes=%4").arg(ip).arg(getAttachedProcess()).arg(address, 16, 16, QLatin1Char('0')).arg(input.toHex());
    QString response = get(current_request);
    current_request = "";
    if(response == "")
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject root = doc.object();

    return root.value("success").toBool();
}

quint64 QLibFrame4::allocateMemory(qint32 length) {
    SANITY_CHECK(0)
    SELECTED_PROCESS;

    current_request = QString("http://%1:2812/allocate-memory?pid=%2&length=%3").arg(ip).arg(getAttachedProcess()).arg(length);
    QString response = get(current_request);
    current_request = "";
    if(response == "")
        return 0;

    bool test;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject root = doc.object();

    if(!root.value("success").toBool()) {
        return 0;
    }

    return root.value("allocated_address").toString().toULongLong(&test, 16);
}

bool QLibFrame4::freeMemory(quint64 address, qint32 length) {
    SANITY_CHECK(false)
    SELECTED_PROCESS;

    current_request = QString("http://%1:2812/free-memory?pid=%2&address=%3&length=%4").arg(ip).arg(getAttachedProcess()).arg(address, 16, 16, QLatin1Char('0')).arg(length);
    QString response = get(current_request);
    current_request = "";
    if(response == "")
        return false;


    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    QJsonObject root = doc.object();

    return root.value("success").toBool();
}


