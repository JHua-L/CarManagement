#ifndef MANAGEMENT_PAGE_H
#define MANAGEMENT_PAGE_H

#include <QMainWindow>
#include<QQueue>
#include<QColor>
#include<QMap>

namespace Ui {
class management_page;
}

class management_page : public QMainWindow
{
    Q_OBJECT

public:
    explicit management_page(QWidget *parent = 0);
    ~management_page();
    void connectDatabase();//连接数据库
    void InitData();//初始化数据

    void getData(QVariantList listName, QVariantList listNum,int totalSum);
    QBrush newBrush(); //从所有颜色队列退出获取新画刷
    QBrush getBrush(); //从已使用的颜色队列中退出并获取画刷
    void InitColorAndData();


private slots:
    void on_actionCar_triggered();

    void on_actionCalc_triggered();

    void on_comboBox_factory_currentIndexChanged(const QString &arg1);

    void on_comboBox_brand_currentIndexChanged(const QString &arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

    void on_comboBox_factory_new_currentIndexChanged(const QString &arg1);

    void on_btn_cancel_new_clicked();

    void on_btn_confirm_new_clicked();

    void on_btn_cancel_newK_clicked();

    void on_btn_confirm_newK_clicked();
protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::management_page *ui;
    QMap<QString, double> mapObject; //存放物品的名字和数量占比
    double ratioAngleToRadian = 3.1415 / 180; //角度转弧度
    double ratioRadianToAngle = 180 / 3.1415; //弧度转角度
    QFont fontTips; //右侧标注栏字体
    QFont fontPer; //扇形图百分比字体
    QQueue<QColor> usedColor; //存放该图使用的颜色
    QQueue<QColor> allColor; //存放提供的颜色
    QVariantList listName, listNum;

};

#endif // MANAGEMENT_PAGE_H
