#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <qcustomplot.h>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void Connect();
    bool connected;

    QVector<QVector<double>> eeg_all;
    QVector<double> eeg_data;
    QVector<double> sample_no;

    int s_no;

    void PlotData(int ch_no, int p1, int pt, QVector<double> &sample_no, QVector<double> &eeg_data);
    ~MainWindow();

public slots:
    //void newConnection();
    void readTcpData();
private slots:
    void on_pushButton_clicked();

private:
    QTcpServer * server;
    QTcpSocket *mySocket;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
