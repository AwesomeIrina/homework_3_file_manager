#ifndef STRATEGY_H
#define STRATEGY_H
#include <iostream>
#include <QObject>

//Структура, для хранения необходимой информации
//Имя файла
//Размер файла
//процентное соотношение
struct fileSizeInfo{
    QString fileName;
    qint64  size;
    float percent;
    //Конструктор
    fileSizeInfo(QString path_ = "No path there", qint64 size_ = 0, float percent_ = 0.00){
        fileName = path_;
        size = size_;
        percent = percent_;
    }
};

//Класс для определения нужной стратегии
class Strategy
{
public:
    //Конструктор класса
    Strategy() {};
    //виртуальный десруктор
    virtual ~Strategy() {}
    //Функция для вычисления размеров содержимого в папке
    //На вход подаётся путь к папке, на выходе - имена файлов,
    //а также информацию о размерах и процентных соотношениях
    virtual QList<fileSizeInfo> SizeInfo(const QString &) = 0;
};

#endif // STRATEGY_H
