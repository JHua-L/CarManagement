#include "register_page.h"
#include<QLabel>
#include<QFont>
#include<QLineEdit>
#include<QSqlDatabase>
#include<QSqlError>
#include<QMessageBox>
#include<QDebug>
#include<QSqlQuery>


register_page::register_page(QWidget *parent) : QWidget(parent)
{
    Init();

    connect(btn_confirm,SIGNAL(clicked(bool)),this,SLOT(on_btn_confirm_clicked()));
    connectDatabase();

    connect(btn_return,&QPushButton::clicked,this,&register_page::sendSignal);
}


//-----------------------------------------------------------------------------------------------------functions--------------------------
void register_page::Init(){
    this->setWindowTitle("注册页面");
    //b.setParent(this);
    this->setMaximumHeight(350);
    this->setMinimumHeight(350);
    this->setMaximumWidth(500);
    this->setMinimumWidth(500);
    QLabel *lbl_register_title = new QLabel(this);
    lbl_register_title->setGeometry(170,20,311,81);
    lbl_register_title->setText("欢迎注册");
    lbl_register_title->setFont(QFont("楷体",22,50));

    QLabel *lbl_register_username = new QLabel(this);
    lbl_register_username->setGeometry(40,120,121,51);
    lbl_register_username->setText("请输入用户名：");
    lbl_register_username->setFont(QFont("楷体",10,50));



    QLabel *lbl_register_password = new QLabel(this);
    lbl_register_password->setGeometry(48,180,121,51);
    lbl_register_password->setText("请输入密码：");
    lbl_register_password->setFont(QFont("楷体",10,50));


    edit_username = new QLineEdit(this);
    edit_username->setGeometry(170,130,241,31);

    edit_password = new QLineEdit(this);
    edit_password->setGeometry(170,190,241,31);
    edit_password->setEchoMode(QLineEdit::Password);

    btn_confirm = new QPushButton(this);
    btn_confirm->setGeometry(92,270,111,41);
    btn_confirm->setText("确认");
    btn_confirm->setFont(QFont("楷体",12,50));
    btn_confirm->setStyleSheet("background-color:rgb(85, 255, 0)");

    btn_return = new QPushButton(this);
    btn_return->setGeometry(300,270,111,41);
    btn_return->setText("返回");
    btn_return->setFont(QFont("楷体",12,50));
    btn_return->setStyleSheet("background-color:rgb(255, 0, 0)");
}

void register_page::sendSignal(){
    emit signal_for_login();
}

void register_page::connectDatabase(){
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
//---------------------------------------------------------------------------------------------------end function--------------------------

//------------------------------------------------------------------------------------------------------events--------------------------

//---------------------------------------------------------------------------------------------------end event--------------------------

//------------------------------------------------------------------------------------------------------slots--------------------------
void register_page::on_btn_confirm_clicked(){
    //qDebug()<<"按键已按下！！";
    QString usernameT = edit_username->text();
    QString passwordT = edit_password->text();

    QSqlQuery *query = new QSqlQuery();
    query->exec("select password from login where username = '"+usernameT+"'");

    if(!query->next()){
        query->exec("insert into login values('"+usernameT+"','"+passwordT+"')");
        QMessageBox::information(this,"提示","注册成功！！");
        edit_username->clear();
        edit_password->clear();
    }else{
            QMessageBox::warning(this,"提示","用户名已存在！！");
    }
}

//----------------------------------------------------------------------------------------------------end slot--------------------------

