#include "management_page.h"
#include "ui_management_page.h"

#include<QSqlDatabase>
#include<QSqlError>
#include<QMessageBox>
#include<QDebug>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include"docxml.h"
#include<QPainter>

management_page::management_page(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::management_page)
{
    ui->setupUi(this);
    on_actionCar_triggered();
    connectDatabase();
    InitData();

    DocXML::createXML("../carManagement/demo.xml");
//    QStringList list;
//    list<<"二汽神龙"<<"毕加索"<<"6"<<"6"<<"6";
//    DocXML::appendXML("../carManagement/demo.xml",list);



}

management_page::~management_page()
{
    delete ui;
}

//-----------------------------------------------------------------------------------------------------functions--------------------------
void management_page::connectDatabase(){
    QSqlDatabase db= QSqlDatabase::addDatabase("QMYSQL");


    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("root");
    db.setDatabaseName("car");
    if(!db.open()){
        QMessageBox::warning(this,"数据库打开失败！",db.lastError().text());
        return;
    }
}

void management_page::InitData(){
    QSqlQuery *query = new QSqlQuery();
    query->exec("select name from factory");
    while(query->next()){
       ui->comboBox_factory->addItem(query->value(0).toString());
       ui->comboBox_factory_new->addItem(query->value(0).toString());
   }
    ui->label_last->setText("0");
    ui->lineEdit_total->setEnabled(false);

}

QBrush management_page::newBrush(){
    QColor newColor = allColor.dequeue();
    allColor.enqueue(newColor);
    usedColor.enqueue(newColor);
    return QBrush(newColor);
}

QBrush management_page::getBrush(){
    QColor newColor = usedColor.dequeue();
    usedColor.enqueue(newColor);
    return QBrush(newColor);
}

//---------------------------------------------------------------------------------------------------end function--------------------------

//------------------------------------------------------------------------------------------------------events--------------------------
void management_page::paintEvent(QPaintEvent *event){
    bool isFirst; //真为原图重绘 假为绘制另一幅图
    if(usedColor.isEmpty()) isFirst = false; //原图重绘，画刷颜色从所有颜色队列中退出
    else isFirst = true; //绘制另一幅图，画刷颜色从已经取过的颜色队列中退出

    int leftWidth = this->width() / 4 * 3, leftHeight = this->height(); //扇形统计图左边扇形图所占的区域
    int minLen = qMin(leftWidth, leftHeight); //获取高和宽的最小值
    int radius = (minLen - 260) / 2; //扇形统计图的半径
    QPoint pointTopLeft; //扇形图绘制区域的左上角坐标
    if(minLen == leftWidth)
    {
        pointTopLeft.setX(130);
        pointTopLeft.setY((leftHeight - radius * 2) / 2);
    }
    else
    {
        pointTopLeft.setX((leftWidth - radius * 2) / 2);
        pointTopLeft.setY(130);
    }
    QRect rect(pointTopLeft.x(), pointTopLeft.y(), radius * 2, radius * 2); //绘制的区域

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); //抗锯齿

    qreal startAngle = 0; //起始角度
    int Count = 0; //已经绘制的扇形数量
    int Total = mapObject.size(); //物品种类的数量
    auto it = mapObject.begin();

    while(it != mapObject.end())
    {
        QPainterPath path;
        path.moveTo(pointTopLeft.x() + radius, pointTopLeft.y() + radius);

        double valuePer = it.value(); //物品的占比
        qreal Angle = qreal(valuePer * 360); //物品占比转化为扇形的角度

//绘制并填充扇形
        //从startAngle角度开始，逆时针绘制Angle度弧线以及始末点和中心的连线，形成一个透明的扇形
        path.arcTo(rect, startAngle, Angle);
        //获取画刷
        QBrush brush;
        if(!isFirst) brush = newBrush();
        else brush = getBrush();
        painter.fillPath(path, brush); //填充刚刚画的扇形

//绘制扇形和扇形占比文字之间的指向线
        qreal qrealCen = startAngle + Angle / 2; //扇形的中间角度
        double resSin = sin(qrealCen * ratioAngleToRadian);
        double resCos = cos(qrealCen * ratioAngleToRadian); //sin值和cos值，里面先转弧度
        double l1 = radius - 20, l2 = radius + 50; //指向线的始末点距离中心的长度
        double h1 = resSin * l1; //中心y - 指向线起点y
        double w1 = resCos * l1; //中心x - 指向线起点x
        double h2 = resSin * l2; //中心y - 指向线终点y
        double w2 = resCos * l2; //中心x - 指向线终点x
        QPoint pointStart(int(pointTopLeft.x() + radius + w1), int(pointTopLeft.y() + radius - h1)); //指向线起点
        QPoint pointEnd(int(pointTopLeft.x() + radius + w2), int(pointTopLeft.y() + radius - h2)); //指向线终点
        painter.drawLine(pointStart, pointEnd); //绘制指向线

//绘制扇形占比文字
        painter.setFont(fontPer);
        QString strPer = QString::number(valuePer * 100, 'f', 2) + "%"; //物品的占比百分率
        int labelHeight = 20, labelWidth = 80; //文本显示的区域高和宽
        if((qrealCen == 0.0 || qrealCen < 45) || (qrealCen == 315.0 || (qrealCen < 360 && qrealCen > 315)))
            painter.drawText(pointEnd.x(), pointEnd.y() - labelHeight / 2, labelWidth, labelHeight, Qt::AlignLeft, strPer);
        else if(qrealCen == 45.0 || qrealCen < 135)
            painter.drawText(pointEnd.x() - labelWidth / 2, pointEnd.y() - labelHeight, labelWidth, labelHeight, Qt::AlignCenter, strPer);
        else if(qrealCen == 135.0 || qrealCen < 225)
            painter.drawText(pointEnd.x() - labelWidth, pointEnd.y() - labelHeight / 2, labelWidth, labelHeight, Qt::AlignRight, strPer);
        else
            painter.drawText(pointEnd.x() - labelWidth / 2, pointEnd.y(), labelWidth, labelHeight, Qt::AlignCenter, strPer);

//绘制右侧标注栏
        int tipsWidth = 50, tipsHeidght = 25; // 标注栏颜色提示的宽和高
        int tipsTextWidth = 100, tipsTextHeidght = tipsHeidght; // 标注栏文字提示的区域宽和高
        int disTipToText = 10; //颜色和文字的距离
        int disTipToTip = qMin(80, qMax(30, this->height() / Total - tipsHeidght)); //每个标注栏提示之间的距离
        painter.setBrush(brush);
        painter.drawRect(leftWidth , disTipToTip + Count * (disTipToTip + tipsHeidght), tipsWidth, tipsHeidght); //颜色提示
        painter.setFont(fontTips);
        painter.drawText(leftWidth + tipsWidth + disTipToText, disTipToTip + Count * (disTipToTip + tipsHeidght),
                         tipsTextWidth, tipsTextHeidght, Qt::AlignLeft, it.key()); //文字提示

        startAngle += Angle; //起始角度改变
        it++; //迭代器增加
        Count++; //已经绘制的扇形增加
    }
    painter.setBrush(Qt::NoBrush);

}

//---------------------------------------------------------------------------------------------------end event--------------------------

//------------------------------------------------------------------------------------------------------slots--------------------------
//车辆管理页面
void management_page::on_actionCar_triggered()
{
    //切换到车辆管理
    ui->stackedWidget->setCurrentWidget(ui->car);
    ui->label->setText("车辆管理");
    int totalSum = 0;
    listName.clear();
    listNum.clear();
    getData(listName, listNum, totalSum);
    this->update();

}
//销售管理页面
void management_page::on_actionCalc_triggered()
{
    //切换到销售管理
    ui->stackedWidget->setCurrentWidget(ui->calc);
    ui->label->setText("销售统计");
    InitColorAndData();
}

void management_page::getData(QVariantList listName, QVariantList listNum,int totalSum){
    mapObject.clear();

    double Sum = totalSum;
    for(int i = 0; i < listNum.size(); i++)
    {
        mapObject.insert(listName[i].toString(), listNum[i].toDouble() / Sum);
    }
    usedColor.clear();
    update();

}

void management_page::InitColorAndData(){
    fontTips.setPixelSize(12);
    fontPer.setPixelSize(20);

    int totalSum =0;
    allColor.enqueue(QColor(255, 255, 255));    allColor.enqueue(QColor(0, 0, 0));
    allColor.enqueue(QColor(0, 255, 255));      allColor.enqueue(QColor(0, 128, 128));
    allColor.enqueue(QColor(255, 0, 0));        allColor.enqueue(QColor(128, 0, 0));
    allColor.enqueue(QColor(255, 0, 255));      allColor.enqueue(QColor(128, 0, 128));
    allColor.enqueue(QColor(0, 255, 0));        allColor.enqueue(QColor(0, 128, 0));
    allColor.enqueue(QColor(255, 255, 0));      allColor.enqueue(QColor(128, 128, 0));
    allColor.enqueue(QColor(0, 0, 255));        allColor.enqueue(QColor(0, 0, 128));
    allColor.enqueue(QColor(160, 160, 164));    allColor.enqueue(QColor(128, 128, 128));

    QSqlQuery *query = new QSqlQuery;
    QString sql = QString("select factory,name , sell from brand");
    query->exec(sql);
    while(query->next()){
        listName.append(query->value("factory").toString()+"-"+query->value("name").toString());
        listNum.append(query->value("sell").toInt());
        totalSum+=query->value("sell").toInt();

    }
    getData(listName, listNum, totalSum);

}

void management_page::on_comboBox_factory_currentIndexChanged(const QString &arg1)
{
    if(arg1=="请选择厂家"){
        //清空
        ui->comboBox_brand->clear();
        ui->lineEdit_price->clear();
        ui->label_last->setText("0");
        ui->lineEdit_total->clear();
        ui->spinBox->setValue(0);
        ui->spinBox->setEnabled(false);
        ui->pushButton_confirm->setEnabled(false);
    }else{
        ui->comboBox_brand->clear();
        QSqlQuery *query = new QSqlQuery;
        QString sql = QString("select name from brand where factory = '%1'").arg(arg1);
        query->exec(sql);

        while(query->next()){
            QString name = query->value(0).toString();
            ui->comboBox_brand->addItem(name);
        }
        ui->spinBox->setEnabled(true);
    }
}

void management_page::on_comboBox_brand_currentIndexChanged(const QString &arg1)
{
    QSqlQuery *query = new QSqlQuery;
    QString sql = QString("select price ,last from brand where factory = '%1' and name = '%2'").arg(ui->comboBox_factory->currentText()).arg(arg1);
    query->exec(sql);

    while(query->next()){
        int price = query->value("price").toInt();
        int last = query->value("last").toInt();

        ui->lineEdit_price->setText(QString::number(price));
        ui->label_last->setText(QString::number(last));
    }
}

void management_page::on_spinBox_valueChanged(int arg1)
{

    QString factoryStr = ui->comboBox_factory->currentText();
    QString brandStr = ui->comboBox_brand->currentText();


    if(0 == arg1){
        ui->pushButton_confirm->setEnabled(false);
    }else{
        ui->pushButton_confirm->setEnabled(true);
    }

    QSqlQuery *query = new QSqlQuery();
    QString sql = QString("select sum ,last from brand where factory = '%1' and name = '%2'").arg(factoryStr).arg(brandStr);

    query->exec(sql);
    int last;
    while(query->next()){
        last = query->value("last").toInt();
    }

    if(last<arg1){
        ui->spinBox->setValue(last);
        return;
    }
    if(arg1==0){
        return;
    }
    int tempNum = last - arg1;

    ui->label_last->setText(QString::number(tempNum));
    int price = ui->lineEdit_price->text().toInt();
    int sum = price * arg1;
    ui->lineEdit_total->setText(QString::number(sum));


}

void management_page::on_pushButton_confirm_clicked()
{
    //获取信息
    //1、销售数据
    int num = ui->spinBox->value();
    //2、剩余
    int last = ui->label_last->text().toInt();

    //获取数据库销量
    QSqlQuery *query = new QSqlQuery();
    QString sql = QString("select sell from brand where factory = '%1' and name = '%2'")
            .arg(ui->comboBox_factory->currentText())
            .arg(ui->comboBox_brand->currentText());
    query->exec(sql);
    int sell;
    while(query->next()){
        sell = query->value("sell").toInt();
    }
    //更新数据库
    sell += num;
    sql = QString("update brand set sell = %1 ,last = %2 where factory = '%3' and name = '%4'")
            .arg(sell)
            .arg(last)
            .arg(ui->comboBox_factory->currentText())
            .arg(ui->comboBox_brand->currentText());
    query->exec(sql);

    QStringList list;
    list<<ui->comboBox_factory->currentText()
       <<ui->comboBox_brand->currentText()
      <<ui->lineEdit_price->text()
     <<QString::number(num)
    <<ui->lineEdit_total->text();
    DocXML::appendXML("../carManagement/demo.xml",list);

    QStringList fList;
    QStringList bList;
    QStringList pList;
    QStringList nList;
    QStringList tList;
    DocXML::readXML("../carManagement/demo.xml",fList,bList,pList,nList,tList);

    for(int i=0;i<fList.size();i++){
        QString str = QString("%1：%2：卖出了%3，单价：%4，总价：%5").
                arg(fList.at(i)).
                arg(bList.at(i)).
                arg(pList.at(i)).
                arg(nList.at(i)).
                arg(tList.at(i));
        //qDebug()<<str.toUtf8().data();
        ui->textEdit->append(str);
    }


    ui->pushButton_confirm->setEnabled(false);
    on_pushButton_cancel_clicked();


}

void management_page::on_pushButton_cancel_clicked()
{
    on_comboBox_factory_currentIndexChanged("请选择厂家");
    ui->comboBox_factory->setCurrentIndex(0);
    ui->label_last->setText("0");
}

void management_page::on_comboBox_factory_new_currentIndexChanged(const QString &arg1)
{
    if(arg1=="请选择厂家"){
        //清空
        ui->comboBox_brand_new->clear();
        ui->spinBox_new->setValue(0);
        ui->spinBox_new->setEnabled(false);
        ui->btn_confirm_new->setEnabled(false);
    }else{
        ui->comboBox_brand_new->clear();
        QSqlQuery *query = new QSqlQuery;
        QString sql = QString("select name from brand where factory = '%1'").arg(arg1);
        query->exec(sql);

        while(query->next()){
            QString name = query->value(0).toString();
            ui->comboBox_brand_new->addItem(name);
        }
        ui->spinBox_new->setEnabled(true);
        ui->btn_confirm_new->setEnabled(true);
    }
}

void management_page::on_btn_cancel_new_clicked()
{
    on_comboBox_factory_new_currentIndexChanged("请选择厂家");
    ui->comboBox_factory_new->setCurrentIndex(0);
}

void management_page::on_btn_confirm_new_clicked()
{
    QString factoryStr = ui->comboBox_factory_new->currentText();
    QString brandStr = ui->comboBox_brand_new->currentText();

    int num = ui->spinBox_new->value();

    QSqlQuery *query = new QSqlQuery;
    QString sql = QString("select sum,last from brand where factory = '%1' and name = '%2'")
            .arg(factoryStr)
            .arg(brandStr);

    query->exec(sql);
    query->next();
    int sum,last;
    sum =query->value("sum").toInt();
    last = query->value("last").toInt();

    sum += num;
    last += num;

    sql = QString("update brand set sum = %1 ,last = %2 where factory = '%3' and name = '%4'").
            arg(sum).
            arg(last).
            arg(factoryStr).
            arg(brandStr);
    query->exec(sql);

    ui->btn_confirm_new->setEnabled(false);
    on_btn_cancel_new_clicked();


}

void management_page::on_btn_cancel_newK_clicked()
{
    ui->edit_factory_newK->clear();
    ui->edit_brand_newK->clear();
    ui->edit_price_newK->clear();
    ui->edit_num_newK->clear();
}

void management_page::on_btn_confirm_newK_clicked()
{
    QString factoryStr = ui->edit_factory_newK->text();
    QString brandStr = ui->edit_brand_newK->text();
    int price = ui->edit_price_newK->text().toInt();
    int num = ui->edit_num_newK->text().toInt();
    int last = num;
    int sell = 0;
    QSqlQuery *query = new QSqlQuery();
    QString sql = QString("select count(*) from brand");
    query->exec(sql);
    query->next();

    int row;
    row = query->value(0).toInt();
    row++;

    sql = QString("insert brand values(%1,'%2','%3',%4,%5,%6,%7)").
            arg(row).
            arg(factoryStr).
            arg(brandStr)
            .arg(price)
            .arg(num)
            .arg(sell)
            .arg(last);
    query->exec(sql);

    on_btn_cancel_newK_clicked();

}

//----------------------------------------------------------------------------------------------------end slot--------------------------


