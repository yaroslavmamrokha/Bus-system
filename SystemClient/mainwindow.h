#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "payment.h"
#include "client_network.h"
#include "myclientthread.h"
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    //set interfaces to selected status(based on user)
    void SetAllToStatus(bool = Ui::disabled);
    //store ticket to PC
    void StoreTicket();
private slots:
    //send selected date to client
    void on_calendarWidget_selectionChanged();
    //save all selected user data
    void on_selectButton_clicked();
    //go to payment step, wait until all data filled, send bus id and seat to server for reservation
    void on_nextButton_clicked();
    //change all selected data
    void on_changedecisionButtom_clicked();
    //print received data from server
    void Print_Data_To_Screen();
    //check if data was received
    void isDataReceived();
    //close all connections and shutdown program
    void on_exitButton_clicked();
    //update current time
    void update_clock();
    //disable all interface elements
    void DisableAll();
    //try to connect to server again
    void on_reconnectButton_clicked();
    //add current rows places to comboBox
    void on_tableWidget_itemSelectionChanged();
    //set servers ip
    void on_ipBox_clicked();

private:
    MyClientThread* working_thread; //thread for communication with server
    vector<QString>recv_data;
    bool server_init{false};//flag for checking if server structure is initialised
    QString current_date;
    QString id;
    QString place;
    int current_index;
    Payment* pay_dialog;
    Ui::MainWindow *ui;
    Client_Network * curr_network; //current network
    QTimer* up_clock_timer;
    QTimer* recv_check_timer;
};

#endif // MAINWINDOW_H
