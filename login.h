#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include"register_page.h"
#include"management_page.h"


namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

    //connect database
    void connectDatabase();//连接数据库

    void deal_signal_for_login();//处理来自注册页面的信号

private slots:
    void on_btn_login_login_clicked();//登录按键

    void on_btn_login_register_clicked();

private:
    Ui::login *ui;
    register_page r;
    management_page m;

};

#endif // LOGIN_H
