#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    asignarFigura();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    // dibujarFigura();
    dibujarFiguraMapeada();
}

void Widget::asignarFigura()
{
    /*     // X              Y               H
    Fig[0][0] = 10; Fig[0][1] = 10; Fig[0][2] = 1;
    Fig[1][0] = 30; Fig[1][1] = 10; Fig[1][2] = 1;
    Fig[2][0] = 10; Fig[2][1] = 30; Fig[2][2] = 1;
*/

    Fig[0][0] = 10; Fig[0][1] = 10; Fig[0][2] = 1;
    Fig[1][0] = 30; Fig[1][1] = 10; Fig[1][2] = 1;
    Fig[2][0] = 20; Fig[2][1] = 30; Fig[2][2] = 1;

    Fig2[0][0] = 10; Fig2[0][1] = 10; Fig2[0][2] = 1;
    Fig2[1][0] = 30; Fig2[1][1] = 10; Fig2[1][2] = 1;
    Fig2[2][0] = 20; Fig2[2][1] = 30; Fig2[2][2] = 1;


    // PUNTO DE INICIO
    Fig[3][0] = 10; Fig[3][1] = 10; Fig[3][2] = 1;
    Fig2[3][0] = 10; Fig2[3][1] = 10; Fig2[3][2] = 1;

    // RESPALDAR LOS VALORES DEL POLIGONO
    for ( int r = 0; r < 4; r++) {
        for ( int c = 0; c < 3; c++ ) {
            FigAux[r][c] = Fig[r][c];
            FigAux2[r][c] = Fig2[r][c];
        }
    }

    //SE REQUIERE ESTABLECER EL ORIGEN EN EL CENTRO
    //DEL AREA DE DIBUJO
    //RESTARL EL WIDTH DE LA FORMA - EL WIDTH DEL WIDGET CONTENEDOR DE LOS BOTONES}

    ancho = this->size().width() - ui->widget->size().width();

    //CENTRO DEL AREA DE DIBUJO
    centroX = ancho/2;
    centroY = this->size().height()/2;


    tX = 0;
    tY = 0;
}

void Widget::dibujarFigura()
{
    /*
    QPainter *canvas = new QPainter(this);

    for(int i = 0; i < 3; i++)
        canvas->drawLine(centroX+Fig[i][0], centroY+Fig[i][1], centroX+Fig[i+1][0], centroY+Fig[i+1][1]);
                    // centroX y centroY desplazan o trasladan
                    // al centro de la pantalla

    canvas->drawPolyline(points, 4);
    canvas->end();
    */
}

void Widget::dibujarFiguraMapeada()
{
    QPainter *canvas = new QPainter(this);

    canvas->setRenderHint(QPainter::Antialiasing, true);
    // INVESTIGAR QUE ES ANTIALAS

    //DIBUJAR EJE COORDENADO
    //LINEA HORIZONTAL
    canvas->drawLine(0, this->size().height()/2, ancho, size().height()/2);
    // LINEA VERTICAL
    canvas->drawLine(ancho/2, 0, ancho/2, size().height());

    // DIBUJAR LINEA DIAGONAL

    Mapeo dibujo;

    //LA VENTANA ES EL AREA DEL MUNDO REAL
    dibujo.ventana(ancho/2, 0, ancho, size().height()/2);

    //EL PUERTO DE VISION SE COLOCARA LA DIVISIOM
    //DEL EJE CARTESIANO EN EL RECTANGULO DE LA DERECHA

    dibujo.puerto(ancho/2, 0, ancho, size().height()/2);

    int L = centroX;
    int M = centroY; // ES LA SUMA DE LOS VALORES PARA YPVMIN + YPVMAX

    //COLOCACION DE LA VENTANA EN EL PUERTO DE VISION

    int x1, y1, x2, y2;
    int x1_2, y1_2, x2_2, y2_2;


                    //pasamos la referencia de x1, y1
    dibujo.mapear(Fig[0][0], Fig[0][1], x1, y1, L, M);
    dibujo.mapear(Fig[2][0], Fig[2][1], x2, y2, L, M);
    // PRIMERO DIBUJAMOS LA LUNEA QUE CIERRA EL TRIANGULO

    canvas->setPen(QColor(0, 0, 255));

    dibujo.mapear(Fig2[0][0], Fig2[0][1], x1_2, y1_2, L, M);
    dibujo.mapear(Fig2[2][0], Fig2[2][1], x2_2, y2_2, L, M);

    canvas->setPen(QColor(255, 0, 0));
    canvas->drawLine(x1, y1, x2, y2);

    canvas->setPen(QColor(0, 0, 255));
    canvas->drawLine(x1_2, y1_2, x2_2, y2_2);

    for (int i = 0; i < 2; i++) {

        canvas->setPen(QColor(255, 0, 0));

        dibujo.mapear(Fig[i][0], Fig[i][1], x1, y1, L, M);
        dibujo.mapear(Fig[i+1][0], Fig[i+1][1], x2, y2, L, M);

        dibujo.mapear(Fig2[i][0], Fig2[i][1], x1_2, y1_2, L, M);
        dibujo.mapear(Fig2[i+1][0], Fig2[i+1][1], x2_2, y2_2, L, M);

        canvas->setPen(QColor(255, 0, 0));
        canvas->drawLine(x1, y1, x2, y2);

        canvas->setPen(QColor(0, 0, 255));
        canvas->drawLine(x1_2, y1_2, x2_2, y2_2);
    }

    canvas->end();

}

void Widget::trasladar(int tx, int ty)
{
    Matrix fig, id, fres; // OBJETOS ESTATICOS
    fig.Inicializa(4, 3);
    // figura resultante
    fres.Inicializa(4, 3);

    id.Inicializa(3, 3);
    //ASIGNAR PUNTOS
    for ( int i = 0; i < fig.numRen(); i++ )
        for ( int j = 0; j<fig.numCol(); j++ )
            fig.SET(i, j, Fig[i][j]);

    // APLICAR LA TRASLACION
    fres.traslacion(fig, id, tx, ty);   //tX, tY);

    //ASIGNA LOS NUEVOS VALORES
    for(int i = 0; i < fres.numRen(); i++)
        for( int j = 0; j < fres.numCol(); j++)
            Fig[i][j] = fres.GET(i, j);

    //repaint();//INVOCA A paintEvent
    update();

}

void Widget::escalar(int sx, int sy)
{
    Matrix fig, id, fres; // OBJETOS ESTATICOS
    fig.Inicializa(4, 3);
    // figura resultante
    fres.Inicializa(4, 3);

    id.Inicializa(3, 3);
    //ASIGNAR PUNTOS
    for ( int i = 0; i < fig.numRen(); i++ )
        for ( int j = 0; j<fig.numCol(); j++ )
            fig.SET(i, j, Fig[i][j]);

    // APLICAR LA TRASLACION
    fres.escalacion(fig, id, sx, sy);

    //ASIGNA LOS NUEVOS VALORES
    for(int i = 0; i < fres.numRen(); i++)
        for( int j = 0; j < fres.numCol(); j++)
            Fig[i][j] = fres.GET(i, j);

    //repaint();//INVOCA A paintEvent
    update();
}

void Widget::escalarFijo(int sx, int sy, int pfx, int pfy)
{
    Matrix fig, id, fres; // OBJETOS ESTATICOS
    fig.Inicializa(4, 3);
    // figura resultante
    fres.Inicializa(4, 3);

    id.Inicializa(3, 3);
    //ASIGNAR PUNTOS
    for ( int i = 0; i < fig.numRen(); i++ )
        for ( int j = 0; j<fig.numCol(); j++ )
            fig.SET(i, j, Fig[i][j]);

    // APLICAR LA TRASLACION
    fres.escalacionFijo(fig, id, sx, sy, pfx, pfy);

    //ASIGNA LOS NUEVOS VALORES
    for(int i = 0; i < fres.numRen(); i++)
        for( int j = 0; j < fres.numCol(); j++)
            Fig[i][j] = fres.GET(i, j);

    //repaint();//INVOCA A paintEvent
    update();
}

void Widget::rotar(double angulo_grados)
{
    Matrix fig, id, fres; // OBJETOS ESTATICOS
    fig.Inicializa(4, 3);
    // figura resultante
    fres.Inicializa(4, 3);

    id.Inicializa(3, 3);
    //ASIGNAR PUNTOS
    for ( int i = 0; i < fig.numRen(); i++ )
        for ( int j = 0; j<fig.numCol(); j++ )
            fig.SET(i, j, Fig[i][j]);

    // APLICAR LA ROTACION
    fres.rotacion(fig, id, angulo_grados);

    //ASIGNA LOS NUEVOS VALORES
    for(int i = 0; i < fres.numRen(); i++)
        for( int j = 0; j < fres.numCol(); j++)
            Fig[i][j] = fres.GET(i, j);

    //repaint();//INVOCA A paintEvent
    update();
}

void Widget::rotarPuntoPivote(double angulo_grados, int xr, int yr)
{
    Matrix fig, id, fres; // OBJETOS ESTATICOS
    fig.Inicializa(4, 3);
    // figura resultante
    fres.Inicializa(4, 3);

    id.Inicializa(3, 3);
    //ASIGNAR PUNTOS
    for ( int i = 0; i < fig.numRen(); i++ )
        for ( int j = 0; j<fig.numCol(); j++ )
            fig.SET(i, j, Fig[i][j]);

    // APLICAR LA ROTACION
    fres.rotacionPuntoPivote(fig, id, angulo_grados, xr, yr);

    //ASIGNA LOS NUEVOS VALORES
    for(int i = 0; i < fres.numRen(); i++)
        for( int j = 0; j < fres.numCol(); j++)
            Fig[i][j] = fres.GET(i, j);

    //repaint();//INVOCA A paintEvent
    update();
}

void Widget::keyPressEvent(QKeyEvent *evento)
{
    if(evento->key() == Qt::Key_W) //FLECHA ARRIBA en WINDOWS VK_UP
        on_pushButton_clicked();
    if(evento->key() == Qt::Key_S)
        on_pushButton_2_clicked();
    if(evento->key() == Qt::Key_A)
        on_pushButton_4_clicked();
    if(evento->key() == Qt::Key_D)
        on_pushButton_3_clicked();
    if(evento->key() == Qt::Key_Z)
        on_pushButton_5_clicked();
    if(evento->key() == Qt::Key_E)
        on_pushButton_6_clicked();
    if(evento->key() == Qt::Key_F)
        on_pushButton_7_clicked();
    if(evento->key() == Qt::Key_R)
        on_pushButton_15_clicked();
    if(evento->key() == Qt::Key_V)
        on_pushButton_8_clicked();

}


void Widget::on_pushButton_clicked()
{
    trasladar(0, tY + ui->spinBox_2->value());
}

void Widget::on_pushButton_2_clicked()
{
    trasladar(0, tY - ui->spinBox_2->value());
}

void Widget::on_pushButton_4_clicked()
{
    trasladar(tX - ui->spinBox->value(), 0);
}

void Widget::on_pushButton_3_clicked()
{
    trasladar(tX + ui->spinBox->value(), 0);
}

void Widget::on_pushButton_5_clicked()
{
    for ( int i = 0; i < 4; i++)
        for(int j = 0; j < 3; j++)
            Fig[i][j] = FigAux[i][j];

    tX = tY = 0;
    //repaint():;
    update();
}

void Widget::on_pushButton_6_clicked()
{
    // Para que no se repita
    on_pushButton_5_clicked();

    escalar(ui->spinBox_4->value(), ui->spinBox_3->value() );
}

void Widget::on_pushButton_7_clicked()
{
    // Para que no se repita
    on_pushButton_5_clicked();


    escalarFijo(ui->spinBox_4->value(), ui->spinBox_3->value(), ui->spinBox_5->value(), ui->spinBox_6->value());
}

void Widget::on_spinBox_5_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    on_pushButton_7_clicked();
}

void Widget::on_spinBox_6_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    on_pushButton_7_clicked();
}

void Widget::on_spinBox_4_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    on_pushButton_7_clicked();
}

void Widget::on_spinBox_3_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    on_pushButton_7_clicked();
}

void Widget::on_pushButton_15_clicked()
{
    rotar( ui->doubleSpinBox->value() );
}

void Widget::on_pushButton_8_clicked()
{
    rotarPuntoPivote( ui->doubleSpinBox->value(), ui->spinBox_7->value(), ui->spinBox_8->value());
}
