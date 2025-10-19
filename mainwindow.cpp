#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    frame4 = new QLibFrame4();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_RefreshProcesses_clicked()
{
    frame4->refreshProcessList();
}

void MainWindow::on_pushButton_CheckProcessList_clicked()
{
    for(Process proc : frame4->getProcessList()) {
        qDebug() << proc.name << " " << proc.pid;
    }
}





void MainWindow::on_pushButton_ConnectPS4_clicked()
{
    if(frame4->Connect(ui->lineEdit_ipAddress->displayText())) {
        QMessageBox::information(this, "Success", "Connected to the console with success!");
    }
}


void MainWindow::on_pushButton_GetProcessInfo_clicked()
{
    ProcessInfo procInfo = frame4->getProcessInfo(frame4->getAttachedProcess());
    if(procInfo.path == "")
        return;


    qDebug() << procInfo.path;
}


void MainWindow::on_pushButton_AttachProcess_clicked()
{
    if(frame4->AttachProcess(ui->lineEdit_processName->displayText())) {
        ProcessInfo procInfo = frame4->getProcessInfo(frame4->getAttachedProcess());
        QMessageBox::information(0, "Success", QString("Attached to %1 at %2 with success!").arg(procInfo.name).arg(procInfo.path));
    }
}


void MainWindow::on_pushButton_TestNotify_clicked()
{
    if(frame4->sendNotify(ui->lineEdit_NotifyText->displayText())) {
        QMessageBox::information(0, "Success", QString("Notify '%1' sent to console").arg(ui->lineEdit_NotifyText->displayText()));
    }
}


void MainWindow::on_pushButton_TestProcMaps_clicked()
{
    QVector<ProcMap> pMaps = frame4->getProcessMaps();

    for(ProcMap pmap : pMaps) {
        qDebug("%s - %p - %p - %p - %i", pmap.name.toStdString().c_str(), pmap.start, pmap.end, pmap.offset, pmap.prot);
    }
}


void MainWindow::on_pushButton_TestWrite_clicked()
{
    QByteArray bytes = { "\x01\x02\x03\x04\x05\x06\x07" };
    if(frame4->writeMemory(0x2050EC000, bytes)) {
        QMessageBox::information(0, "Success", "Write a silly string successfully");
    }
}


void MainWindow::on_pushButton_TestReading_clicked()
{
    QByteArray bytes = {};
    if(frame4->readMemory(0x2050EC000, 10, bytes)) {
        QMessageBox::information(0, "Success", QString("Read '%1' bytes").arg(bytes.toHex(' ').toUpper()));
    }
}


void MainWindow::on_pushButton_TestAllocation_clicked()
{
    quint64 address = frame4->allocateMemory(0x100);
    if(address != 0) {
        QMessageBox::information(0, "Success", QString("%1").arg(address, 16, 16, QLatin1Char('0')));
    }
}


void MainWindow::on_pushButton_TestFreeing_clicked()
{
    if(frame4->freeMemory(0xC000, 0x100)) {
        QMessageBox::information(0, "Success", "Memory at 0xC000 has been freed!");
    }
}

