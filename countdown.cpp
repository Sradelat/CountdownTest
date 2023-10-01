#include "countdown.h"
#include "./ui_countdown.h"
#include <QTimer>
#include <windows.h>
#include <string>
#include <QByteArray>

Countdown::Countdown(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Countdown)
{
    // Timer Setup
    ui->setupUi(this);
    timer = new QTimer(this);

    // Run ticker() on timeout
    connect(timer, SIGNAL(timeout()), this, SLOT(ticker()));
}

Countdown::~Countdown()
{
    delete ui;
}


/**
 * Stops timer and resets UI and UI related variables to runtime state
 * except for spinner values as a reminder.
 */
void Countdown::resetUI()
{
    // Could look into using saveState() and restoreState() using QSetting instead of this
    timer->stop();
    ui->spinSeconds->show();
    ui->spinMinutes->show();
    ui->spinHours->show();
    ui->sLabel->show();
    ui->mLabel->show();
    ui->hLabel->show();
    ui->numlabel->clear();
    ui->text_above_num_label->clear();
    ui->cdButton->setText("Start");
    Countdown::timerStarted = false;
    Countdown::defaultState = true;
}


/**
 * Converts seconds, minutes, hours variables from int to QString for display
 * in format HH:MM:SS
 */
QString Countdown::createTimeString(int hours, int minutes, int seconds)
{
    QString tString{ "" };

    // 0 pads to the left if < 10
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


/**
 * Grabs input from spin boxes and converts to a string to display starting time and then starts timer.
 *
 * If no input, throws error and does not start timer
 */
void Countdown::on_cdButton_clicked()
{
    if (Countdown::defaultState == true)
    {
        // Variables capture values in spin boxes
        int seconds = ui->spinSeconds->value();
        int minutes = ui->spinMinutes->value();
        int hours = ui->spinHours->value();

        // If no input but start button was clicked - throw error and stop function
        if (seconds == 0 && minutes == 0 && hours == 0)
        {
            ui->text_above_num_label->setText("ERROR: Please enter a time.");
            return;
        }

        // Hide these UI elements
        ui->spinSeconds->hide();
        ui->spinMinutes->hide();
        ui->spinHours->hide();
        ui->sLabel->hide();
        ui->mLabel->hide();
        ui->hLabel->hide();

        // Create time formatted string HH:MM:SS
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

    // Restarts ticker with no save state from paused time
    else // defaultState == false and timerStarted == false
    {
        timer->start(1000);
        Countdown::timerStarted = true;
        ui->cdButton->setText("Pause");
    }
}


/**
 * Decrements time according to timer timeout and displays time remaining.
 *
 * When time runs out, resets UI and displays notification that countdown was completed.
 */
void Countdown::ticker()
{
    // Convert number strings to integers as variables for numeric manipulation
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

    // Convert int variables back to QString and set it to label text
    QString tString{Countdown::createTimeString(intHours, intMinutes, intSeconds)};
    ui->numlabel->setText(tString);

    // Countdown completes
    if (Countdown::cSeconds == "00" &&
        Countdown::cMinutes == "00" &&
        Countdown::cHours == "00")
    {
        resetUI();

        // Notify user countdown was completed
        ui->text_above_num_label->setText("Countdown complete.");
    }
}


// Closes window
void Countdown::on_closeButton_clicked()
{
    close();
}


// Resets UI to runtime state
void Countdown::on_resetButton_clicked()
{
    resetUI();
}

