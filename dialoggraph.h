#ifndef DIALOGGRAPH_H
#define DIALOGGRAPH_H

#include <QDialog>
#include "Graph.h"

namespace Ui {
class Dialoggraph;
}

class Dialoggraph : public QDialog
{
    Q_OBJECT

public:
    explicit Dialoggraph(QWidget *parent = nullptr);
    ~Dialoggraph();

private slots:
    void on_pushButtonVertices_clicked();

    void on_pushButtonAristas_clicked();

    void on_pushButtonBusquedaProfunda_clicked();

    void on_pushButtonAtras_clicked();

    void on_pushButtonBusquedaAncha_clicked();

    void on_pushButtonPrim_clicked();

    void on_pushButtonWarshall_clicked();

    void on_pushButtonFloyd_clicked();

    void on_pushButtonDijkstra_clicked();

private:
    Ui::Dialoggraph *ui;
    Graph<long> *tad;

    std::string fileDot;
    std::string command;
    QString fileSvg;

    void render(Graph<long> tree = Graph<long>());

};

#endif // DIALOGGRAPH_H
