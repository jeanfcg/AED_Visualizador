#ifndef DIALOGQUEUE_H
#define DIALOGQUEUE_H

#include <QDialog>
#include "Queue.h"
namespace Ui {
class DialogQueue;
}

class DialogQueue : public QDialog
{
    Q_OBJECT

public:
    explicit DialogQueue(QWidget *parent = nullptr);
    ~DialogQueue();


private slots:
    void on_pushButtonEnqueue_clicked();

    void on_pushButtonDequeue_clicked();

private:
    Ui::DialogQueue *ui;
    Queue<int> *tad;

    std::string fileDot;
    std::string command;
    QString fileSvg;

    void render();

};

#endif // DIALOGQUEUE_H
