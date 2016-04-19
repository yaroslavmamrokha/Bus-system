#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QStringList>
#include <QWidget>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->deletefromDBButton->hide();
    col_names.push_back("idBuslines");
    col_names.push_back("Fromdirection");
    col_names.push_back("Todirectiom");
    col_names.push_back("Date");
    col_names.push_back("station");
    col_names.push_back("Places");
    ui->showDBButton->setEnabled(false);
    ui->inserttoDBButton->setEnabled(false);
    ui->dateEdit->setEnabled(false);
    ui->searchinDBButton->setEnabled(false);
    ui->disconAllButton->setEnabled(false);
    thread_timer = new QTimer;
    dataBase = new SQLDbaseService;
     ui->ipEdit->setInputMask( "000.000.000.000" );
    ui->pushButton->setEnabled(false);
    connect(thread_timer,SIGNAL(timeout()),this,SLOT(check_connections()));


}

MainWindow::~MainWindow()
{
    delete ui;
    if(accepting_thread){
        accepting_thread->terminate();
    }
    if(working_thread){
        working_thread->terminate();
    }
    thread_timer->stop();
}

void MainWindow::on_connectDBButton_clicked()
{

    try{
        //if connection failed exception is triggered
    dataBase->ConnectBase();
    ui->connectedBrowser->append("Connected succesfully to the database: "+dataBase->BaseName());
    ui->showDBButton->setEnabled(true);
    ui->inserttoDBButton->setEnabled(true);
    ui->dateEdit->setEnabled(true);
    ui->searchinDBButton->setEnabled(true);
     ui->pushButton->setEnabled(true);
     ui->connectDBButton->setEnabled(false);
     ui->connectDBButton->hide();
    }
    catch(SQLException& ex){
        QMessageBox::critical(this,"We got exception",QString::fromStdString(ex.what()));
        ui->connectedBrowser->append("Connection to database failed. Error: "+QString::fromStdString(ex.what()));
    }
}


void MainWindow::on_pushButton_clicked()
{
     int row = ui->sqlView->currentRow();
     QString id = ui->sqlView->item(row,0)->text();
     dataBase->UpdateDatabase(id,DatabaseColumnNames::PLACES,"6,7,8");
}

void MainWindow::on_showDBButton_clicked()
{
    try{
       cur_db_vals.clear();
       cur_db_vals = dataBase->GetDataBase();
       ui->connectedBrowser->append("Succesfully received database");
       //print database to table widget
       ShowDB();
    }
    catch(SQLException& ex){
        ui->connectedBrowser->append("Failed to recieve database. Error: "+QString::fromStdString(ex.what()));

    }
}

void MainWindow::Insert()
{
    inst_val.clear();
    for(int i = 0; i<6; i++){
        if(insert_widg->item(0,i)->text().isEmpty()){
            QMessageBox::critical(this, "Empty input column","Not all columns filled, try again!");
           return;
        }
      inst_val = inst_val+insert_widg->item(0,i)->text() + "#";
    }
    QStringList lst = inst_val.split("#");
    try{
    dataBase->InsertToDatabase(lst.toVector().toStdVector());
    ui->connectedBrowser->append("Succesfully inserted in the database");
    }
    catch(SQLException& ex){
         QMessageBox::critical(this,"We got exception",QString::fromStdString(ex.what()));
         ui->connectedBrowser->append("Failed to insert in the database. Error: "+QString::fromStdString(ex.what()));
    }
}



void MainWindow::check_connections()
{
   //lock current section to avoid data races
    mt2.lock();
    //check if client connected and start working with it
    if(!connected.isEmpty()){
        working_thread = new MyServerThread(curr_network,dataBase);
        working_thread->SetOpType(2);
        working_thread->start();
         ui->connectedBrowser->append("Connected client with Ip: "+connected);
         connected.clear();
    }
    mt2.unlock();
}

void MainWindow::on_searchinDBButton_clicked()
{
    try{
        cur_db_vals.clear();
        QString date = ui->dateEdit->text();
        date.replace("/",".");
        //if failed triggers exception
        cur_db_vals = dataBase->SearchInDatabase(col_names.at(DatabaseColumnNames::DATE_NAME),date);
        ui->connectedBrowser->append("Data was successfuly found in the database");
        ShowDB();
    }
    catch(SQLException& ex){
        ui->connectedBrowser->append("Failed to search in the database. Error: "+QString::fromStdString(ex.what()));
    }
}

void MainWindow::ShowDB()
{
    ui->sqlView->deleteLater();
    ui->sqlView = new QTableWidget(this);
    QHeaderView* p = ui->sqlView->horizontalHeader();
    p->setSectionResizeMode(QHeaderView::Stretch);
    ui->sqlView->setColumnWidth(0,200);
    ui->sqlView->setMaximumSize(621,271);
    ui->sqlView->setMinimumSize(621,271);
    ui->sqlView->show();
    vector<QStringList> tmp_str;
    QStringList lst;
    for(auto x : cur_db_vals){
        lst = x.split("#");
        lst.pop_back();
        tmp_str.push_back(lst);
    }
    ui->sqlView->setColumnCount(6);
    int i = 0;
    int j = 0;
   for(auto tmp_lst: tmp_str){
       ui->sqlView->insertRow(i);
       for(auto x: tmp_lst){
           ui->sqlView->setItem(i,j++,new QTableWidgetItem(x));
       }
       j = 0;
       i++;
   }
   ui->sqlView->setHorizontalHeaderLabels(QStringList::fromVector(QVector<QString>::fromStdVector(col_names)));
   ui->sqlView->setCurrentCell(0,0);
   ui->deletefromDBButton->show();
}

void MainWindow::on_inserttoDBButton_clicked()
{
    QDialog* ma = new QDialog(this);
    insert_widg = new QTableWidget(ma);
    insert_widg->setMaximumSize(500,100);
    insert_widg->setMinimumSize(500,100);
    ma->setMaximumSize(500,100);
    ma->setMinimumSize(500,100);
    insert_widg->setColumnCount(6);
    insert_widg->insertRow(0);
    for(int i = 0; i<6; i++){
      insert_widg->setItem(0,i,new QTableWidgetItem(""));
    }
    insert_widg->setHorizontalHeaderLabels(QStringList::fromVector(QVector<QString>::fromStdVector(col_names)));
    insert_widg->show();
    ma->exec();
    Insert();
}

void MainWindow::on_deletefromDBButton_clicked()
{


   int row = ui->sqlView->currentRow();
   int col = ui->sqlView->currentColumn();
   if(!ui->sqlView->rowCount()){
       return;
   }
   QString col_name = ui->sqlView->horizontalHeaderItem(col)->text();
   QString curr_id = ui->sqlView->item(row,col)->text();
   try{
       //triggers exception if failde to delete from table
   dataBase->DeleteFromDatabase(col_name,curr_id);
   ui->connectedBrowser->append("Succesfully deleted from the database");
   }
   catch(SQLException& ex){
       ui->connectedBrowser->append("Failed to delete from the database. Error: "+QString::fromStdString(ex.what()));
   }
}



void MainWindow::on_exitButton_clicked()
{
    this->close();
}

void MainWindow::on_startserverButton_clicked()
{
    try{
        // if failed  triggers exception
        curr_network = new Server_Network;
        curr_network->Start_Server();
        ui->connectedBrowser->append("Succesfully created server. Launching acception thread...");
        accepting_thread = new MyServerThread(curr_network,dataBase);
        connected = prev_connected;
        accepting_thread->UpDateConnected(connected);
        //set operation type to client acception
        accepting_thread->SetOpType(1);
        accepting_thread->start();
        thread_timer->start(100);
        ui->startserverButton->setEnabled(false);

    }
    catch(NETException& ex){
         ui->connectedBrowser->append("Failed to start server. Error: "+QString::fromStdString(ex.what()));
    }
}

void MainWindow::on_ipBox_clicked()
{
    if(!ui->ipBox->isChecked()){
        ui->ipEdit->setEnabled(true);
        return;
    }
    ui->ipEdit->setEnabled(false);
    //set new host ip to database
    dataBase->ChangeHostIp(ui->ipEdit->text());
}
