#include "payment.h"
#include "ui_payment.h"

Payment::Payment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Payment)
{
    ui->setupUi(this);
    this->setMaximumSize(600,400);
    this->setMinimumSize(600,400);
    this->setWindowTitle("Payment Operations");
    ui->editButton->setEnabled(false);
    ui->finishButton->setEnabled(false);
    ui->lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,4}")));
    ui->lineEdit_2->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,4}")));
    ui->lineEdit_3->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,4}")));
    ui->lineEdit_4->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,4}")));
    ui->lineEdit_5->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,3}")));
    ui->expireTime->setPalette(Qt::green);
    ui->expireTime->display(timer);
     lcd_timer = new QTimer(this);
    connect(lcd_timer, SIGNAL(timeout()), this, SLOT(update_lcd()));
    lcd_timer->start(1000);
}

Payment::~Payment()
{
    delete ui;
    delete lcd_timer;
}

bool Payment::isFinish()
{
    return send_data;
}

void Payment::SetLineStatus(bool type)
{
    ui->lineEdit->setEnabled(type);
    ui->lineEdit_2->setEnabled(type);
    ui->lineEdit_3->setEnabled(type);
    ui->lineEdit_4->setEnabled(type);
    ui->lineEdit_5->setEnabled(type);

}

int Payment::GetSizes()
{
    int sizes = ui->lineEdit->text().size();
    sizes+=ui->lineEdit_2->text().size();
    sizes+=ui->lineEdit_3->text().size();
    sizes+=ui->lineEdit_4->text().size();
    sizes+=ui->lineEdit_5->text().size();
    return sizes;
}

void Payment::update_lcd()
{
    ui->expireTime->display(--timer);
    if(timer == 10){
        ui->expireTime->setPalette(Qt::red);
    }
     if(timer<0){
         lcd_timer->stop();
         this->close();
     }
}

void Payment::on_confirmButton_clicked()
{
    int max_size = static_cast<int>(Ui::Magic::LineSizes);
    if(GetSizes()<max_size){
        QMessageBox::warning(this,"Bad input", "All fields must be filled");
        return;
    }
    SetLineStatus(Ui::disabled);
    //form card number
    card_data=card_data+ui->lineEdit->text()+" ";
    card_data=card_data+ui->lineEdit_2->text()+" ";
    card_data=card_data+ui->lineEdit_3->text()+" ";
    card_data=card_data+ui->lineEdit_4->text()+" ";
    //and seperate it from CVV using '#'
    card_data=card_data+"#"+ui->lineEdit_5->text();
    ui->editButton->setEnabled(Ui::enabled);
    ui->finishButton->setEnabled(Ui::enabled);
    ui->confirmButton->setEnabled(Ui::disabled);
}

void Payment::on_editButton_clicked()
{
    SetLineStatus(Ui::enabled);
    ui->finishButton->setEnabled(Ui::disabled);
    ui->confirmButton->setEnabled(Ui::enabled);
    ui->editButton->setEnabled(Ui::disabled);
}


void Payment::on_backButton_clicked()
{
    this->close();
}

void Payment::on_finishButton_clicked()
{
    send_data = true;
    this->close();
}
