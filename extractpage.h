#ifndef EXTRACTPAGE_H
#define EXTRACTPAGE_H

#include <QDialog>
#include <QLabel>
#include <QTimer>

class ExtractPage : public QDialog
{

public:
    ExtractPage(QString projectName);

private:
    int screenHeight;
    int screenWidth;
    int simpleNumber=0;
    int difficultNumber=0;
    int nonZeroCount=0;
    int nonZeroCount1=0;

    QString projectName;
    QLabel *informationLabel;
    QLabel *simpleLabel;
    QLabel *difficultLabel;
    QPushButton *startButton;
    QTimer *timer;
    int currentInterval;

    int simplearray[31];
    int difficultarray[31];

    void startLottery();
    void updateNumber();
    void stopLottery();

    void updatearray(QString projectName);
    void saveData(QString projectName);

    void onreturnBottonPressed();

};

#endif // EXTRACTPAGE_H
