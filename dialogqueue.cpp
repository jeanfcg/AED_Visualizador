#include "dialogqueue.h"
#include "ui_dialogqueue.h"

DialogQueue::DialogQueue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogQueue)
{
    ui->setupUi(this);

    /******************** herramienta base para generar dots *******************/
    tad= new Queue<int>;
                ui->textEditHistory->insertPlainText("History");

                fileDot = "temp.dot";
                fileSvg = "temp.svg";
                command = "dot temp.dot -Tsvg -Gsize=9,6 -o temp.svg";
                render();
                /******************** herramienta base para generar dots *******************/

}

DialogQueue::~DialogQueue()
{
    delete ui;
    delete tad;
}


void DialogQueue::render()
{
    tad->toDot(fileDot);
    system(command.c_str());
    QImage image;
    image.load(fileSvg);

    ui->labelDisplay->setAlignment(Qt::AlignCenter);
    ui->labelDisplay->setPixmap(QPixmap::fromImage(image));
    ui->labelDisplay->setScaledContents(false);
}


void DialogQueue::on_pushButtonEnqueue_clicked()
{
    QString strValue = ui->lineEditData->text();
        if (strValue != "") {
            int value = strValue.toInt();
            tad->enqueue(value);
            ui->lineEditData->clear(); //limpiar el edit

            render(); //renderizar grafico

            QString str = "\nenqueue: " + QString::number(value); //actualizando historial de objetos en cola
            ui->textEditHistory->insertPlainText(str);
        }
}

void DialogQueue::on_pushButtonDequeue_clicked()
{
    if (tad ->getSize()){
        int value=tad->dequeue();
         render(); //renderizar grafico

         QString str = "\ndequeue: " + QString::number(value); //actualizando historial de objetos en cola
         ui->textEditHistory->insertPlainText(str);
    }
    else {
         ui->textEditHistory->insertPlainText("\ndequeue: Empty!");
    };
    }


