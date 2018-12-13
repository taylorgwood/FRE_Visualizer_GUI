#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include "shape.h"
#include "gcode.h"

class OutputWindow;
class OSGWidget;

namespace Ui
{
class MainWindowForm;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

private slots:
    void on_actionExit_triggered();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionView_Cylinders_triggered();
    void on_actionView_Print_Path_triggered();
    void on_actionView_Wireframe_triggered();
    void on_actionView_Axes_triggered();
    void view_axes();
    void view_wireframe();
    void view_cylinders();
    void view_print_path();
    void on_needleGauge_valueChanged(int arg1);
    void set_needle_diameter(int needleGauge);
    void set_needle_diameter_label();
    void on_objectSizeButton_clicked();
    void on_applyParametersButton_clicked();
    void on_clearButton_clicked();
    void on_redrawButton_clicked();
    void on_layerHeight_returnPressed();
    void on_extrusionMultiplier_returnPressed();
    void on_infillPercentage_returnPressed();
    void on_extrusionWidth_returnPressed();
    void on_shapeWidth_returnPressed();
    void on_shapeLength_returnPressed();
    void on_shapeHeight_returnPressed();
    void redraw_and_refresh_information();
    void set_volume_label();
    void on_resetParametersButton_clicked();
    void on_resetObjectSizeButton_clicked();
    void set_default_print_parameters();
    void set_default_object_size();
    void reset_print_parameter_labels();
    void reset_object_size_labels();
    void on_autoAdjustLayersButton_clicked(bool checked);
    void on_autoAdjustWidthButton_clicked(bool checked);
    void on_actionExport_G_code_triggered();

signals:

private:
    Ui::MainWindowForm *mMainWindowUI;
    OSGWidget *mOSGWidget{nullptr};
    Shape  *mShape{nullptr};
    double mNeedleDiameter{0.26};
    double mExtrusionMultiplier{0.785};
    double mInfillPercentage{100};
    double mExtrusionWidth{0.26};
    float  mLayerHeight{0.26f};
    float  mShapeWidth{10};
    float  mShapeHeight{10};
    float  mShapeLength{10};

};

#endif // MAINWINDOW_H
