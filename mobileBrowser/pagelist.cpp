#include "pagelist.h"

#include <QDebug>

#include <QVariant>
#include <QVariantList>

#include <QStandardPaths>
#include <QFile>

#include <QtAlgorithms>


pageList::pageList(QObject *parent) : QObject(parent)
{

}

QVector<PageItem> pageList::items() const
{
    return mItems;
}

bool pageList::setItemAt(int index, const PageItem &item)
{
    if (index < 0 || index >= mItems.size())
        return false;

    const PageItem &oldItem = mItems.at(index);
    if (item.id == oldItem.id
            && item.addr == oldItem.addr
            && item.title == item.title)
        return false;

    mItems[index] = item;
    return true;
}

void pageList::addPage(QString addr)
{
    PageItem item;
    int moreValues = 0;
    for(int i = 0;i<mItems.count();i++){
        if(moreValues <= mItems[i].id){
            moreValues = mItems[i].id+1;
            qDebug()<<moreValues;
        }
    }
    item.id = moreValues;
    item.addr = addr;
    mItems.append(item);
    changeValue();
    qDebug()<<item.id;
}

/*
delete selected page
*/
void pageList::deletePage(int id)
{
    for (int i = 0;i<mItems.count();i++) {
        if(mItems[i].id == id){
            mItems.remove(i);
            changeValue();
            break;
        }
    }
}

/*

*/
void pageList::changeAddress(QString addr, int id,QString title)
{
    if(mItems.empty())
        mItems.append({0,""});
    for(int i = 0;i<mItems.count();i++){
        if(id == mItems[i].id){
            mItems[i].addr = addr;
            mItems[i].title = title;
            changeValue();
        }
    }
}

/*
save page (when application is closing)
*/

void pageList::savePage(QString fileName)
{
    auto pathToDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile fileSavePage(pathToDir+"/"+fileName);
    if(fileSavePage.open(QIODevice::WriteOnly)){
        QString context = "";
        if(!mItems.empty()){
            for(int i = 0;i<mItems.count();i++){
                context+=QString::number(mItems[i].id)+"~"+mItems[i].addr+";";
            }
            QByteArray contextSave = context.toUtf8();
            fileSavePage.write(contextSave);
            //qDebug()<<contextSave;
        }
        else{
            fileSavePage.remove();
        }
    }

}

/*
load saved page
*/

void pageList::loadPage(QString fileName)
{
    auto pathToDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile fileSavePage(pathToDir+"/"+fileName);
    if(fileSavePage.exists() && fileSavePage.open(QIODevice::ReadOnly)){
        QStringList data = QString::fromUtf8(fileSavePage.readAll()).split(";");
        for(int i = 0;i<data.count() - 1;i++){
            QStringList selfData = data[i].split("~");
            PageItem item;
            item.id = selfData[0].toInt();
            item.addr = selfData[1];
            mItems.append(item);
            qDebug()<<item.addr;
        }
    }
}

QVector<PageItem> pageList::getPages()
{
    return mItems;
}

int pageList::getNumberPages()
{
    return mItems.count();
}















