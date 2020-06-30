#ifndef GROUPBYTYPE_H
#define GROUPBYTYPE_H
#include "strategy.h"
#include "QFileInfo"
#include "QDir"
#include "QMap"
#include "QTextStream"

using namespace std;

//Класс для группировки по типу данных
class GroupByType: public Strategy
{
private:
    //Вычисление размера вложенной папки
    //На вход подаётся путь, а также QMap - контейнер, хранящий информацию о размере,
    //который занимает в папке каждый тип данных
    void FolderSize(const QString&path, QMap<QString, qint32>&data);
    //Функция, которая определяет тип файла
    //На вход подаётся путь
    //На выходе - тип файла
    QString Type(const QString&path);
public:
    //конструктор
    GroupByType() {};
    //Функция для вычисления размеров элементов, находящихся в папке
    //На вход подаётся путь к файлу
    //На выходе - информация вида: типи и занимаемое в папке место
    QList<fileSizeInfo> SizeInfo(const QString & path);
};

QString GroupByType::Type(const QString&path)
{
    //Проверки на то, какой тип файла
    //Директория
    if(QFileInfo(path).isDir())
    {
        return "folder";
    }
    //Неизвестный тип файла
    if(QFileInfo(path).fileName().lastIndexOf('.') == -1)
    {
        return "unknown type";
    }
    //Любой другой тип
    else
    {
        return QFileInfo(path).fileName().mid(QFileInfo(path).fileName().lastIndexOf('.'));
    }
}

void GroupByType::FolderSize( const QString&path, QMap<QString, qint32>&data)
{
    //Цикл по всем папкам в текущей папке
    foreach(QFileInfo inform, QDir(path).entryList(QDir::Dirs | QDir::NoDotAndDotDot| QDir::Hidden | QDir::System, QDir::Type))
    {
        FolderSize(inform.absoluteFilePath(), data);
    }
    //Цикл по всем файлам в папке
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
    //Проверка на существование
    if (!QFileInfo(path).exists()){
        cout<<"There is no object like "<<path<<endl;
        return QList<fileSizeInfo>();
    }
    //Проверка на читабельность
    if (!QFileInfo(path).isReadable()){
        cout<<"The object "<<path<<" is not readable"<<endl;
        return QList<fileSizeInfo>();
    }
    //Если на вход была подана директория
    if(QFileInfo(path).isDir()){
        //Если папка пуста, пользователь получает соответствующее сообщение
        if(QDir(path).isEmpty()){
            cout<<"The object "<<path<<" is empty"<<endl;
            return QList<fileSizeInfo>();
        }
        //Размер в байтах дл каждого типа
        QMap<QString, qint32> byte;
        //здесь начинается вычисление размеров объектов
        //цикл по всем папкам в текущей папке
        foreach(QFileInfo inform, QDir(path).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System, QDir::Type))
        {
            FolderSize(inform.absoluteFilePath(), byte);
        }
        //Цикл по всем файлам в папке
        foreach(QFileInfo inform, QDir(path).entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System, QDir::Type))
        {
            QString name = Type(inform.fileName());
            qint32 size_ = inform.size();
            byte[name] += size_;
        }
        //Список типов
        QStringList all_types;
        //Результирующий размер
        qint32 res_size = 0;
        //в цикле выполняется вычисление размера папки, а также массив типов заполняется
        for(auto it = byte.begin(); it != byte.end(); it++)
        {
            res_size += *it;
            all_types.append(it.key());
        }
        //Если папка пуста, пользователь получает соответствующее сообщение
        if(res_size ==0)
        {
            cout<<"The folder is empty"<<endl;
            return QList<fileSizeInfo>();
        }
        //Сортировка типов по названиям
        all_types.sort();
        //перевод в процентное соотношение имеющихся результатов
        for(int i = 0; i<all_types.size(); i++)
        {
            res.append(fileSizeInfo(all_types[i], byte[all_types[i]], ((float)byte[all_types[i]]/res_size) * 100));
        }
    }
    //Если файл не является папкой, попадаем в эту часть программы
    else
    {
        //Вычисление размера файла
        qint32 file_size = QFileInfo(path).size();
        res.append(fileSizeInfo(QFileInfo(path).fileName(), file_size, 100));
    }
    return res;
}


#endif // GROUPBYTYPE_H
