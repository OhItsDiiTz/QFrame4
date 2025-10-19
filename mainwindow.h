#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "qlibframe4.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_RefreshProcesses_clicked();

    void on_pushButton_CheckProcessList_clicked();

    void on_pushButton_ConnectPS4_clicked();

    void on_pushButton_GetProcessInfo_clicked();

    void on_pushButton_AttachProcess_clicked();

    void on_pushButton_TestNotify_clicked();

    void on_pushButton_TestProcMaps_clicked();

    void on_pushButton_TestWrite_clicked();

    void on_pushButton_TestReading_clicked();

    void on_pushButton_TestAllocation_clicked();

    void on_pushButton_TestFreeing_clicked();

private:
    Ui::MainWindow *ui;
    QLibFrame4* frame4;
};
#endif // MAINWINDOW_H
