#ifndef TESTS_H
#define TESTS_H
#include "strategy.h"
#include "groupByFolder.h"
#include "groupByType.h"
#include "QString"

class Tests
{
private:
    Strategy *choose_strategy;
    QString console(QList<fileSizeInfo> info)
    {
        QString res;
        for(auto it = info.begin(); it != info.end(); it++)
        {
            res+= it -> fileName + ", size in percents: ";
            if(it->percent < 0.01 && it->percent != 0)
            {
                res += "<0.01%\n";
            }
            else
            {
                res+= QString::number(it->percent) + "%\n";
            }
        }
        return res;
    }
public:
    explicit Tests()
    {
        QTextStream cout(stdout), cin(stdin);
        QString path("C:/QtProjects/homework_3_file_manager/Tests/Test1");
        for(int i = 1; i<6; i++)
        {
            if(i == 5)
            {
                path[48] = QChar('0' + i);
                path.append(".txt");
            }
            else
            {
                path[48] = QChar('0' + i);
            }

            cout<<path<<endl;
            choose_strategy = new GroupByFolder();
            cout<<"Group by folder:"<<endl;
            cout<<console(choose_strategy->SizeInfo(path))<<endl;
            delete choose_strategy;
            choose_strategy = new GroupByType();
            cout<<"Group by type:"<<endl;
            cout<<console(choose_strategy->SizeInfo(path))<<endl;
            delete choose_strategy;
        }

    }
};

#endif // TESTS_H
