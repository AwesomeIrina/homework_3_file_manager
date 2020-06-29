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
    qint32 FolderSize(const QString&);
public:
    GroupByFolder() {};
    QList<fileSizeInfo> SizeInfo(const QString & path);
};

QList<fileSizeInfo> SizeInfo(const QString & path)
{
    QTextStream cout(stdout), cin(stdin);

    if (!QFileInfo(path).exists()){
        cout<<"There is no object like "<<path<<endl;
        return QList<fileSizeInfo>();
    }

    if (!QFileInfo(path).isReadable()){
        cout<<"The object "<<path<<" is not readable"<<endl;
        return QList<fileSizeInfo>();
    }

    if(QFileInfo(path).isDir() && !QFileInfo(path).isSymLink()){
        if(QFileInfo(path).dir().isEmpty()){
            cout<<"The folder "<< path<<" is empty"<< endl;
            return QList<fileSizeInfo>();
        }
    }





}


#endif // GROUPBYFOLDER_H
