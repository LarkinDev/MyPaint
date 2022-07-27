#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "drawingarea.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void open();
    bool save();
    void penColor();
    void penWidth ();

    void on_action_Open_triggered();

    void on_action_Save_As_triggered();

    void on_action_Print_triggered();

    void on_action_Color_triggered();

    void on_action_Clear_triggered();

    void on_actionPen_triggered();

    void on_actionDence_1_triggered();

    void on_actionDence_2_triggered();

    void on_actionHor_triggered();

    void on_actionVertical_triggered();

    void on_actionRectangle_triggered();

    void on_actionEllipse_triggered();

    void on_actionTriangle_triggered();

    void on_actionRhombus_triggered();

    void on_actionHexagon_triggered();

    void on_actionText_triggered();

    void on_actionFill_triggered();

    void on_actionSolid_triggered();

    void on_actionDash_triggered();

    void on_actionDot_triggered();

    void on_actionDash_Dot_triggered();

    void on_actionDash_Dot_Dot_triggered();

    void on_actionWidth_triggered();

    void on_actionBrush_triggered();

    void on_action_Text_Size_triggered();

    void on_actionNone_Brush_triggered();

    void on_actionSolid_2_triggered();

private:
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();
    bool maybeSave();

    DrawingArea *area;
};
#endif // MAINWINDOW_H
