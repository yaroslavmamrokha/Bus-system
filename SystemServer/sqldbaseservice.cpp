#include "sqldbaseservice.h"

SQLDbaseService::SQLDbaseService()
{
    db_col_names.push_back(":id");
    db_col_names.push_back(":from");
    db_col_names.push_back(":to");
    db_col_names.push_back(":date");
    db_col_names.push_back(":station");
    db_col_names.push_back(":places");
}

SQLDbaseService::SQLDbaseService(const int)
{

}

void SQLDbaseService::ConnectBase()
{
    curr_database = QSqlDatabase::addDatabase("QMYSQL");
    curr_database.setHostName(host_name);
    curr_database.setPort(DB_PORT);
    curr_database.setDatabaseName(DBASE_NAME);
    curr_database.setUserName(DB_USER_NAME);
    curr_database.setPassword(DB_PASSWORD);
    if(!curr_database.open(DB_USER_NAME,DB_PASSWORD)){
       throw SQLException{"Failed to open database",ExcepCode::OPEN_FAILED};
    }
}

QString SQLDbaseService::BaseName()
{
    return curr_database.databaseName();
}

void SQLDbaseService::ChangeHostIp(QString new_ip)
{
    host_name = new_ip;
}


bool SQLDbaseService::UpdateDatabase(QString id, int column, QString value)
{
    QSqlQuery working_query;
    LoadDataToUpdateVector();
    value.push_front("'");
    value.push_back("'");
    update_data.at(column) = value;
    QString tmp1 = "UPDATE `bussql`.`buslines` SET `idBuslines` = "+update_data.at(DatabaseColumnNames::ID);
    QString tmp2 = ",`Fromdirection` ="+update_data.at(DatabaseColumnNames::SRC_CITY)+",`Todirectiom` = ";
    QString tmp3 = update_data.at(DatabaseColumnNames::DST_CITY)+",`Date` = "+update_data.at(DatabaseColumnNames::DATE_NAME);
    QString tmp4 = ",`station` = "+update_data.at(DatabaseColumnNames::STATION)+",`Places` = "+update_data.at(DatabaseColumnNames::PLACES);
    query_name = tmp1+tmp2+tmp3+tmp4+"WHERE `idBuslines` = "+id+";";
    if(working_query.exec(query_name)){
    }else{
        throw SQLException{"Failed to update value: "+curr_database.lastError().text(),ExcepCode::QUERY_FAILED};
    }
    return true;
}

void SQLDbaseService::LoadDataToUpdateVector()
{
    update_data.clear();
    update_data.push_back("`idBuslines`");
    update_data.push_back("`Fromdirection`");
    update_data.push_back("`Todirectiom`");
    update_data.push_back("`Date`");
    update_data.push_back("`station`");
    update_data.push_back("`Places`");
}

bool SQLDbaseService::InsertToDatabase(vector<QString> rq)
{
    QSqlQuery working_query;
    //set current query
    query_name = "INSERT INTO `bussql`.`buslines`(`idBuslines`,`Fromdirection`,`Todirectiom`,`Date`,`station`,`Places`) VALUES (:id,:to,:from,:date,:station,:places);";
    working_query.prepare(query_name);
    input_data.clear();
    input_data = rq;
    int i = 0;
    for(auto x : db_col_names){
    working_query.bindValue(x,input_data.at(i++));
    }
    //start execution of current query
    if(working_query.exec()){
    }else{
        throw SQLException{"Failed to insert value: "+curr_database.lastError().text(),ExcepCode::QUERY_FAILED};
    }
    return true;
}

vector<QString> SQLDbaseService::SearchInDatabase(QString row,QString recv_value)
{
    response_data.clear();
    QSqlQuery working_query;
     //set current query
    query_name = "SELECT idBuslines,Fromdirection,Todirectiom,Date,station,Places FROM bussql.buslines WHERE "+row+" LIKE '%"+recv_value+"%' LIMIT 100;";
    QString tmp_data;
       //start execution of current query
    if(working_query.exec(query_name)){
        while(working_query.next()){
            for(int i = 0; i< 6; i++)
                tmp_data = tmp_data+ working_query.value(i).toString()+"#";
            response_data.push_back(tmp_data);
            tmp_data.clear();
        }
    }else{
        throw SQLException{"No Data Found: "+curr_database.lastError().text(),ExcepCode::DATA_NOT_FOUND};
    }
    return response_data;
}

vector<QString> SQLDbaseService::GetDataBase()
{
    QSqlQuery working_query;
    response_data.clear();
     //set current query
    query_name = "SELECT * FROM bussql.buslines;";
    QString tmp_data;
     //start execution of current query
    if(working_query.exec(query_name)){
        while(working_query.next()){
            for(int i = 0; i< 6; i++)
                tmp_data = tmp_data+ working_query.value(i).toString()+"#";
            response_data.push_back(tmp_data);
            tmp_data.clear();
        }
    }else{
        throw SQLException{"No Data Found: "+curr_database.lastError().text(),ExcepCode::DATA_NOT_FOUND};
    }
    return response_data;
}

bool SQLDbaseService::DeleteFromDatabase(QString column, QString condition)
{
    QSqlQuery working_query;
     //set current query
    query_name = "DELETE FROM `bussql`.`buslines` WHERE "+column+" LIKE '"+condition+"';";
     //start execution of current query
    if(working_query.exec(query_name)){
    }else{
        throw SQLException{"Failed to delete value: "+curr_database.lastError().text(),ExcepCode::QUERY_FAILED};
    }
    return true;
}

SQLDbaseService::~SQLDbaseService()
{
    query_name.clear();
    update_data.clear();
    input_data.clear();
    response_data.clear();
    db_col_names.clear();
    curr_database.close();
}

const char *SQLException::what()
{
    return excep_info.toStdString().c_str();
}
