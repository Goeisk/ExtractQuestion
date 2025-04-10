#include "extractpage.h"

#include <QFile>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QRandomGenerator>
#include <QScreen>

ExtractPage::ExtractPage(QString projectName)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    screenHeight = screen->geometry().height();
    screenWidth = screen->geometry().width();
    this->resize(screenWidth*0.8,screenHeight*0.8);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    this->showMaximized();

    this->setWindowTitle("复试题目抽取");
    this->projectName = projectName;

    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    QLabel *label1 = new QLabel("当前复试小组:");
    label1->setFont(QFont("华文行楷", 30));
    QLabel *label2 = new QLabel(projectName);

    label2->setFont(QFont("华文行楷", 30));
    hlayout1->addStretch(1);
    hlayout1->addWidget(label1);
    hlayout1->addWidget(label2);
    hlayout1->addStretch(1);

    QHBoxLayout *hlayout4 = new QHBoxLayout;
    QLabel *label3 = new QLabel("当前题库:A库");
    label3->setFont(QFont("华文行楷", 30));
    QLabel *label4 = new QLabel("当前题库:B库");
    label4->setFont(QFont("华文行楷", 30));
    hlayout4->addStretch(1);
    hlayout4->addWidget(label3);
    hlayout4->addStretch(1);
    hlayout4->addWidget(label4);
    hlayout4->addStretch(1);

    QHBoxLayout *hlayout5 = new QHBoxLayout;
    difficultLabel = new QLabel("第？题");
    difficultLabel->setFont(QFont("华文行楷", 30));
    simpleLabel = new QLabel("第？题");
    simpleLabel->setFont(QFont("华文行楷", 30));
    hlayout5->addStretch(1);
    hlayout5->addWidget(difficultLabel);
    hlayout5->addStretch(1);
    hlayout5->addWidget(simpleLabel);
    hlayout5->addStretch(1);

    QHBoxLayout *hlayout6 = new QHBoxLayout;
    informationLabel = new QLabel;
    informationLabel->setText("等待抽题");
    informationLabel->setFont(QFont("华文行楷", 26));
    informationLabel->setStyleSheet("color: red;");

    hlayout6->addStretch(1);
    hlayout6->addWidget(informationLabel);
    hlayout6->addStretch(1);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    startButton = new QPushButton;
    startButton->setText("开始抽题");
    startButton->setFont(QFont("华文行楷", 30));
    startButton->setFixedSize(screenWidth * 0.1, screenHeight * 0.1);
    hlayout2->addWidget(startButton);

    QHBoxLayout *hlayout3 = new QHBoxLayout;
    QPushButton *returnButton = new QPushButton;
    returnButton->setText("返回");
    returnButton->setFont(QFont("华文行楷", 20));
    returnButton->setFixedSize(screenWidth * 0.05, screenHeight * 0.05);
    hlayout3->addStretch(1);
    hlayout3->addWidget(returnButton);

    layout->addStretch(1);
    layout->addLayout(hlayout1);
    layout->addStretch(1);
    layout->addLayout(hlayout4);
    layout->addLayout(hlayout5);
    layout->addStretch(1);
    layout->addLayout(hlayout6);
    layout->addStretch(1);
    layout->addLayout(hlayout2);
    layout->addStretch(1);
    layout->addLayout(hlayout3);
    this->setLayout(layout);

    connect(startButton, &QPushButton::clicked, this, &ExtractPage::startLottery);
    connect(returnButton, &QPushButton::clicked, this, &ExtractPage::onreturnBottonPressed);

    // 创建定时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ExtractPage::updateNumber);
    // 初始时间间隔
    currentInterval = 50;

    updatearray(projectName);
}

void ExtractPage::startLottery()
{
    informationLabel->setText("正在抽题中...");
    // 禁用开始按钮
    startButton->setEnabled(false);
    // 启动定时器，使用初始时间间隔
    timer->start(currentInterval);
    // 设置定时器在 5 秒后停止
    QTimer::singleShot(4000, this, &ExtractPage::stopLottery);
}

void ExtractPage::updateNumber()
{
    //difficult(1~30)
    int arraySize = 31;
    int nonZeroIndices[30];
    nonZeroCount = 0;
    // 找出非零元素的索引
    for (int i = 1; i < arraySize; ++i) {
        if (difficultarray[i] != 0) {
            nonZeroIndices[nonZeroCount++] = i;
        }
    }
    if (nonZeroCount > 0) {
        // 随机选择一个非零元素的索引
        int randomIndex = QRandomGenerator::global()->bounded(nonZeroCount);
        int selectedIndex = nonZeroIndices[randomIndex];
        difficultNumber = difficultarray[selectedIndex];
        QString text = QString("第 <span style='font-weight:bold; color:red;'>%1</span> 题").arg(difficultNumber);
        difficultLabel->setText(text);
    }

    //simple(31~60)
    int arraySize1 = 31;
    int nonZeroIndices1[30];
    nonZeroCount1 = 0;
    // 找出非零元素的索引
    for (int i = 1; i < arraySize1; ++i) {
        if (simplearray[i] != 0) {
            nonZeroIndices1[nonZeroCount1++] = i;
        }
    }
    if (nonZeroCount1 > 0) {
        int randomIndex1 = QRandomGenerator::global()->bounded(nonZeroCount1);
        int selectedIndex1 = nonZeroIndices1[randomIndex1];
        simpleNumber = simplearray[selectedIndex1];
        QString text = QString("第 <span style='font-weight:bold; color:red;'>%1</span> 题").arg(simpleNumber+30);
        simpleLabel->setText(text);
    }
    currentInterval += 10;
    if (currentInterval > 200) {
        currentInterval = 200;
    }
    timer->setInterval(currentInterval);
}

void ExtractPage::stopLottery()
{
    // 停止定时器
    timer->stop();
    // 启用开始按钮
    currentInterval = 50;

    if(nonZeroCount && nonZeroCount1){
        informationLabel->setText("抽题完成！");
    }else if(nonZeroCount){
        informationLabel->setText("A库抽题完成，B库无可抽取题目！");
    }else if(nonZeroCount1){
        informationLabel->setText("A库无可抽取题目，B库抽取完成！");
    }else{
        informationLabel->setText("无可抽取题目！");
    }

    if(simpleNumber){
        simplearray[simpleNumber] = 0;
    }
    if(difficultNumber){
        difficultarray[difficultNumber] = 0;
    }
    saveData(projectName);
    startButton->setEnabled(true);
}

void ExtractPage::updatearray(QString projectName)
{
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
}

void ExtractPage::saveData(QString projectName)
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
        }
        if (lines.size() >= 3) {
            lines[2] = newdifficult;
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

void ExtractPage::onreturnBottonPressed()
{
    if (timer->isActive()) {
        timer->stop();
    }
    this->close();
}

