#ifndef DOCXML_H
#define DOCXML_H
#include<QString>
#include<QStringList>
#include<QDomDocument>
#include<QDomElement>


class DocXML
{
public:
    DocXML();

    static void createXML(QString filePath);//创建XML文件

    static void appendXML(QString filePath,QStringList list);

    static void writeXML(QDomDocument &doc,QDomElement &root,QStringList &list);

    static void readXML(QString filePath,
                        QStringList &fList,
                        QStringList &bList,
                        QStringList &pList,
                        QStringList &nList,
                        QStringList &tList);
};

#endif // DOCXML_H
