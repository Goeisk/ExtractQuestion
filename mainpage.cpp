#include "mainpage.h"
#include "extractpage.h"
#include "modifyproject.h"

#include <QDir>
#include <QGuiApplication>
#include <QLineEdit>
#include <QMessageBox>
#include <QScreen>
#include <QPushButton>
#include <QLabel>

MainPage::MainPage(QWidget *parent)
    : QMainWindow{parent}
{
    QScreen *screen = QGuiApplication::primaryScreen();
    screenHeight = screen->geometry().height();
    screenWidth = screen->geometry().width();
    this->resize(screenWidth*0.8,screenHeight*0.8);

    QVBoxLayout *vlayout = new QVBoxLayout;

    QLabel *titlelabel = new QLabel;
    titlelabel->setFont(QFont("华文行楷", 30));
    titlelabel->setText("<html><head/><body><p align=\"center\"><span style=\" \n"
                        "font-weight:700;\">请选择复试小组</span></p><p align=\"center\"></p></body></html>");
    titlelabel->setAlignment(Qt::AlignCenter);
    titlelabel->setFixedHeight(screenHeight*0.2);

    buttonLayout = new QHBoxLayout;

    QHBoxLayout *hlayout = new QHBoxLayout;

    QPushButton *addButton = new QPushButton;
    double zoomnumber = 0.03;
    addButton->setText("添加");
    addButton->setFixedSize(screenWidth*zoomnumber, screenHeight*zoomnumber);
    QPushButton *deleteBotton = new QPushButton;
    deleteBotton->setText("删除");
    deleteBotton->setFixedSize(screenWidth*zoomnumber, screenHeight*zoomnumber);
    QPushButton *modifyBotton = new QPushButton;
    modifyBotton->setText("修改");
    modifyBotton->setFixedSize(screenWidth*zoomnumber, screenHeight*zoomnumber);
    QPushButton *resetBotton = new QPushButton;
    resetBotton->setText("重置");
    resetBotton->setFixedSize(screenWidth*zoomnumber, screenHeight*zoomnumber);
    hlayout->addStretch(1);
    hlayout->addWidget(addButton);
    hlayout->addWidget(deleteBotton);
    hlayout->addWidget(modifyBotton);
    hlayout->addWidget(resetBotton);

    vlayout->addWidget(titlelabel);
    vlayout->addStretch(1);
    vlayout->addLayout(buttonLayout);
    vlayout->addStretch(2);
    vlayout->addLayout(hlayout);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(vlayout);
    setCentralWidget(centralWidget);

    connect(addButton, &QPushButton::pressed, this, &MainPage::onaddButtonPressed);
    connect(deleteBotton, &QPushButton::pressed, this, &MainPage::ondeleteButonPressed);
    connect(modifyBotton, &QPushButton::pressed, this, &MainPage::onmodifyButtonPressed);
    connect(resetBotton, &QPushButton::pressed, this, &MainPage::onresetButtonPressed);

    showProject();
}

MainPage::~MainPage()
{

}

void MainPage::onaddButtonPressed()
{
    QDialog dialog(this);
    dialog.setWindowTitle("添加小组");

    QLabel *label = new QLabel;
    label->setText("组名:");

    QLineEdit *lineEdit = new QLineEdit;

    // 创建确定和返回按钮
    QPushButton *okButton = new QPushButton("确定", &dialog);
    QPushButton *cancelButton = new QPushButton("返回", &dialog);

    // 布局管理
    QHBoxLayout *editLayout = new QHBoxLayout;
    editLayout->addWidget(label);
    editLayout->addWidget(lineEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(editLayout);
    mainLayout->addLayout(buttonLayout);
    dialog.setLayout(mainLayout);

    // 按钮点击信号连接
    connect(okButton, &QPushButton::clicked, this,[this,lineEdit,dialog = &dialog]() {
        QString newProjectName = lineEdit->text();
        if (!newProjectName.isEmpty()) {
            if(addProject(newProjectName)){
                QMessageBox::information(this, "提示", "添加"+newProjectName+"成功");
                dialog->accept();
                dialog->close();
            }else{
                QMessageBox::information(this, "提示", "添加"+newProjectName+"失败");
            }
        } else {
            QMessageBox::warning(this, "警告", "组名不能为空");
        }
    });
    connect(cancelButton, &QPushButton::clicked, this, [dialog = &dialog]() {
        dialog->close();
    });
    // 显示对话框并等待用户操作
    if (dialog.exec() == QDialog::Accepted) {
        // 处理对话框接受后的逻辑
        showProject();
    }
}

void MainPage::ondeleteButonPressed()
{
    QDialog dialog(this);
    dialog.setWindowTitle("删除小组");

    QLabel *label = new QLabel;
    label->setText("组名:");

    QComboBox *projectComboBox = new QComboBox;
    projectComboBox->setCurrentIndex(0);
    projectComboBox->setMaxVisibleItems(7);
    projectComboBox->setStyleSheet("QComboBox{combobox-popup:0;}");

    // 创建确定和返回按钮
    QPushButton *deleteButton = new QPushButton("删除", &dialog);
    QPushButton *deleteAllButton = new QPushButton("删除全部", &dialog);
    QPushButton *cancelButton = new QPushButton("返回", &dialog);

    // 布局管理
    QHBoxLayout *comboxLayout = new QHBoxLayout;
    comboxLayout->addWidget(label);
    comboxLayout->addWidget(projectComboBox);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(comboxLayout);
    mainLayout->addWidget(deleteButton);
    mainLayout->addWidget(deleteAllButton);
    mainLayout->addWidget(cancelButton);
    dialog.setLayout(mainLayout);
    showDialogProject(projectComboBox);

    connect(deleteButton, &QPushButton::clicked, this,[this,projectComboBox,dialog = &dialog]() {
        QString newProjectName = projectComboBox->currentText();
        if (deleteproject(newProjectName)) {
            QMessageBox::information(this, "提示", "删除"+newProjectName+"成功！");
            dialog->accept();
        } else {
            QMessageBox::information(this, "提示", "删除"+newProjectName+"失败！");
        }
    });
    connect(deleteAllButton, &QPushButton::clicked, this,[this,dialog = &dialog]() {
        if (deleteAllProject()) {
            QMessageBox::information(this, "提示", "删除所有小组成功！");
            dialog->accept();
        } else {
            QMessageBox::information(this, "提示", "删除所有小组失败！");
        }
    });
    connect(cancelButton, &QPushButton::clicked, this, [dialog = &dialog]() {
        dialog->close();
    });
    if (dialog.exec() == QDialog::Accepted) {
        showProject();
    }
}

void MainPage::onmodifyButtonPressed()
{
    ModifyProject *dialog = new ModifyProject;
    if (dialog->exec() == QDialog::Accepted) {
        showProject();
    }
}

void MainPage::onresetButtonPressed()
{
    QDialog dialog(this);
    dialog.setWindowTitle("重置题库");

    QLabel *label = new QLabel;
    label->setText("组名:");

    QComboBox *projectComboBox = new QComboBox;
    projectComboBox->setCurrentIndex(0);
    projectComboBox->setMaxVisibleItems(7);
    projectComboBox->setStyleSheet("QComboBox{combobox-popup:0;}");

    // 创建确定和返回按钮
    QPushButton *resetButton = new QPushButton("重置", &dialog);
    QPushButton *resetAllButton = new QPushButton("重置全部", &dialog);
    QPushButton *cancelButton = new QPushButton("返回", &dialog);

    // 布局管理
    QHBoxLayout *comboxLayout = new QHBoxLayout;
    comboxLayout->addWidget(label);
    comboxLayout->addWidget(projectComboBox);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(comboxLayout);
    mainLayout->addWidget(resetButton);
    mainLayout->addWidget(resetAllButton);
    mainLayout->addWidget(cancelButton);
    dialog.setLayout(mainLayout);
    showDialogProject(projectComboBox);

    connect(resetButton, &QPushButton::clicked, this,[this,projectComboBox,dialog = &dialog]() {
        QString newProjectName = projectComboBox->currentText();
        if (resetproject(newProjectName)) {
            QMessageBox::information(this, "提示", "重置"+newProjectName+"成功！");
            dialog->accept();
        } else {
            QMessageBox::information(this, "提示", "重置"+newProjectName+"失败！");
        }
    });
    connect(resetAllButton, &QPushButton::clicked, this,[this,dialog = &dialog]() {
        if (resetAllProject()) {
            QMessageBox::information(this, "提示", "重置所有小组成功！");
            dialog->accept();
        } else {
            QMessageBox::information(this, "提示", "重置所有小组失败！");
        }
    });
    connect(cancelButton, &QPushButton::clicked, this, [dialog = &dialog]() {
        dialog->close();
    });
    if (dialog.exec() == QDialog::Accepted) {
        showProject();
    }
}

void MainPage::onProjectButtonPressed(QString projectName)
{
    ExtractPage dialog(projectName);
    dialog.exec();
}

void MainPage::showProject()
{
    QLayoutItem* item;
    while ((item = buttonLayout->takeAt(0)) != nullptr) {
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
            QMessageBox::critical(nullptr, QString("错误"), QString("project文件夹创建失败"));
        }
    }
    QStringList projectFiles;
    projectFiles << "*.txt";
    QFileInfoList fileInfoList = dir.entryInfoList(projectFiles, QDir::Files);
    for (QFileInfo& fileInfo : fileInfoList)
    {
        QPushButton *button = new QPushButton(fileInfo.baseName(), this);
        button->setFixedSize(screenWidth*0.1,screenHeight*0.1);
        button->setFont(QFont("华文行楷", 26));
        buttonLayout->addWidget(button);
        connect(button, &QPushButton::pressed, this, [this,button](){
            onProjectButtonPressed(button->text());
        });
    }
}

void MainPage::showDialogProject(QComboBox* comBox)
{
    QDir dir("./project");
    if (!dir.exists())
    {
        if (!dir.mkpath(dir.absolutePath()))
        {
            QMessageBox::critical(nullptr, QString("错误"), QString("project文件夹创建失败"));
        }
    }
    QStringList projectFiles;
    projectFiles << "*.txt";
    QFileInfoList fileInfoList = dir.entryInfoList(projectFiles, QDir::Files);
    for (QFileInfo& fileInfo : fileInfoList)
    {
        comBox->addItem(fileInfo.baseName());
    }
}

bool MainPage::deleteproject(QString projectName)
{
    QFile file("./project/" + projectName + ".txt");
    return file.remove();
}

bool MainPage::deleteAllProject()
{
    bool res = true;
    QDir projectDir("./project");
    QStringList filters;
    filters << "*.txt";
    projectDir.setNameFilters(filters);
    QStringList fileList = projectDir.entryList();
    for (QString& fileName : fileList) {
        QString projectName = fileName.left(fileName.length() - 4);
        if (!deleteproject(projectName)) {
            res = false;
        }
    }
    return res;
}

bool MainPage::modifyproject(QString projectName)
{
    bool res = false;
    return res;
}

bool MainPage::resetproject(QString projectName)
{
    bool res = false;
    QFile file("./project/"+projectName+".txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        file.close();
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << projectName << "\n";
            QString str;
            for(int i = 1; i <= 30; i++){
                str.append(","+QString::number(i));
            }
            out << "简单题" + str << "\n";
            out << "困难题" + str;
            file.close();
            res = true;
        }
    }
    return res;
}

bool MainPage::resetAllProject()
{
    bool res = true;
    QDir projectDir("./project");
    QStringList filters;
    filters << "*.txt";
    projectDir.setNameFilters(filters);
    QStringList fileList = projectDir.entryList();
    for (QString& fileName : fileList) {
        QString projectName = fileName.left(fileName.length() - 4);
        if (!resetproject(projectName)) {
            res = false;
        }
    }
    return res;
}

bool MainPage::addProject(QString ProjectName)
{
    bool res = false;
    QDir dir("./project");
    if (!dir.exists())
    {
        if (!dir.mkpath(dir.absolutePath()))
        {
            QMessageBox::critical(nullptr, QString("错误"), QString("project文件夹创建失败"));
        }
    }
    QFile file("./project/"+ProjectName+".txt");
    if (file.open(QIODevice::WriteOnly  | QIODevice::Text))
    {
        QTextStream out(&file);
        out << ProjectName << "\n";
        QString str;
        for(int i=1;i<=30;i++){
            str.append(","+QString::number(i));
        }
        out << "简单题"+str<< "\n";
        out << "困难题"+str;
        file.close();
        res = true;
    }
    return res;
}


