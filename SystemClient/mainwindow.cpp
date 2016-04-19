#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers( QAbstractItemView::NoEditTriggers );
    ui->nextButton->setEnabled(Ui::disabled);
    ui->dateLCD->setDigitCount(10);
    ui->dateLCD->setPalette(Qt::black);
    ui->clockLcd->setDigitCount(8);
    ui->reconnectButton->hide();
    ui->clockLcd->setPalette(Qt::black);
    ui->ipEdit->setInputMask( "000.000.000.000" );

    update_clock();
    up_clock_timer = new QTimer(this);
    recv_check_timer = new QTimer(this);
        up_clock_timer->start(1000);
        DisableAll();
        connect(up_clock_timer,SIGNAL(timeout()),this,SLOT(update_clock()));
        connect(recv_check_timer,SIGNAL(timeout()),this,SLOT(isDataReceived()));
    }

MainWindow::~MainWindow()
{
    try{
    curr_network->Send_Data("2");
    ui->logBrowser->append("Shutdown request sent..");
    }catch(NETException& ex){
         ui->logBrowser->append(QString::fromStdString(string{ex.what()}));
    }

    delete ui;
    up_clock_timer->stop();
}

void MainWindow::SetAllToStatus(bool status)
{
    ui->calendarWidget->setEnabled(status);
    ui->placeList->setEnabled(status);
    ui->selectButton->setEnabled(status);
    ui->tableWidget->setEnabled(status);
    ui->changedecisionButtom->setEnabled(!status);
    ui->nextButton->setEnabled(!status);
}

void MainWindow::StoreTicket()
{
    QStringList lst = recv_data.at(ui->tableWidget->currentRow()).split("#");
    lst.pop_back();
    QFile ticket("Ticket.doc");
    remove("Ticket.doc");
    ticket.open(QFile::WriteOnly|QFile::Text);
    QTextStream out(&ticket);
    out<<"/*************************************************************/\n";
    out<<"/********************Ticket on bus line: "+lst.at(0)+"********************/\n";
    out<<"From: "+lst.at(1)+". To: "+lst.at(2)+". Date: "+lst.at(3)+"\n";
    out<<"Station: "+lst.at(4)+". Bus seat: "+ui->placeList->currentText()+"\n";
    out<<"Have a nice journey!!! Sincerely: YMamrokha Bus Company(c)\n";
    out<<"/*************************************************************/\n";
    ticket.close();
}

void MainWindow::on_calendarWidget_selectionChanged()
{
  current_date = ui->calendarWidget->selectedDate().toString("dd.MM.yyyy");
  working_thread = new MyClientThread(curr_network,current_date);
  working_thread->start();
  recv_check_timer->start(200);
}

void MainWindow::on_selectButton_clicked()
{
    //get selected row and store it in variable until user press Next
    if(!ui->tableWidget->isItemSelected(ui->tableWidget->currentItem())){
         QMessageBox::warning(this,"Force select!","You haven't selected bus route");
         return;
    }
    current_index = ui->tableWidget->currentRow();
    id = ui->tableWidget->item(current_index,0)->text();
    place = ui->placeList->currentText();
    if(!place.compare("No free seats!")){
           QMessageBox::warning(this,"No available seats","No seats. Choose another rout!");
           return;
    }
    SetAllToStatus(Ui::disabled);


}


void MainWindow::on_nextButton_clicked()
{
    //send request to server for payment process
    pay_dialog = new Payment(this);
    this->hide();
    pay_dialog->exec();
    //check if payment successfully finished
    if(pay_dialog->isFinish()){
        try{
            //send request with data to server; if failed triggers exception
       curr_network->Send_Data("1");
       QThread::msleep(250);
       curr_network->Send_Data(id.toStdString().c_str());
       QThread::msleep(150);
       curr_network->Send_Data(place.toStdString().c_str());
       QThread::msleep(150);
       curr_network->Send_Data("2");
       ui->logBrowser->append("payment data sent..");
        }
        catch(NETException &ex){
            ui->logBrowser->append(QString::fromStdString(string{ex.what()}));
            ui->reconnectButton->show();
            DisableAll();
            this->show();
            return;
          }
        QMessageBox::information(this,"Ticket Data","Your ticket is stored on disk");

    }
    if(pay_dialog->isFinish()){

        //store ticket on pc
        StoreTicket();
        this->close();
    }else{
    this->show();
    }
}

void MainWindow::on_changedecisionButtom_clicked()
{
    id.clear();
    place.clear();
    //reset everything to date selection
    SetAllToStatus(Ui::enabled);
}

void MainWindow::Print_Data_To_Screen()
{
    if(working_thread->isRunning()){
        return;
    }
    if(working_thread->isExcept()){
        ui->logBrowser->append(working_thread->GetExcept());
        ui->reconnectButton->show();
        DisableAll();
        recv_check_timer->stop();
        return;
    }
    recv_check_timer->stop();
    //get received data from thread
    recv_data = working_thread->GetThreadData();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"Bus id","From City","To city","Station"});
    QStringList lst;
    for(int i = 0; i< recv_data.size(); i++){
        ui->tableWidget->insertRow(i);
        lst = recv_data.at(i).split("#");
        int inc = 0;
        for(int j = 0; j < 6; j++){
            if(j ==3 ||j==5){
                continue;
            }
            ui->tableWidget->setItem(i,inc++,new QTableWidgetItem{lst.at(j)});

        }
    }

}

void MainWindow::isDataReceived()
{
   Print_Data_To_Screen();
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}

void MainWindow::update_clock()
{
    ui->clockLcd->display(QTime::currentTime().toString(QString("hh:mm:ss")));
    ui->dateLCD->display(QDateTime::currentDateTime().toString(QString("dd.MM.yyyy")));
}

void MainWindow::DisableAll()
{
    ui->calendarWidget->setEnabled(Ui::disabled);
    ui->placeList->setEnabled(Ui::disabled);
    ui->selectButton->setEnabled(Ui::disabled);
    ui->tableWidget->setEnabled(Ui::disabled);
    ui->changedecisionButtom->setEnabled(Ui::disabled);
    ui->nextButton->setEnabled(Ui::disabled);
}

void MainWindow::on_reconnectButton_clicked()
{
    try{
        curr_network->Start_Client();
        curr_network->Connect();
        ui->reconnectButton->hide();
        //enable all buttons for user
        SetAllToStatus(Ui::enabled);
        ui->logBrowser->append("Successfully reconnected");
    }
    catch(NETException&ex){
          ui->logBrowser->append(QString::fromStdString(string{ex.what()}));
        }
}

void MainWindow::on_tableWidget_itemSelectionChanged()
{
    int curr_row = ui->tableWidget->currentRow();
    if(curr_row<0){
        ui->placeList->clear();
        return;
    }
    //all data is divided using '#'
    // split it and add to comboBox
    QStringList lst = recv_data.at(curr_row).split("#");
    lst.pop_back();
    QString places = lst.back();
    lst = places.split(",");
    ui->placeList->clear();
    for(auto x: lst){
        ui->placeList->addItem(x);
    }
}

void MainWindow::on_ipBox_clicked()
{

    if(ui->ipEdit->text().isEmpty()){
        QMessageBox::information(this, "Invalid IP format","Bad server_ip address");
        return;
    }
    if(!ui->ipBox->isChecked()){
        ui->reconnectButton->hide();
         ui->ipEdit->setEnabled(true);
        return;
    }
    ui->ipEdit->setEnabled(false);
    //set structure with new server IP
    curr_network = new Client_Network(ui->ipEdit->text());
    try{
        curr_network->Start_Client();
        ui->logBrowser->append("Client Started..");
    }catch(NETException&ex){
        ui->logBrowser->append(QString::fromStdString(string{ex.what()}));
        ui->logBrowser->append("Try again or enter another IP...");
        ui->ipBox->setEnabled(true);
        ui->ipBox->setChecked(false);
        ui->ipEdit->setEnabled(true);
        DisableAll();
    }
    try{
        //if failed triggers exception
        curr_network->Connect();
        ui->logBrowser->append("client connected to server..");
        SetAllToStatus(true);
    }catch(NETException&ex){
      ui->logBrowser->append(QString::fromStdString(string{ex.what()}));
       ui->logBrowser->append("Try again or enter another IP...");
      ui->ipBox->setEnabled(true);
       ui->ipBox->setChecked(false);
      ui->ipEdit->setEnabled(true);
      DisableAll();
    }
}
