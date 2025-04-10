#ifndef MODIFYPROJECT_H
#define MODIFYPROJECT_H

#include <QDialog>
#include <QHBoxLayout>
#include <QAbstractButton>

class ModifyProject : public QDialog
{
public:
    ModifyProject();

private:
    int screenHeight;
    int screenWidth;
    QString projectName;

    int simplearray[31];
    int difficultarray[31];
    QMap<int, QPushButton*> simplebuttonMap;
    QMap<int, QPushButton*> difficulbuttonMap;
    void updateButtonStates();
    void updateMapBasedOnButtonStates();

    QHBoxLayout *projectlayout;
    QVBoxLayout *layout;
    void showProject();
    void initMap();
    void initButtonGroup();
    void saveData();

private slots:
    void onProjectButtonClicked(QAbstractButton *button);
    void onModifyButtonClicked();
    void onReturnButtonClicked();
};

#endif // MODIFYPROJECT_H
