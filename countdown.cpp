#include "countdown.h"
#include "./ui_countdown.h"
#include <QTimer>
#include <windows.h>
#include <string>

Countdown::Countdown(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Countdown)
{
    ui->setupUi(this);
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(ticker()));
}

Countdown::~Countdown()
{
    delete ui;
}

QString Countdown::createTimeString(int hours, int minutes, int seconds)
{
    QString tString{ "" };
    if (hours >= 10)
    {
        Countdown::cHours = QString::fromStdString(std::to_string(hours));
        tString += Countdown::cHours + ':';
    }
    else
    {
        Countdown::cHours = QString::fromStdString('0' + std::to_string(hours));
        tString += Countdown::cHours + ':';
    }

    if (minutes >= 10)
    {
        Countdown::cMinutes = QString::fromStdString(std::to_string(minutes));
        tString += Countdown::cMinutes + ':';
    }
    else
    {
        Countdown::cMinutes = QString::fromStdString('0' + std::to_string(minutes));
        tString += Countdown::cMinutes + ':';
    }

    if (seconds >= 10)
    {
        Countdown::cSeconds = QString::fromStdString(std::to_string(seconds));
        tString += Countdown::cSeconds;
    }
    else
    {
        Countdown::cSeconds = QString::fromStdString('0' + std::to_string(seconds));
        tString += Countdown::cSeconds;
    }
    return tString;
}

void Countdown::on_cdButton_clicked()
{
    // If time is up or not yet started
    if (Countdown::cSeconds == "00" &&
        Countdown::cMinutes == "00" &&
        Countdown::cHours == "00")
    {
        // Variables
        int seconds = ui->spinSeconds->value();
        int minutes = ui->spinMinutes->value();
        int hours = ui->spinHours->value();

        // Create time formatted string
        QString tString{Countdown::createTimeString(hours, minutes, seconds)};

        ui->text_above_num_label->setText("Downloading virus in:");

        ui->numlabel->setText(tString);
        timer->start(1000);
    }

    // BOOKMARK - set up a pause that stops the timer and changed the text on the button
    // set up github commits with this maybe https://cpb-us-w2.wpmucdn.com/u.osu.edu/dist/7/11881/files/2018/02/QtCreatorGitTutorial-2ahr2m0.pdf
    else
    {
        timer->stop();
        Countdown::cSeconds = "00";
        Countdown::cMinutes = "00";
        Countdown::cHours = "00";
    }
}

void Countdown::ticker()
{
    int intHours {Countdown::cHours.toInt()};
    int intMinutes {Countdown::cMinutes.toInt()};
    int intSeconds {Countdown::cSeconds.toInt()};


    // Decrements seconds/minutes/hours correctly
    if (intSeconds > 0)
    {
        --intSeconds;
    }
    else if (intSeconds == 0)
    {
        if (intMinutes > 0)
        {
            --intMinutes;
            intSeconds = 59;
        }
        else if (intMinutes == 0)
        {
            --intHours;
            intMinutes = 59;
            intSeconds = 59;
        }

    }




    QString tString{Countdown::createTimeString(intHours, intMinutes, intSeconds)};
    //Countdown::cNum = QString::number(intNum);
    ui->numlabel->setText(tString);


    if (Countdown::cSeconds == "00" &&
        Countdown::cMinutes == "00" &&
        Countdown::cHours == "00")
    {
        timer->stop();
        qApp->processEvents();
        Sleep(1000);
        ui->text_above_num_label->clear();
        ui->numlabel->clear();
        qApp->processEvents();
        Sleep(1000);
        ui->text_above_num_label->setText("Download complete.");
        qApp->processEvents();
        Sleep(2000);
        ui->numlabel->setText("Thank you :)");
    }
}

void Countdown::on_closeButton_clicked()
{
    Sleep(2000);
    ui->numlabel->setStyleSheet("QLabel{color: rgb(170, 0, 0);}");
    qApp->processEvents();
    Sleep(300);
    close();
}


//void Countdown::ticker()
//{
//    int intNum {Countdown::cNum.toInt()};
//    --intNum;
//    Countdown::cNum = QString::number(intNum);
//    ui->numlabel->setText(Countdown::cNum);
//    if (intNum == 0)
//    {
//        timer->stop();
//        qApp->processEvents();
//        Sleep(1000);
//        ui->text_above_num_label->clear();
//        ui->numlabel->clear();
//        qApp->processEvents();
//        Sleep(1000);
//        ui->text_above_num_label->setText("Download complete.");
//        qApp->processEvents();
//        Sleep(2000);
//        ui->numlabel->setText("Thank you :)");
//    }
//}
