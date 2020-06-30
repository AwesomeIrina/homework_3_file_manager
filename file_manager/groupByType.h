#ifndef GROUPBYTYPE_H
#define GROUPBYTYPE_H
#include "strategy.h"
#include "QFileInfo"
#include "QDir"
#include "QMap"
#include "QTextStream"

using namespace std;

class GroupByType: public Strategy
{
private:
    void FolderSize(const QString&path, QMap<QString, qint32>&data);
    QString Type(const QString&path);
public:
    GroupByType() {};
    QList<fileSizeInfo> SizeInfo(const QString & path);
};

QString GroupByType::Type(const QString&path)
{
    if(QFileInfo(path).isDir())
    {
        return "folder";
    }

    if(QFileInfo(path).fileName().lastIndexOf('.') == -1)
    {
        return "unknown type";
    }

    else
    {
        return QFileInfo(path).fileName().mid(QFileInfo(path).fileName().lastIndexOf('.'));
    }
}

void GroupByType::FolderSize( const QString&path, QMap<QString, qint32>&data)
{
    foreach(QFileInfo inform, QDir(path).entryList(QDir::Dirs | QDir::NoDotAndDotDot| QDir::Hidden | QDir::System, QDir::Type))
    {
        FolderSize(inform.absoluteFilePath(), data);
    }

    foreach(QFileInfo inform, QDir(path).entryList(QDir::Files | QDir::NoDotAndDotDot| QDir::Hidden | QDir::System, QDir::Type))
    {
        QString name = Type(inform.fileName());
        qint32 size_ = inform.size();
        data[name] += size_;
    }
}

QList<fileSizeInfo>GroupByType::SizeInfo(const QString& path)
{
    QTextStream cout(stdout), cin(stdin);
    QList<fileSizeInfo> res;
    if (!QFileInfo(path).exists()){
        cout<<"There is no object like "<<path<<endl;
        return QList<fileSizeInfo>();
    }

    if (!QFileInfo(path).isReadable()){
        cout<<"The object "<<path<<" is not readable"<<endl;
        return QList<fileSizeInfo>();
    }

    if(QFileInfo(path).isDir()){
        if(QDir(path).isEmpty()){
            cout<<"The object "<<path<<" is empty"<<endl;
            return QList<fileSizeInfo>();
        }

        QMap<QString, qint32> byte;

        foreach(QFileInfo inform, QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System, QDir::Type))
        {
            FolderSize(inform.absoluteFilePath(), byte);
        }

        foreach(QFileInfo inform, QDir(path).entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System, QDir::Type))
        {
            QString name = Type(inform.fileName());
            qint32 size_ = inform.size();
            byte[name] += size_;
        }

        QStringList all_types;
        qint32 res_size = 0;

        for(auto it = byte.begin(); it != byte.end(); it++)
        {
            res_size += *it;
            all_types.append(it.key());
        }

        if(res_size ==0)
        {
            cout<<"The folder is empty"<<endl;
            return QList<fileSizeInfo>();
        }

        all_types.sort();

        for(int i = 0; i<all_types.size(); i++)
        {
            res.append(fileSizeInfo(all_types[i], byte[all_types[i]], ((float)byte[all_types[i]]/res_size) * 100));
        }
    }
    else
    {
        qint32 file_size = QFileInfo(path).size();
        res.append(fileSizeInfo(QFileInfo(path).fileName(), file_size, 100));
    }
    return res;
}


#endif // GROUPBYTYPE_H
