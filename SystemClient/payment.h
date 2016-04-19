#ifndef PAYMENT_H
#define PAYMENT_H
#include <QDialog>
#include "samelibs.h"
namespace Ui {
class Payment;
enum Status{
   disabled = false,
   enabled = true
};
enum class Magic : int{
    LineSizes = 19
};
}

class Payment : public QDialog
{
    Q_OBJECT

public:
    explicit Payment(QWidget *parent = 0);
    ~Payment();
    //check if payment successfully finished
    bool isFinish();
protected:
    //set current line status
    void SetLineStatus(bool = Ui::disabled);
    //get sizes of card number fields
    int GetSizes();
private slots:
    //update timer
    void update_lcd();
    //confirm current input
    void on_confirmButton_clicked();
    //edit current input
    void on_editButton_clicked();
    //abandon payment and go back to data selection
    void on_backButton_clicked();
    //finish payment process
    void on_finishButton_clicked();

private:
    Ui::Payment *ui;
    QString card_data;
    QTimer* lcd_timer;
    bool send_data{0};
    int timer{60};
    bool timer_on = false;
};

#endif // PAYMENT_H
