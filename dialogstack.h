#ifndef DIALOGSTACK_H
#define DIALOGSTACK_H

#include <QDialog>
#include "Stack.h"

namespace Ui {
class DialogStack;
}

class DialogStack : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStack(QWidget *parent = nullptr);
    ~DialogStack();

private slots:
    void on_pushButton_clicked();

    void on_pushButtonApilar_clicked();

    void on_pushButtonDesapilar_clicked();

private:
    Ui::DialogStack *ui;
    Stack<int> *tad;

    std::string fileDot;
    std::string command;
    QString fileSvg;

    void render();
};

#endif // DIALOGSTACK_H
