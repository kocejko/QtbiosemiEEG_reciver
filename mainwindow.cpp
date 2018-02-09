#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i=0;i<ui->spinBox_2->value();i++)
    {
        ui->widget->addGraph();
    }
    s_no = 0;
}

void MainWindow::Connect()
{
    QByteArray data; // <-- fill with data

    mySocket = new QTcpSocket(this);
    //data_counter = 0;
    connect(mySocket, SIGNAL(readyRead()), this, SLOT(readTcpData()));

    QString ip_add = ui->lineEdit->text();
    int port = ui->spinBox->value();

    mySocket->connectToHost(ip_add, port);


    if(mySocket->waitForConnected())
    {
        qDebug()<<"writing data";
        mySocket->write(data);

    }
    else
    {
        qDebug()<<"no connection";
    }
}

void MainWindow::PlotData(int ch_no,  int p1, int pt, QVector<double> &sample_no, QVector<double> &eeg_data)
{
    //eeg_all[ch_no%8].push_back(p1);

    eeg_data.push_back(p1);
//    if(ch_no%8 == 0)
//    {
        sample_no.push_back(pt);
    //}


    int time_s = 8;
    int buffer_size = ui->spinBox_2->value()*ui->spinBox_3->value()*3*time_s;
    if(sample_no.size()==buffer_size)
    {
//        for(int i=0;i<8;i++)
//        {
//            eeg_all[i].pop_front();
//        }

        eeg_data.pop_front();
        sample_no.pop_front();
    }


    if(pt%20 == 0)
    {
        //qDebug()<<pt%300;

        ui->widget->graph(0)->setData(sample_no,eeg_data);
        ui->widget->graph(0)->rescaleAxes();
        ui->widget->replot();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readTcpData()
{
    mySocket->waitForBytesWritten(3000);
    mySocket->waitForReadyRead(3000);


    qDebug()<<"cos czytam";//<<data_counter;
    QByteArray danetcp;
    danetcp.reserve(3000);
    danetcp = mySocket->readAll();
    //dekodowanie
    int offset = 0;

    int dsize= danetcp.size();

    if(dsize>0)
    {
        int ch_no = 0;
        for(int i = 0; i<dsize/3; i++)
        {
            char fbyte, mbyte, lbyte;

            offset = i*3;

            if(offset+2<=dsize)
            {
                fbyte = danetcp.at(offset+2);
                mbyte = danetcp.at(offset+1);
                lbyte = danetcp.at(offset);

                int b3,b2,b1;
                b3 = int(fbyte)*256*256*256;
                b2 = int(mbyte)*256*256;
                b1 = int(lbyte)*256;

                int sample = uint32_t(b3+b2+b1+0);

                if(ch_no == 8)
                {
                    ch_no = 0;
                }

                sample = sample + 256*256*256*ch_no;
                if(ch_no==0)
                {
                    s_no+=1;
                    sample_no.push_back(s_no);
                }

                eeg_all[ch_no].push_back(sample);

                ch_no+=1;


                    //PlotData(i/3, sample, s_no, sample_no, eeg_data);
                    qDebug()<<eeg_all[0].size()<<" and "<<sample_no.size();
                //}
                //std::string stdString(fbyte.constData(), fbyte.length());
                //qDebug()<<QString::fromStdString(stdString);
                //qDebug()<<b3+b2+b1<<";"<<sample;
            }

        }
    }

    if(sample_no.size()>1000)
    {
        int rem = sample_no.size();
        sample_no.remove(0,(rem-1000));
        //sample_no.pop_front();
        for(int i=0;i<8;i++)
        {
            eeg_all[i].remove(0,(rem-1000));;
        }
    }
    //plot
    ui->widget->graph(0)->setData(sample_no,eeg_all[ui->spinBox_3->value()-1]);
    ui->widget->graph(0)->rescaleAxes();
    ui->widget->replot();




    //qDebug()<<"size: "<<data.size();
}

void MainWindow::on_pushButton_clicked()
{
    eeg_all.clear();
    eeg_all.reserve(ui->spinBox_2->value());
    for(int i=0;i<ui->spinBox_2->value();i++)
    {
        eeg_all.push_back(eeg_data);
    }
    Connect();
}
