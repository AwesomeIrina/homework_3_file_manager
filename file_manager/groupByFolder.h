#ifndef GROUPBYFOLDER_H
#define GROUPBYFOLDER_H
#include "strategy.h"
#include "QTextStream"
#include "QFileInfo"
#include "QDir"

using namespace std;

class GroupByFolder: public Strategy
{
private:
    qint32 FolderSize(const QString&path);
public:
    GroupByFolder() {};
    QList<fileSizeInfo> SizeInfo(const QString & path);
};

qint32 GroupByFolder::FolderSize(const QString &path){
    qint32 res = 0;
    foreach (QFileInfo size, QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden))
    {
        res+= FolderSize(size.absoluteFilePath());
    }

    foreach(QFileInfo result, QDir(path).entryInfoList(QDir::Files | QDir::NoDotAndDotDot| QDir::Hidden))
    {
        res+= result.size();
    }

    return res;
}

QList<fileSizeInfo> GroupByFolder:: SizeInfo(const QString & path)
{
    QTextStream cout(stdout), cin(stdin);
    QList<fileSizeInfo> res;
    QList<qint32> size_;
    qint32 res_size = 0;

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

        foreach(QFileInfo size, QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir:: System, QDir::Name))
        {
            size_.append(FolderSize(size.absoluteFilePath()));
            res_size += size_[size_.size()-1];
        }

        foreach(QFileInfo size, QDir(path).entryInfoList(QDir::Files | QDir::NoDotAndDotDot|QDir::Hidden | QDir:: System, QDir::Name)){
            size_.append(size.size());
            res_size += size_[size_.size()-1];
        }

        if(res_size == 0){
            cout<<"The object "<<path<<" is empty";
            return QList<fileSizeInfo>();
        }

        auto iter = size_.begin();

        foreach(QFileInfo folder_info, QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot|QDir::Hidden | QDir:: System, QDir::Name))
        {
        if(folder_info.fileName().mid(folder_info.fileName().lastIndexOf('.') + 1) == "lnk")
        {
            continue;
        }

        res.append(fileSizeInfo(folder_info.fileName(), *iter, ((float)*iter/res_size)*100));
        iter++;
        }

        foreach(QFileInfo file, QDir(path).entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir:: Hidden | QDir:: System, QDir::Name))
        {
            res.append(fileSizeInfo(file.fileName(), *iter, ((float)*iter/res_size)*100));
            iter++;
        }
    }

    else
    {
        cout<<"Files: "<<endl;
        qint32 file_size = QFileInfo(path).size();
        res.append(fileSizeInfo(QFileInfo(path).fileName(), file_size, 100));
    }

    return res;
}



#endif // GROUPBYFOLDER_H
