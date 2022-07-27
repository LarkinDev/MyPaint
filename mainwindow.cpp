#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "drawingarea.h"

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QImageWriter>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), area(new DrawingArea(this))
{
    ui->setupUi(this);
    setCentralWidget(area);
    setWindowTitle("My PAINT");
    ui->actionPen->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::open()
{
    if(maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"),
                                                        "",
                                                        tr("PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)" ));
        if (!fileName.isEmpty())
        {
            area->openImage(fileName);
        }
    }
}

bool MainWindow::save()
{
    QImage saveDrawing = area->getImage();
    QString filePath = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)");
    if (filePath.isEmpty())
    {
        return false;
    }
    saveDrawing.save(filePath);
    return true;
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(area->getColor());
    if (newColor.isValid())
    {
        area->setColor(newColor);
    }
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, "",
                                          tr("Select pen width"),
                                          area->getPenWidth(),1,25, 1, &ok);
    if (ok)
    {
        area->setPenWidth(newWidth);
    }
}


bool MainWindow::maybeSave()
{
    if(area->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::information(this, "",
                           tr("Do you want to save your changes?"),
                           QMessageBox::Save | QMessageBox::Discard
                           | QMessageBox::Cancel);
         if (ret == QMessageBox::Save)
         {
             return save();
         }
         else if (ret == QMessageBox::Cancel)
             return false;
     }
     return true;
}



void MainWindow::on_action_Open_triggered()
{
    open();
}


void MainWindow::on_action_Save_As_triggered()
{
    save();
}


void MainWindow::on_action_Print_triggered()
{
    area->print();
    switch (area->getTool()) {
        case Pen : ui->actionPen->setChecked(true); break;
        case Brush : ui->actionBrush->setChecked(true); break;
        case TextTool : ui->actionText->setChecked(true); break;
        case Fill : ui->actionFill->setChecked(true); break;
    }
}


void MainWindow::on_action_Color_triggered()
{
    penColor();
    switch (area->getTool()) {
        case Pen : ui->actionPen->setChecked(true); break;
        case Brush : ui->actionBrush->setChecked(true); break;
        case TextTool : ui->actionText->setChecked(true); break;
        case Fill : ui->actionFill->setChecked(true); break;
    }

}


void MainWindow::on_action_Clear_triggered()
{
    area->clearImage();
}

void MainWindow::on_actionPen_triggered()
{
    area->setTool(Pen);
    ui->actionText->setChecked(false);
    ui->actionBrush->setChecked(false);
    ui->actionFill->setChecked(false);
    ui->action_Color->setChecked(false);
    ui->action_Print->setChecked(false);
}

void MainWindow::on_actionHor_triggered()
{
     area->setBrushStyle(Qt::HorPattern);
}

void MainWindow::on_actionDence_1_triggered()
{
      area->setBrushStyle(Qt::Dense1Pattern);
}


void MainWindow::on_actionDence_2_triggered()
{
    area->setBrushStyle(Qt::Dense2Pattern);
}



void MainWindow::on_actionVertical_triggered()
{
     area->setBrushStyle(Qt::VerPattern);
}


void MainWindow::on_actionRectangle_triggered()
{
    area->setFigure(Rectangle);
}


void MainWindow::on_actionEllipse_triggered()
{
    area->setFigure(Ellipse);
}


void MainWindow::on_actionTriangle_triggered()
{
    area->setFigure(Triangle);
}


void MainWindow::on_actionRhombus_triggered()
{
    area->setFigure(Rhombus);
}


void MainWindow::on_actionHexagon_triggered()
{
    area->setFigure(Hexagon);
}


void MainWindow::on_actionText_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, "",
                                          tr("Text:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty())
    {
        area->setTool(TextTool);
        area->setText(text);
        ui->action_Color->setChecked(false);
        ui->actionBrush->setChecked(false);
        ui->actionFill->setChecked(false);
        ui->actionPen->setChecked(false);
        ui->action_Print->setChecked(false);
    }
    else
    {
        ui->actionText->setChecked(false);
        switch (area->getTool()) {
            case Pen : ui->actionPen->setChecked(true); break;
            case Brush : ui->actionBrush->setChecked(true); break;
            case TextTool : ui->actionText->setChecked(true); break;
            case Fill : ui->actionFill->setChecked(true); break;
        }
    }
}


void MainWindow::on_actionFill_triggered()
{
    area->setTool(Fill);
    ui->actionText->setChecked(false);
    ui->actionBrush->setChecked(false);
    ui->action_Color->setChecked(false);
    ui->actionPen->setChecked(false);
    ui->action_Print->setChecked(false);
}


void MainWindow::on_actionSolid_triggered()
{
    area->setPenStyle(Qt::SolidLine);
}


void MainWindow::on_actionDash_triggered()
{
    area->setPenStyle(Qt::DashLine);
}


void MainWindow::on_actionDot_triggered()
{
    area->setPenStyle(Qt::DotLine);
}


void MainWindow::on_actionDash_Dot_triggered()
{
    area->setPenStyle(Qt::DashDotDotLine);
}


void MainWindow::on_actionDash_Dot_Dot_triggered()
{
    area->setPenStyle(Qt::DashDotDotLine);
}


void MainWindow::on_actionWidth_triggered()
{
    penWidth();
}


void MainWindow::on_actionBrush_triggered()
{
    area->setTool(Brush);
    ui->actionText->setChecked(false);
    ui->action_Color->setChecked(false);
    ui->actionFill->setChecked(false);
    ui->actionPen->setChecked(false);
    ui->action_Print->setChecked(false);
}

void MainWindow::on_action_Text_Size_triggered()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, "",
                                          tr("Select pen width"),
                                          area->getTextWidth(),1,25, 1, &ok);
    if (ok)
    {
        area->setTextWidth(newWidth);
    }
}


void MainWindow::on_actionNone_Brush_triggered()
{
    area->setBrushStyle(Qt::NoBrush);
}


void MainWindow::on_actionSolid_2_triggered()
{
    area->setBrushStyle(Qt::SolidPattern);
}

