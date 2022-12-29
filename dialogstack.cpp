#include "dialogstack.h"
#include "ui_dialogstack.h"

DialogStack::DialogStack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStack)
{
    ui->setupUi(this);
    /******************** herramienta base para generar dots *******************/
    tad= new Stack<int>;
                ui->textEditHistory->insertPlainText("History");

                fileDot = "temp.dot";
                fileSvg = "temp.svg";
                command = "dot temp.dot -Tsvg -Gsize=9,6 -o temp.svg";
                render();
                /******************** herramienta base para generar dots *******************/

}

DialogStack::~DialogStack()
{
    delete ui;
}


void DialogStack::render()
{
    tad->toDot(fileDot);
    system(command.c_str());
    QImage image;
    image.load(fileSvg);

    ui->labelDisplay->setAlignment(Qt::AlignCenter);
    ui->labelDisplay->setPixmap(QPixmap::fromImage(image));
    ui->labelDisplay->setScaledContents(false);
}


void DialogStack::on_pushButton_clicked()
{

}

//adaptar de cola a pila
void DialogStack::on_pushButtonApilar_clicked()
{
    QString strValue = ui->lineEditData->text();
        if (strValue != "") {
            int value = strValue.toInt();
            tad->push(value);
            ui->lineEditData->clear(); //limpiar el edit

            render(); //renderizar grafico

            QString str = "\nPushing: " + QString::number(value); //actualizando historial de objetos en cola
            ui->textEditHistory->insertPlainText(str);
        }
}


void DialogStack::on_pushButtonDesapilar_clicked()
{
    if (tad ->getSize()){
        int value=tad->pop();
         render(); //renderizar grafico

         QString str = "\npop! :o : " + QString::number(value); //actualizando historial de objetos en cola
         ui->textEditHistory->insertPlainText(str);
    }
    else {
         ui->textEditHistory->insertPlainText("\npop! :'O : Empty!");
    };

}

