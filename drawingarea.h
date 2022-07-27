#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QList>
#include <QVector>
#include <QPen>
#include <QPrinter>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>


enum Tools { TextTool, Pen, Brush, Fill };
enum Figures { Rectangle, Ellipse, Triangle, Rhombus, Hexagon  };

class DrawingArea: public QWidget
{
    Q_OBJECT
public:
    DrawingArea(QWidget *parent = nullptr);

    bool openImage(QString &fileName);
    QImage getImage();
    void setColor(QColor &newColor);
    void setPenWidth(int w);
    void setTextWidth(int w);
    void setTool(Tools newTool);
    void setBrushStyle (Qt::BrushStyle newBrushStyle);
    void setFigure (Figures newFigure);
    void setText (QString newText);
    void setIsGradient(bool isGrad);
    void setPenStyle(Qt::PenStyle newStyle);

    bool isModified() {return modified;}
    QColor getColor() {return currColor;}
    int getPenWidth() {return PenWidth;}
    Tools getTool()   {return tool;}
    int getTextWidth(){return TextWidth;}
public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void showPreview(QPrinter * printer);
private:

    void drawFill();

    void drawLine();

    void drawRect(QPainter *painter);

    void drawEllipse(QPainter *painter);

    void drawTriangle(QPainter *painter);

    void drawRhombus(QPainter *painter);

    void drawHexagon(QPainter *painter);

    void drawText();

    void fill(QPainter *painter, int x, int y);

    void resizeImage(QImage *image, const QSize &newSize);

    void setBrush();

    Tools tool = Pen;
    Figures figure = Rectangle;

    bool modified = false;
    bool isDrawing = false;
    int PenWidth = 5;

    QColor currColor = Qt::blue;
    QColor prevColor = Qt::blue;
    QImage image;
    QPoint startPoint;
    QPoint lastPoint;

    bool mousePressed = false;
    QString text = "";
    int TextWidth = 12;
    QBrush brush = QBrush(currColor, Qt::SolidPattern);
    QPen pen = QPen(currColor, PenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    int k = 0;

};

#endif // DRAWINGAREA_H
