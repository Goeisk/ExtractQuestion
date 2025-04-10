#include "modifyproject.h"

#include <QButtonGroup>
#include <QDir>
#include <QGuiApplication>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScreen>

ModifyProject::ModifyProject() {
    QScreen *screen = QGuiApplication::primaryScreen();
    screenHeight = screen->geometry().height();
    screenWidth = screen->geometry().width();
    this->resize(screenWidth*0.4,screenHeight*0.4);
    //setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
    this->setWindowTitle("修改复试小组题库");
    layout = new QVBoxLayout;

    projectlayout = new QHBoxLayout;
    layout->addStretch(1);
    layout->addLayout(projectlayout);

    initMap();
    initButtonGroup();

    QHBoxLayout *modifylayout = new QHBoxLayout;
    QPushButton *modifybutton = new QPushButton("修改", this);
    modifybutton->setFixedSize(screenWidth * 0.05, screenHeight * 0.05);
    modifybutton->setFont(QFont("华文行楷", 20));
    QPushButton *returnbutton = new QPushButton("返回", this);
    returnbutton->setFixedSize(screenWidth * 0.05, screenHeight * 0.05);
    returnbutton->setFont(QFont("华文行楷", 20));
    modifylayout->addStretch(1);
    modifylayout->addWidget(modifybutton);
    modifylayout->addStretch(1);
    modifylayout->addWidget(returnbutton);
    modifylayout->addStretch(1);

    layout->addLayout(modifylayout);
    layout->addStretch(1);
    setLayout(layout);

    showProject();
    updateButtonStates();
    connect(modifybutton, &QPushButton::pressed, this,&ModifyProject::onModifyButtonClicked);
    connect(returnbutton, &QPushButton::pressed, this,&ModifyProject::onReturnButtonClicked);
}

void ModifyProject::showProject()
{
    QLayoutItem* item;
    while ((item = projectlayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    QDir dir("./project");
    if (!dir.exists())
    {
        if (!dir.mkpath(dir.absolutePath()))
        {
            qDebug()<<"project文件夹创建失败";
        }
    }
    QStringList projectFiles;
    projectFiles << "*.txt";
    QFileInfoList fileInfoList = dir.entryInfoList(projectFiles, QDir::Files);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    for (QFileInfo& fileInfo : fileInfoList)
    {
        QPushButton *button = new QPushButton(fileInfo.baseName(), this);
        button->setFixedSize(screenWidth*0.05,screenHeight*0.05);
        button->setFont(QFont("华文行楷", 20));
        button->setCheckable(true);
        projectlayout->addWidget(button);
        buttonGroup->addButton(button);
    }
    connect(buttonGroup, &QButtonGroup::buttonClicked, this, &ModifyProject::onProjectButtonClicked);
}

void ModifyProject::initMap()
{
    for (int i = 1; i <= 30; ++i) {
        simplearray[i] = i;
        difficultarray[i] = i;
    }
}

void ModifyProject::initButtonGroup()
{
    QLabel *simplelabel = new QLabel;
    simplelabel->setText("简单题(B库)");
    simplelabel->setFont(QFont("华文行楷", 20));
    simplelabel->setAlignment(Qt::AlignCenter);

    QLabel *difficultlabel = new QLabel;
    difficultlabel->setText("困难题(A库)");
    difficultlabel->setFont(QFont("华文行楷", 20));
    difficultlabel->setAlignment(Qt::AlignCenter);

    layout->addStretch(1);
    layout->addWidget(difficultlabel);

    QHBoxLayout *currentRow = nullptr;
    for (int i = 1; i <= 30; ++i)
    {
        if (i % 10 == 1)
        {
            if (currentRow)
            {
                currentRow->addStretch(1);
                layout->addLayout(currentRow);
            }
            currentRow = new QHBoxLayout;
            currentRow->addStretch(1);
        }
        QPushButton *button = new QPushButton(QString::number(difficultarray[i]), this);
        difficulbuttonMap[i] = button;
        button->setFixedSize(screenWidth * 0.05, screenHeight * 0.05);
        button->setFont(QFont("宋体", 14));
        button->setCheckable(true);
        currentRow->addWidget(button);
    }
    if (currentRow) {
        currentRow->addStretch(1);
        layout->addLayout(currentRow);
    }

    layout->addStretch(1);
    layout->addWidget(simplelabel);

    currentRow = nullptr;
    for (int i = 1; i <= 30; ++i)
    {
        if (i % 10 == 1)
        {
            if (currentRow)
            {
                currentRow->addStretch(1);
                layout->addLayout(currentRow);
            }
            currentRow = new QHBoxLayout;
            currentRow->addStretch(1);
        }
        QPushButton *button = new QPushButton(QString::number(simplearray[i]+30), this);
        simplebuttonMap[i] = button;
        button->setFixedSize(screenWidth * 0.05, screenHeight * 0.05);
        button->setFont(QFont("宋体", 14));
        button->setCheckable(true);
        currentRow->addWidget(button);
    }
    if (currentRow) {
        currentRow->addStretch(1);
        layout->addLayout(currentRow);
    }
    layout->addStretch(1);
}

void ModifyProject::saveData()
{
    QString newsimple;
    newsimple.append("简单题");
    QString newdifficult;
    newdifficult.append("困难题");
    for (int i = 1; i <= 30; ++i) {
            newsimple.append(","+QString::number(simplearray[i]));
    }
    for (int i = 1; i <= 30; ++i) {
        newdifficult.append(","+QString::number(difficultarray[i]));
    }

    QFile file("./project/"+projectName+".txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QStringList lines;
        while (!in.atEnd()) {
            lines.append(in.readLine());
        }
        file.close();
        if (lines.size() >= 2) {
            lines[1] = newsimple;
            qDebug()<<"newsimple"<<newsimple;
        }
        if (lines.size() >= 3) {
            lines[2] = newdifficult;
            qDebug()<<"newdifficult"<<newdifficult;
        }
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QTextStream out(&file);
            for (QString& line : lines) {
                out << line << "\n";
            }
            file.close();
        }
    }
}

void ModifyProject::onProjectButtonClicked(QAbstractButton *button)
{
    projectName = button->text();
    QFile file("./project/"+projectName+".txt");
    QStringList stringList;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString line;

        while (!(line = in.readLine()).isNull())
        {
            stringList.append(line);
        }
        file.close();
    }
    if(stringList.length()>=3){
        QStringList simplelist = stringList[1].split(',');
        if (simplelist.size() >= 26) {
            for (int i = 1; i <= 30; ++i) {
                simplearray[i] = simplelist[i].toInt();
            }
        }
        QStringList difficultlist = stringList[2].split(',');
        if (difficultlist.size() >= 26) {
            for (int i = 1; i <= 30; ++i) {
                difficultarray[i] = difficultlist[i].toInt();
            }
        }
    }
    this->updateButtonStates();
}

void ModifyProject::onModifyButtonClicked()
{
    if(projectName.length()==0){
        QMessageBox::critical(nullptr, QString("错误"), QString("请先选择复试小组！"));
        return;
    }
    updateMapBasedOnButtonStates();
    saveData();
    QMessageBox::information(nullptr, QString("information"), QString("修改成功！"));
}

void ModifyProject::onReturnButtonClicked()
{
    this->close();
}

void ModifyProject::updateButtonStates()
{
    for (int i = 1; i <= 30; ++i) {
        if (simplearray[i] == 0) {
            if (simplebuttonMap.contains(i)) {
                simplebuttonMap[i]->setChecked(true);
            }
        }
        if (simplearray[i] == i) {
            if (simplebuttonMap.contains(i)) {
                simplebuttonMap[i]->setChecked(false);
            }
        }
    }

    for (int i = 1; i <= 30; ++i) {
        if (difficultarray[i] == 0) {
            if (difficulbuttonMap.contains(i)) {
                difficulbuttonMap[i]->setChecked(true);
            }
        }
        if (difficultarray[i] == i) {
            if (difficulbuttonMap.contains(i)) {
                difficulbuttonMap[i]->setChecked(false);
            }
        }
    }
}

void ModifyProject::updateMapBasedOnButtonStates()
{
    for (int i = 1; i <= 30; ++i) {
        if (simplebuttonMap.contains(i)) {
            QPushButton* button = simplebuttonMap[i];
            if (button->isChecked()) {
                simplearray[i] = 0;
            } else {
                simplearray[i] = i;
            }
        }
    }
    for (int i = 1; i <= 30; ++i) {
        if (difficulbuttonMap.contains(i)) {
            QPushButton* button = difficulbuttonMap[i];
            if (button->isChecked()) {
                difficultarray[i] = 0;
            } else {
                difficultarray[i] = i;
            }
        }
    }
}

