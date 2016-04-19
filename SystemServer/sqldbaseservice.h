#ifndef SQLDBASESERVICE_H
#define SQLDBASESERVICE_H
#include "samelibs.h"
using namespace std;
#define DB_HOST_NAME "localhost"
#define DB_USER_NAME "guest"
#define DBASE_NAME "bussql"
#define DB_PASSWORD "1111"
#define DB_PORT 3307
enum ExcepCode{
    DATA_NOT_FOUND,
    OPEN_FAILED,
    QUERY_FAILED
};

enum DatabaseColumnNames{
    ID,
    SRC_CITY,
    DST_CITY,
    DATE_NAME,
    STATION,
    PLACES
};


class SQLException:public exception{
private:
    QString excep_info;
    int exc_code;

public:
    SQLException(QString ei,int ec):excep_info{ei},exc_code{ec}{}
    const char *what();
    int GetExceptionCode()const;
};

class SQLDbaseService
{
private:
 QSqlDatabase curr_database; //database
 QString host_name{DB_HOST_NAME};
 QString query_name;
 vector<QString> db_col_names;
 vector<QString> input_data;
 vector<QString>response_data;
 vector<QString>update_data;
protected:
 void LoadDataToUpdateVector();
public:
    SQLDbaseService();
    SQLDbaseService(const int);
    //connect to database
    void ConnectBase();
    // get database name
    QString BaseName();
    //change current database ip
    void ChangeHostIp(QString);
    //update current database
    bool UpdateDatabase(QString, int, QString);
    //insert data to database
    bool InsertToDatabase(vector<QString>);
    //search row using date
    vector<QString>SearchInDatabase(QString,QString);\
    //get all database
    vector<QString>GetDataBase();
     //delete lines with specific value 1 param colums, 2 value
    bool DeleteFromDatabase(QString,QString);
    ~SQLDbaseService();
};

#endif // SQLDBASESERVICE_H
