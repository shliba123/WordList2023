#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDialog"
#include "QDir"
#include "QMessageBox"
#include "QFileDialog"
#include "iostream"
#include "unordered_set"
#include "cstdio"
#include "util.h"

void exceptionHandler(QString qstring) {
    //创建消息框

    QMessageBox MyBox;
    MyBox.setWindowTitle("异常提示");
    MyBox.setText(qstring);
    MyBox.addButton("确定", QMessageBox::AcceptRole);
    MyBox.exec();
}

char* words[MAX_WORD];
char* result[MAX_WORD];
int wordLen;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

        ui->setupUi(this);


   }


MainWindow::~MainWindow()
{
    delete ui;
}



int MainWindow::readWords(char *inputFilePath, char *words[]) {
    FILE * file;
    file = fopen( inputFilePath, "rb");

    fseek(file, 0, SEEK_END);
    int len = (int)ftell(file);
    auto context = (char*)malloc(len + 1);
    fseek(file, 0, SEEK_SET);
    auto readLen = fread(context, 1, (size_t) len + 1, file);
    context[readLen] = '\0';
    fclose(file);
    std::unordered_set<std::string> wordSet;
    int cnt = 0;
    bool flag = false;
    int prev = 0;
    QString qstring;
    for (int i = 0; i < readLen; ++i) {
        qstring += context[i];
        if (isalpha(context[i])) {
            if (!flag) {
                flag = true;
                prev = i;
            }
            context[i] = (char) tolower(context[i]);
        } else {
            context[i] = '\0';
            auto currentWord = std::string(&context[prev]);
            if (flag && !wordSet.count(currentWord)) {
                wordSet.insert(currentWord);
                words[cnt++] = &context[prev];
            }
            flag = false;
        }

    }
    ui->textEdit_2->setText(qstring);
    if (flag) {
        context[readLen] = '\0';
        auto currentWord = std::string(&context[prev]);
        if (flag && !wordSet.count(currentWord)) {
            wordSet.insert(currentWord);
            words[cnt++] = &context[prev];
        }
    }
    return cnt;
}







void MainWindow::on_pushButton_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);

    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("选择文件"));

    //设置打开的文件路径
    fileDialog->setDirectory(".");

    //设置文件过滤器,只显示.ui .cpp 文件,多个过滤文件使用空格隔开
    fileDialog->setNameFilter(tr("File(*.txt)"));

    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);

    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);

    //获取选择的文件的路径
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
        for (auto i : fileNames) {
            ui->lineEdit->setText(i);
            char path[130];
            strcpy(path, i.toStdString().c_str());
            wordLen = readWords(path, words);
        }
    }
}

inline bool getAllowLoop(QString string) {
    if (string.toStdString() == "是") {
        return true;
    } else {
        return false;
    }
}

void MainWindow::on_toolButton_3_clicked()
{
    try {
        int chainLen = countChains(words, wordLen, result);
        bool allowLoop = getAllowLoop(ui->comboBox_5->currentText());
        if (allowLoop) {
            throw MyException(N_WORK_WITH_OTHER_PARAMETER);
        }
        QString qResult;
        for (int var = 0; var < chainLen; ++var) {
            qResult += result[var];
            qResult += "\n";
        }
        ui->textEdit->setText(qResult);
    } catch(MyException &e) {
        exceptionHandler(e.what());
    }

}

inline int getFlag(QString qstring) {
    if (qstring.size() > 1) {
        return 0;
    } else {
        return qstring.toStdString()[0];
    }
}


void MainWindow::on_toolButton_clicked()
{
    try {
        int head = getFlag(ui->comboBox_2->currentText());
        int tail = getFlag(ui->comboBox->currentText());
        int ban = getFlag(ui->comboBox_3->currentText());
        int allowLoop = getAllowLoop(ui->comboBox_5->currentText());
        if (ban == head) {
            throw MyException(WORD_NOT_AVAILABLE);
        }
        int  resultLen = getLongestWordChain(words, wordLen, result, head, tail, ban, allowLoop);
        QString qResult;
        for (int var = 0; var < resultLen; ++var) {

            qResult += result[var];
            qResult += "\n";
        }
        ui->textEdit->setText(qResult);
    } catch(MyException &e) {
        exceptionHandler(e.what());
    }



}


void MainWindow::on_toolButton_2_clicked()
{

    try {
        int head = getFlag(ui->comboBox_2->currentText());
        int tail = getFlag(ui->comboBox->currentText());
        int ban = getFlag(ui->comboBox_3->currentText());
        int allowLoop = getAllowLoop(ui->comboBox_5->currentText());
        if (ban == head) {
            throw MyException(WORD_NOT_AVAILABLE);
        }
        int  resultLen = getLongestCharChain(words, wordLen, result, head, tail, ban, allowLoop);
        QString qResult;
        for (int var = 0; var < resultLen; ++var) {

            qResult += result[var];
            qResult += "\n";
        }
        ui->textEdit->setText(qResult);
    } catch(MyException &e) {
        exceptionHandler(e.what());
    }
}

