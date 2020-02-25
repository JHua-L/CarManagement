#include "docxml.h"
#include<QFile>
#include<QDomDocument>
#include<QDomProcessingInstruction>
#include<QDomElement>
#include<QDebug>
#include<QTextStream>
#include<QDateTime>

DocXML::DocXML()
{

}

void DocXML::createXML(QString filePath){
    QFile file(filePath);
    if(file.exists()){
        return;
    }else{
        bool isOK = file.open(QIODevice::WriteOnly);
        if(isOK){
            //创建文档对象
            QDomDocument doc;
            //创建XML头部格式<?xml version="1.0" encoding="UTF-8"?>
            QDomProcessingInstruction ins;
            ins = doc.createProcessingInstruction("xml","version=\'1.0\' encoding=\'UTF-8\'");
            doc.appendChild(ins);
            //根节点元素
            QDomElement root = doc.createElement("日销售清单");
            doc.appendChild(root);
            //保存
            QTextStream stream(&file);
            doc.save(stream,4);//4 为缩进字符



        }else{
            qDebug()<<"WriteOnly error!";
        }
    }
}

void DocXML::appendXML(QString filePath, QStringList list){
    QFile file(filePath);
    bool isOK = file.open(QIODevice::ReadOnly);
    if(isOK){
        //file与xml文档对象关联
        QDomDocument doc;
        isOK = doc.setContent(&file);
        if(isOK){
            file.close();//关闭文件
            QDomElement root = doc.documentElement();
            QDateTime date = QDateTime::currentDateTime();
            QString dateStr = date.toString("yyyy-MM-dd");//2020-01-01

            //判断根节点下有没有子节点
            if(root.hasChildNodes()){//有子节点
                //查找最后一个子节点
                QDomElement lastElement = root.lastChildElement();
                if(lastElement.attribute("date") == dateStr){
                    //有当天日期
                     writeXML(doc,lastElement,list);

                }else{
                    //没有当天日期
                    //创建日期节点元素
                    QDomElement dateElement = doc.createElement("日期");
                    //创建date属性
                    QDomAttr dateAttr =  doc.createAttribute("date");
                    //设置属性的值
                    dateAttr.setNodeValue(dateStr);
                    //节点与属性关联
                    dateElement.setAttributeNode(dateAttr);
                    //把日期节点追加到根节点上
                    root.appendChild(dateElement);

                    //写有效数据
                    writeXML(doc,dateElement,list);

                }
            }else{//无子节点
                //创建日期节点元素
                QDomElement dateElement = doc.createElement("日期");
                //创建date属性
                QDomAttr dateAttr =  doc.createAttribute("date");
                //设置属性的值
                dateAttr.setNodeValue(dateStr);
                //节点与属性关联
                dateElement.setAttributeNode(dateAttr);
                //把日期节点追加到根节点上
                root.appendChild(dateElement);

                //写有效数据
                writeXML(doc,dateElement,list);


            }
            isOK = file.open(QIODevice::WriteOnly);
            if(isOK){
                QTextStream stream(&file);
                doc.save(stream,4);
                file.close();
            }
        }else{
            qDebug()<<"setContent error!";
        }
    }
}

void DocXML::writeXML(QDomDocument &doc, QDomElement &root, QStringList &list){
    //获取当前时间
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("hh-mm-ss");//14:17:33
    //创建时间节点元素
    QDomElement timeElement = doc.createElement("时间");
    //创建属性
    QDomAttr timeAttr =  doc.createAttribute("time");
    //给属性赋值
    timeAttr.setNodeValue(timeStr);
    //时间节点元素与属性相联
    timeElement.setAttributeNode(timeAttr);

    //把时间节点追加到日期节点后
    root.appendChild(timeElement);

    QDomElement factory = doc.createElement("厂家");
    QDomElement brand = doc.createElement("品牌");
    QDomElement price = doc.createElement("报价");
    QDomElement num = doc.createElement("数量");
    QDomElement total = doc.createElement("金额");

    QDomText text = doc.createTextNode(list.at(0));
    factory.appendChild(text);
    text = doc.createTextNode(list.at(1));
    brand.appendChild(text);
    text = doc.createTextNode(list.at(2));
    price.appendChild(text);
    text = doc.createTextNode(list.at(3));
    num.appendChild(text);
    text = doc.createTextNode(list.at(4));
    total.appendChild(text);

    timeElement.appendChild(factory);
    timeElement.appendChild(brand);
    timeElement.appendChild(price);
    timeElement.appendChild(num);
    timeElement.appendChild(total);

}

void DocXML::readXML(QString filePath, QStringList &fList, QStringList &bList, QStringList &pList, QStringList &nList, QStringList &tList){
    QFile file(filePath);
    bool isOK = file.open(QIODevice::ReadOnly);
    if(isOK){
        //file与xml文档对象关联
        QDomDocument doc;
        isOK = doc.setContent(&file);
        if(isOK){
            //获取根节点
            QDomElement root = doc.documentElement();
            file.close();
            QDateTime date = QDateTime::currentDateTime();
            QString dateStr = date.toString("yyyy-MM-dd");

            if(root.hasChildNodes()){
                QDomElement lastElement = root.lastChildElement();
                if(lastElement.attribute("date") == dateStr){
                    //有当天日期
                    //找出当天日期所有时间节点
                    QDomNodeList list = lastElement.childNodes();
                    for(int i=0;i<list.size();i++){
                        //找出时间节点下的所有子节点
                        QDomNodeList subList = list.at(i).toElement().childNodes();
                        //厂家
                        QString factory = subList.at(0).toElement().text();
                        fList.append(factory);
                        //品牌
                        QString brand = subList.at(1).toElement().text();
                        bList.append(brand);
                        //报价
                        QString price = subList.at(2).toElement().text();
                        pList.append(price);
                        //数量
                        QString num = subList.at(3).toElement().text();
                        nList.append(num);
                        //金额
                        QString total = subList.at(4).toElement().text();
                        tList.append(total);
                    }
                }else{
                    qDebug()<<"@@没有当天日期";
                }

            }else{
                qDebug("无子节点");
                return;
            }
        }else{
            qDebug()<<"setContent error!";
            return;
        }
    }else{
        qDebug()<<"readOnly error!";
        return;
    }
}
