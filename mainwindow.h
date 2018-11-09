#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>

class OutputWindow;
class OSGWidget;
class PrintShape;

namespace Ui {
class MainWindowForm;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_actionExit_triggered();
    void on_actionStart_triggered();
    void on_actionStop_triggered();
private slots:
    void on_extrusionMultiplierSlider_sliderMoved(int position);
    void on_needleGauge_valueChanged(int arg1);
    void set_needle_diameter(int needleGauge);
    void on_objectSizeButton_clicked();
    void on_clearButton_clicked();
    void on_redrawButton_clicked();
    void on_autoUpdateButton_clicked(bool checked);
    void on_layerHeight_returnPressed();
    void on_extrusionMultiplier_returnPressed();
    void on_infillPercentage_returnPressed();
    void on_extrusionWidth_returnPressed();
    void send_print_parameters();
    void on_shapeWidth_returnPressed();
    void on_shapeLength_returnPressed();
    void on_shapeHeight_returnPressed();
    void send_shape_parameters();



private:
    Ui::MainWindowForm *mMainWindowUI;
    OSGWidget *mOSGWidget{nullptr};
    PrintShape *mPrintShape{nullptr};
    std::vector<PrintShape*> *mShapeList{nullptr};
    double mNeedleDiameter{0.26};
    double mExtrusionMultiplier{0.785};
    double mInfillPercentage{100};
    double mExtrusionWidth{0.26};
    double mExtrusionWidthCalculated{mExtrusionWidth};
    float  mLayerHeight{0.26f};
    float  mShapeWidth{10};
    float  mShapeHeight{10};
    float  mShapeLength{10};

};

#endif // MAINWINDOW_H
