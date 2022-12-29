#include "dialoggraph.h"
#include "ui_dialoggraph.h"

Dialoggraph::Dialoggraph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialoggraph)
{
    ui->setupUi(this);

    /******************** herramienta base para generar dots *******************/
    tad = new Graph<long>;
                //ui->textEditHistory->insertPlainText("History");

                fileDot = "temp.dot";
                fileSvg = "temp.svg";
                command = "dot temp.dot -Tsvg -Gsize=9,6 -o temp.svg";
                render();
    /******************** herramienta base para generar dots *******************/
}

Dialoggraph::~Dialoggraph()
{
    delete ui;

    delete tad;
}

void Dialoggraph::render(Graph<long> tree)
{
    tad->toDot(fileDot, tree);
    system(command.c_str());
    QImage image;
    image.load(fileSvg);

    ui->labelDisplay->setAlignment(Qt::AlignCenter);
    ui->labelDisplay->setPixmap(QPixmap::fromImage(image));
    ui->labelDisplay->setScaledContents(false);
}

void Dialoggraph::on_pushButtonVertices_clicked()
{
    QString strValue = ui->lineEditData1->text();

    if (strValue != "") {
        long value = strValue.toLong();
        Vertex<long> vertex(value);
        if (tad->index(vertex) == NONE) {

            tad->insert(vertex); ///
            ui->lineEditData1->clear(); //limpiar el edit

            render(); //renderizar grafico
        }

        //QString str = "\Insertar: " + QString::number(value); //actualizando historial de objetos en cola
        //ui->textEditHistory->insertPlainText(str);
    }
}


void Dialoggraph::on_pushButtonAristas_clicked()
{
    QString strValue1 = ui->lineEditData1->text();
    QString strValue2 = ui->lineEditData2->text();
    QString strValue3 = ui->lineEditData3->text();

    if (strValue1 != "" && strValue2 != "" && strValue3 != "") {
        long value1 = strValue1.toLong();
        long value2 = strValue2.toLong();
        Vertex<long> start(value1);
        Vertex<long> end(value2);
        if (tad->index(start) != NONE && tad->index(end) != NONE) {

            int value = strValue3.toInt();

            tad->insert(Edge<long>(start, end, value)); ///


            ui->lineEditData1->clear(); //limpiar el edit
            ui->lineEditData2->clear(); //limpiar el edit
            ui->lineEditData3->clear(); //limpiar el edit

            render(); //renderizar grafico

        //QString str = "\Insertar: " + QString::number(value)
        //                            + QString::number(value2); //actualizando historial de objetos en cola
        //ui->textEditHistory->insertPlainText(str);

        }
    }
}

void Dialoggraph::on_pushButtonAtras_clicked()
{
    render();
}


void Dialoggraph::on_pushButtonBusquedaProfunda_clicked()
{
    QString strValue = ui->lineEditData1->text();
    long value = strValue.toLong();
    Vertex<long> vertex(value);
    int initial = tad->index(vertex);
    if (initial != NONE) {
        Graph<long> tree = tad->alg_DFS(initial);
        render(tree);
        ui->lineEditData1->clear(); //limpiar el edit
    }
}



void Dialoggraph::on_pushButtonBusquedaAncha_clicked()
{
    QString strValue = ui->lineEditData1->text();
    long value = strValue.toLong();
    Vertex<long> vertex(value);
    int initial = tad->index(vertex);
    if (initial != NONE) {
        Graph<long> tree = tad->alg_BFS(initial);
        render(tree);
        ui->lineEditData1->clear(); //limpiar el edit
    }
}


void Dialoggraph::on_pushButtonPrim_clicked()
{
    QString strValue = ui->lineEditData1->text();
    long value = strValue.toLong();
    Vertex<long> vertex(value);
    int initial = tad->index(vertex);
    if (initial != NONE) {
        Graph<long> tree = tad->alg_Prim(initial);
        render(tree);
        ui->lineEditData1->clear(); //limpiar el edit
    }
}


void Dialoggraph::on_pushButtonWarshall_clicked()
{
    QString strValue = ui->lineEditData1->text();
    long value = strValue.toLong();
    Vertex<long> vertex(value);
    int initial = tad->index(vertex);
    if (initial != NONE) {
        std::vector<std::vector<int>> matrix= tad->alg_Floyd_Marshall();
        tad->toDot(fileDot, matrix);
        render();
        ui->lineEditData1->clear(); //limpiar el edit
    }
}


void Dialoggraph::on_pushButtonFloyd_clicked()
{
    QString strValue = ui->lineEditData1->text();
    long value = strValue.toLong();
    Vertex<long> vertex(value);
    int initial = tad->index(vertex);
    if (initial != NONE) {
        std::vector<std::vector<int>> matrix= tad->alg_Floyd_Marshall();
        tad->toDot(fileDot, matrix);
        render();
        ui->lineEditData1->clear(); //limpiar el edit
    }
}


void Dialoggraph::on_pushButtonDijkstra_clicked()
{
    QString strValue = ui->lineEditData1->text();
    long value = strValue.toLong();
    Vertex<long> vertex(value);
    int initial = tad->index(vertex);
    if (initial != NONE) {
        Graph<long> tree = tad->alg_Prim(initial);
        render(tree);
        ui->lineEditData1->clear(); //limpiar el edit
    }
}

