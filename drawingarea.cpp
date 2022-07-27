#include "drawingarea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

DrawingArea::DrawingArea(QWidget *parent):QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
}

bool DrawingArea::openImage(QString &fileName)
{
    QImage loadedImage;
    if(!loadedImage.load(fileName))
        return false;
    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

QImage DrawingArea::getImage()
{
    return image;
}

void DrawingArea::setColor(QColor &newColor)
{
    prevColor = currColor;
    currColor = newColor;
    pen.setColor(newColor);
    brush.setColor(newColor);
}

void DrawingArea::setPenWidth(int w)
{
    pen.setWidth(w);
    PenWidth = w;
}

void DrawingArea::setTextWidth(int w)
{
    TextWidth = w;
}

void DrawingArea::setTool(Tools newTool)
{
    tool = newTool;
    if (tool == Brush)
    {
        pen.setStyle(Qt::SolidLine);
    }
}

void DrawingArea::setBrushStyle(Qt::BrushStyle newBrushStyle)
{
   brush.setStyle(newBrushStyle);
}

void DrawingArea::setFigure(Figures newFigure)
{
    figure = newFigure;
}

void DrawingArea::setText(QString newText)
{
    text = newText;
}

void DrawingArea::setPenStyle(Qt::PenStyle newStyle)
{
    pen.setStyle(newStyle);
    if (tool == Brush)
    {
        pen.setStyle(Qt::SolidLine);
    }
}

void DrawingArea::clearImage()
{
    image.fill(qRgb(255,255,255));
    modified = true;
    mousePressed = false;
    update();
}

void DrawingArea::print()
{
    QPrinter printer (QPrinter::HighResolution);
    QPrintPreviewDialog prev(&printer, this);
    prev.setWindowState(Qt::WindowMaximized);
    connect(&prev, &QPrintPreviewDialog::paintRequested,
             this, &DrawingArea::showPreview);
    prev.exec();
}

void DrawingArea::showPreview(QPrinter * printer)
{
    update();
    QPainter painter(printer);
    QRect rect = painter.viewport();
    QSize size = image.size();
    size.scale(rect.size(), Qt::KeepAspectRatio);
    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(image.rect());
    painter.drawImage(0, 0, image);
    this->render(&painter);
}

void DrawingArea::drawFill()
{
    k = 0;
    QPainter painter(&image);
    painter.setPen(currColor);
    prevColor = image.pixelColor(lastPoint);
    fill(&painter, lastPoint.x(), lastPoint.y());
    modified = true;
}

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        lastPoint = event->pos();
        startPoint = event->pos();
        isDrawing = true;
        mousePressed = true;
        if (tool == TextTool)
        {
            drawText();
        }
        if (tool == Fill)
        {
            drawFill();
        }
    }
    update();
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton && isDrawing))
    {
        lastPoint = event->pos();
    }

    update();
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isDrawing)
    {
        isDrawing = false;
        mousePressed = false;
    }

    update();
}

void DrawingArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect,image,dirtyRect);
    static bool wasMousePresed = false;
    if  (mousePressed)
    {
        QPainter painter(this);
        wasMousePresed = true;
        if (tool == Pen)
        {
            drawLine();
        }
        else if (tool == Brush)
        {
            if (figure == Rectangle)
            {
                drawRect(&painter);
            }
            else if (figure == Ellipse)
            {
                drawEllipse(&painter);
            }
            else if (figure == Triangle)
            {
                drawTriangle(&painter);
            }
            else if (figure == Rhombus)
            {
                drawRhombus(&painter);
            }
            else if (figure == Hexagon)
            {
                drawHexagon(&painter);
            }
        }
    }
    else if (wasMousePresed)
    {
        QPainter painter(&image);

        if (tool == Brush)
        {
            if (figure == Rectangle)
            {
                drawRect(&painter);
            }
            else if (figure == Ellipse)
            {
                drawEllipse(&painter);
            }
            else if (figure == Triangle)
            {
                drawTriangle(&painter);
            }
            else if (figure == Rhombus)
            {
                drawRhombus(&painter);
            }
            else if (figure == Hexagon)
            {
                drawHexagon(&painter);
            }
        }
        wasMousePresed = false;
    }
    update();
}

void DrawingArea::resizeEvent(QResizeEvent *event)
{
    if (width()> image.width() || height() > image.height())
    {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());

        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void DrawingArea::drawLine()
{
    QPainter painter(&image);
    painter.setPen(pen);
    painter.drawLine(startPoint, lastPoint);
    modified = true;
    startPoint = lastPoint;
}

void DrawingArea::drawRect(QPainter *painter)
{
    QRect rect = QRect(startPoint, lastPoint);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawRect(rect);
    modified = true;
}

void DrawingArea::drawEllipse(QPainter *painter)
{
    QRect rect = QRect(startPoint, lastPoint);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawEllipse(rect);
    modified = true;
}


void DrawingArea::drawTriangle(QPainter *painter)
{
    painter->setPen(pen);

    QPoint a = QPoint(startPoint.x(), lastPoint.y());
    QPoint b  = QPoint(((startPoint.x() + lastPoint.x()) / 2), startPoint.y());
    QPoint c = QPoint(lastPoint);

    QPolygon polygon;
    polygon.append(a);
    polygon.append(b);
    polygon.append(c);

    painter->drawPolygon(polygon);
    QPainterPath path;
    path.addPolygon(polygon);
    painter->fillPath(path, brush);
    modified = true;
}

void DrawingArea::drawRhombus(QPainter *painter)
{
    painter->setPen(pen);

    QPoint a = QPoint(startPoint.x(), lastPoint.y());
    QPoint b  = QPoint(((startPoint.x() + lastPoint.x()) / 2), startPoint.y());
    QPoint c = QPoint(lastPoint);
    int tmp = lastPoint.y() - startPoint.y();
    QPoint d  = QPoint( b.x(),  lastPoint.y() + tmp);

    QPolygon polygon;
    polygon.append(a);
    polygon.append(b);
    polygon.append(c);
    polygon.append(d);

    QPainterPath path;
    path.addPolygon(polygon);
    painter->fillPath(path, brush);
    painter->drawPolygon(polygon);
    modified = true;
}


void DrawingArea::drawHexagon(QPainter *painter)
{
    int Ydist = lastPoint.y() - startPoint.y();
    painter->setPen(pen);

    QPoint a = QPoint(startPoint.x(), lastPoint.y());
    QPoint b  = QPoint(((startPoint.x() + lastPoint.x()) / 2), startPoint.y());
    QPoint c = QPoint(lastPoint);

    QPoint d  = QPoint( lastPoint.x(),  lastPoint.y() + Ydist);
    QPoint e  = QPoint( a.x(),  lastPoint.y() + Ydist);
    QPoint f  = QPoint( b.x(),  e.y() + Ydist);

    QPolygon polygon;
    polygon.append(a);
    polygon.append(b);
    polygon.append(c);
    polygon.append(d);
    polygon.append(f);
    polygon.append(e);

    QPainterPath path;
    path.addPolygon(polygon);
    painter->fillPath(path, brush);
    painter->drawPolygon(polygon);
    modified = true;
}

void DrawingArea::drawText()
{
    QPainter painter(&image);
    painter.setPen(currColor);
    painter.setFont(QFont("Arial", TextWidth, QFont::PreferDefaultHinting));
    painter.drawText(startPoint, text);
    startPoint = lastPoint;
    modified = true;
}

void DrawingArea::fill(QPainter *painter, int x, int y)
{
    if (x < 0 || x > this->width() || y < 0 || y > height() || k > 20000)
    {
        return;
    }
    if (image.pixelColor(x, y) != prevColor)
    {
        return;
    }
    if (image.pixelColor(x, y) == currColor)
    {
        return;
    }

    painter->drawPoint(x,y);
    k++;
    fill(painter, x + 1, y);
    fill(painter, x - 1, y);
    fill(painter, x, y - 1);
    fill(painter, x, y + 1);
}


void DrawingArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0,0), *image);
    *image = newImage;
}


