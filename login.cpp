#include "login.h"
#include "ui_login.h"
#include<QSqlDatabase>
#include<QSqlError>
#include<QMessageBox>
#include<QDebug>
#include<QSqlQuery>

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    connectDatabase();
    this->setWindowTitle("登陆界面");
    r.hide();

    connect(&r,&register_page::signal_for_login,this,&login::deal_signal_for_login);
}

login::~login()
{
    delete ui;
}

//-----------------------------------------------------------------------------------------------------functions--------------------------
void login::connectDatabase(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("root");
    db.setDatabaseName("car");

    if(!db.open()){
        QMessageBox::warning(this,"数据库打开失败！",db.lastError().text());
        return;
    }
}

void login::deal_signal_for_login(){
    this->show();
    r.hide();
}

//---------------------------------------------------------------------------------------------------end function--------------------------

//------------------------------------------------------------------------------------------------------events--------------------------

//---------------------------------------------------------------------------------------------------end event--------------------------

//------------------------------------------------------------------------------------------------------slots--------------------------
void login::on_btn_login_login_clicked()
{
    bool flag = false;
    QString usernameT = ui->lineEdit_login_username->text();
    QString passwordT = ui->lineEdit_login_password->text();

    QSqlQuery *query = new QSqlQuery();
    query->exec("select password from login where username = '"+usernameT+"'");

    if(!query->next()){
        QMessageBox::warning(this,"提示","用户名无效，请先注册！");
    }else{
        QString password = query->value(0).toString();
        if(password==passwordT){
            flag = true;
            QMessageBox::information(this,"提示","成功登录！！");
        }else{
            QMessageBox::warning(this,"提示","密码错误！！");
        }
    }
    if(flag){
        this->hide();
        m.show();
    }

}
void login::on_btn_login_register_clicked()
{
    this->hide();
    r.show();
}


//----------------------------------------------------------------------------------------------------end slot--------------------------


