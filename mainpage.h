#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QMainWindow>
#include <QComboBox>
#include <QHBoxLayout>

class MainPage : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();
    bool addProject(QString ProjectName);

private slots:

private:
    int screenHeight;
    int screenWidth;
    QHBoxLayout *buttonLayout;

    void onaddButtonPressed();
    void ondeleteButonPressed();
    void onmodifyButtonPressed();
    void onresetButtonPressed();
    void onProjectButtonPressed(QString projectName);

    void showProject();
    void showDialogProject(QComboBox* comBox);
    bool deleteproject(QString projectName);
    bool deleteAllProject();
    bool modifyproject(QString projectName);
    bool resetproject(QString projectName);
    bool resetAllProject();

};

#endif // MAINPAGE_H
