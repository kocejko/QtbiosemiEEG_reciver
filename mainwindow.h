#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <qcustomplot.h>
#include <QVector>
#include <QFile>

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

    void saveData(QString fname, QString data, bool append);

public slots:
    //void newConnection();
    void readTcpData();
    void onPlotEEGdata(int ch_no, int p1, int pt, QVector<double> &sample_no, QVector<QVector<double>> &eeg_data);

private slots:
    void on_pushButton_clicked();

signals:
    void plotEEGdata(int ch_no, int p1, int pt, QVector<double> &sample_no, QVector<QVector<double> > &eeg_data);

private:
    QTcpServer * server;
    QTcpSocket *mySocket;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
