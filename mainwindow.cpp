#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include "osgwidget.h"

#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    mMainWindowUI{new Ui::MainWindowForm}
{
    mMainWindowUI->setupUi(this);
    mOSGWidget = new OSGWidget{this};
    this->setCentralWidget(mOSGWidget);
}

MainWindow::~MainWindow()
{
    delete mMainWindowUI;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionStart_triggered()
{
    bool On{1};
    MainWindow::mOSGWidget->toggle_start(On);
}

void MainWindow::on_actionStop_triggered()
{
    bool Off{0};
    MainWindow::mOSGWidget->toggle_start(Off);
}

void MainWindow::on_extrusionMultiplierSlider_sliderMoved(int position)
{
    double extrusionMultiplierPercentage = mMainWindowUI->extrusionMultiplierSlider->value();
    mExtrusionMultiplier = extrusionMultiplierPercentage/100;
    redraw_print_parameters();
}

void MainWindow::on_needleGauge_valueChanged(int arg1)
{    
    int needleGauge{arg1};
    set_needle_diameter(needleGauge);
    redraw_print_parameters();
}

void MainWindow::set_needle_diameter(int needleGauge)
{
    double needleDiameter{0.26};
    if (needleGauge == 20)
    {
        needleDiameter = 0.603;
    }
    else if (needleGauge == 21)
    {
        needleDiameter = 0.514;
    }
    else if (needleGauge == 22)
    {
        needleDiameter = 0.413;
    }
    else if (needleGauge == 23)
    {
        needleDiameter = 0.337;
    }
    else if (needleGauge == 24)
    {
        needleDiameter = 0.311;
    }
    else if (needleGauge == 25)
    {
        needleDiameter = 0.260;
    }
    else if (needleGauge == 26)
    {
        needleDiameter = 0.260;
    }
    else if (needleGauge == 27)
    {
        needleDiameter = 0.210;
    }
    else if (needleGauge == 28)
    {
        needleDiameter = 0.184;
    }
    else if (needleGauge == 29)
    {
        needleDiameter = 0.184;
    }
    else if (needleGauge == 30)
    {
        needleDiameter = 0.159;
    }
    mNeedleDiameter = needleDiameter;
}

void MainWindow::on_objectSizeButton_clicked()
{
    mShapeWidth  = mMainWindowUI->shapeWidth->text().toDouble();
    mShapeLength = mMainWindowUI->shapeLength->text().toDouble();
    mShapeHeight = mMainWindowUI->shapeHeight->text().toDouble();
    redraw_shape_parameters();
}

void MainWindow::on_resetParametersButton_clicked()
{

    mLayerHeight = mMainWindowUI->layerHeight->text().toDouble();
    mExtrusionMultiplier = mMainWindowUI->extrusionMultiplier->text().toDouble();
    mInfillPercentage = mMainWindowUI->infillPercentage->text().toDouble();
    mExtrusionWidth = mMainWindowUI->extrusionWidth->text().toDouble();
    redraw_print_parameters();
}

void MainWindow::on_clearButton_clicked()
{
    mOSGWidget->clear_window();
}

void MainWindow::on_redrawButton_clicked()
{
    redraw_print_parameters();
    redraw_shape_parameters();
    mOSGWidget->redraw();
}

void MainWindow::on_autoUpdateButton_clicked(bool checked)
{
    MainWindow::mOSGWidget->toggle_start(checked);
}

void MainWindow::on_layerHeight_returnPressed()
{
    mLayerHeight = mMainWindowUI->layerHeight->text().toDouble();
    redraw_print_parameters();
}

void MainWindow::on_extrusionMultiplier_returnPressed()
{
    mExtrusionMultiplier = mMainWindowUI->extrusionMultiplier->text().toDouble();
    redraw_print_parameters();
}

void MainWindow::on_infillPercentage_returnPressed()
{
    mInfillPercentage = mMainWindowUI->infillPercentage->text().toDouble();
    redraw_print_parameters();
}

void MainWindow::on_extrusionWidth_returnPressed()
{
    mExtrusionWidth = mMainWindowUI->extrusionWidth->text().toDouble();
    redraw_print_parameters();
}

void MainWindow::redraw_print_parameters()
{
    mOSGWidget->set_print_parameters(mNeedleDiameter,mExtrusionMultiplier,mInfillPercentage,mExtrusionWidth,mLayerHeight);
    mOSGWidget->redraw();
}

void MainWindow::on_shapeWidth_returnPressed()
{
    mShapeWidth  = mMainWindowUI->shapeWidth->text().toDouble();
    redraw_shape_parameters();
}

void MainWindow::on_shapeLength_returnPressed()
{
    mShapeLength = mMainWindowUI->shapeLength->text().toDouble();
    redraw_shape_parameters();
}

void MainWindow::on_shapeHeight_returnPressed()
{
    mShapeHeight = mMainWindowUI->shapeHeight->text().toDouble();
    redraw_shape_parameters();
}

void MainWindow::redraw_shape_parameters()
{
    MainWindow::mOSGWidget->set_shape_size(mShapeWidth,mShapeLength,mShapeHeight);
    mOSGWidget->redraw();
}





