#include "countdown.h"
#include "./ui_countdown.h"
#include <QTimer>
#include <windows.h>
#include <string>
#include <QByteArray>

// TODO: Use default state variable to keep time left or not

Countdown::Countdown(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Countdown)
{
    // Timer Setup
    ui->setupUi(this);
    timer = new QTimer(this);

    // Run ticker() on timeout
    connect(timer, SIGNAL(timeout()), this, SLOT(ticker()));

    // Attempt - look into restoreState() use of QSettings
//    QByteArray defaultWindow {this->saveState(1)};
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
    if (Countdown::defaultState == true)
    {
        // Variables capture values
        int seconds = ui->spinSeconds->value();
        int minutes = ui->spinMinutes->value();
        int hours = ui->spinHours->value();

        // If no input but button was clicked - throw error and kill function
        if (seconds == 0 && minutes == 0 && hours == 0)
        {
            ui->text_above_num_label->setText("ERROR: Please enter a time.");
            return;
        }

        // Hide UI
        ui->spinSeconds->hide();
        ui->spinMinutes->hide();
        ui->spinHours->hide();
        ui->sLabel->hide();
        ui->mLabel->hide();
        ui->hLabel->hide();

        // Create time formatted string
        QString tString{Countdown::createTimeString(hours, minutes, seconds)};

        // Update UI
        ui->text_above_num_label->setText("Countdown:");
        ui->numlabel->setText(tString);
        ui->cdButton->setText("Pause");

        // Start timer - slot every 1000ms
        timer->start(1000);

        // Update tracker variables
        Countdown::timerStarted = true;
        Countdown::defaultState = false;
    }

    // Countdown pause if timer is ticking
    else if (Countdown::defaultState == false && Countdown::timerStarted == true)
    {
        timer->stop();
        Countdown::timerStarted = false;
        ui->cdButton->setText("Start");
    }

    else // defaultState == false and timerStarted == false
    {
        timer->start(1000);
        Countdown::timerStarted = true;
        ui->cdButton->setText("Pause");
    }
}

void Countdown::ticker()
{
    // Convert number strings to integers as variables
    int intHours {Countdown::cHours.toInt()};
    int intMinutes {Countdown::cMinutes.toInt()};
    int intSeconds {Countdown::cSeconds.toInt()};

    // Decrements seconds/minutes/hours as needed
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

    // Convert int variables to QString and set it to label text
    QString tString{Countdown::createTimeString(intHours, intMinutes, intSeconds)};

    // Update UI with QString above
    ui->numlabel->setText(tString);

    // Countdown completes
    if (Countdown::cSeconds == "00" &&
        Countdown::cMinutes == "00" &&
        Countdown::cHours == "00")
    {
        timer->stop();
        Countdown::timerStarted = false;
        Countdown::defaultState = true;
        qApp->processEvents();
        Sleep(1000);
        ui->text_above_num_label->clear();
        ui->numlabel->clear();
        qApp->processEvents();
        Sleep(1000);
        ui->text_above_num_label->setText("Countdown complete.");
        qApp->processEvents();
        Sleep(2000);

        // Reset UI
        ui->numlabel->setText("Thank you :)");
        ui->cdButton->setText("Start");
        ui->spinSeconds->show();
        ui->spinMinutes->show();
        ui->spinHours->show();
        ui->sLabel->show();
        ui->mLabel->show();
        ui->hLabel->show();
    }
}

void Countdown::on_closeButton_clicked()
{
    // Fun stuff
//    Sleep(2000);
//    ui->numlabel->setStyleSheet("QLabel{color: rgb(170, 0, 0);}");
//    qApp->processEvents();
//    Sleep(300);

    close();
}

void Countdown::on_resetButton_clicked()
{
    // Reset UI and program variables
    timer->stop();

//    // Attempt
//    this->restoreState(defaultWindow);

    ui->spinSeconds->show();
    ui->spinMinutes->show();
    ui->spinHours->show();
    ui->sLabel->show();
    ui->mLabel->show();
    ui->hLabel->show();
    ui->numlabel->setText("");
    ui->text_above_num_label->setText("");
    ui->cdButton->setText("Start");
    Countdown::cSeconds = "00";
    Countdown::cMinutes = "00";
    Countdown::cHours = "00";
    Countdown::timerStarted = false;
    Countdown::defaultState = true;
}

