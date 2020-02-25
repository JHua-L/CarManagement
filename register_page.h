#ifndef REGISTER_PAGE_H
#define REGISTER_PAGE_H

#include <QWidget>
#include<QPushButton>
#include<QLineEdit>


class register_page : public QWidget
{
    Q_OBJECT
public:
    explicit register_page(QWidget *parent = 0);
    void Init();//初始化函数

    void connectDatabase();//连接数据库

    void sendSignal();//往login发送信号
signals:
    void signal_for_login();
public slots:
    void on_btn_confirm_clicked();
private:
    QLineEdit *edit_username;
    QLineEdit *edit_password;
    QPushButton *btn_confirm;
    QPushButton *btn_return;


};

#endif // REGISTER_PAGE_H
