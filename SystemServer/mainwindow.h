#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "myserverthread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //connect to database button
    void on_connectDBButton_clicked();
    //Fill places column on selected id
    void on_pushButton_clicked();
    //show database in table widget button
    void on_showDBButton_clicked();
    //insert to database method
    void Insert();
    //signal checking for connection to server
    void check_connections();
    //search in database using date parameter;
    void on_searchinDBButton_clicked();
    //insert info to database(all filleds requiers!)
    void on_inserttoDBButton_clicked();
    //delete from database selected id
    void on_deletefromDBButton_clicked();
    //exit program button
    void on_exitButton_clicked();
    //launch server to global network
    void on_startserverButton_clicked();
    //set ip for database
    void on_ipBox_clicked();

private:
    void ShowDB();
    Ui::MainWindow *ui;
    QString connected;
    QString prev_connected;
    QString recv_value{RecvD};
    MyServerThread* accepting_thread; //acception thread
    MyServerThread* working_thread; // working with client thread
    SQLDbaseService* dataBase;
    Server_Network* curr_network;
    vector<QString> cur_db_vals; //current database values
    vector<QString> col_names;
    QString inst_val;
    QTableWidget* insert_widg;
    QTimer *thread_timer; //thread timer
};

#endif // MAINWINDOW_H
