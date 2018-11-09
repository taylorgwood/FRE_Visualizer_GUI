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
    //    bool simulationOn{false};
    //    mPrintShape = new PrintShape();
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
    double extrusionMultiplier = mMainWindowUI->extrusionMultiplierSlider->value();
    mExtrusionMultiplier = extrusionMultiplier;
    send_print_parameters();
}



void MainWindow::on_needleGauge_valueChanged(int arg1)
{
    double needleDiameter{0.26};
    int needleGauge{arg1};
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
    MainWindow::mPrintShape->set_needle_diameter(needleDiameter);
    MainWindow::mOSGWidget->update();
}

void MainWindow::on_objectSizeButton_clicked()
{
    send_shape_parameters();
}

void MainWindow::on_clearButton_clicked()
{
    mOSGWidget->clear_window();
}

void MainWindow::on_redrawButton_clicked()
{
    send_print_parameters();
    on_objectSizeButton_clicked();
    mOSGWidget->redraw();
}

void MainWindow::on_autoUpdateButton_clicked(bool checked)
{
    MainWindow::mOSGWidget->toggle_start(checked);
}

void MainWindow::on_layerHeight_returnPressed()
{
    double layerHeight = mMainWindowUI->layerHeight->text().toDouble();
    mLayerHeight = layerHeight;
    send_print_parameters();
}

void MainWindow::on_extrusionMultiplier_returnPressed()
{
    double extrusionMultiplier = mMainWindowUI->extrusionMultiplier->text().toDouble();
    mExtrusionMultiplier = extrusionMultiplier;
    send_print_parameters();
}

void MainWindow::on_infillPercentage_returnPressed()
{
    double infillPercentage = mMainWindowUI->infillPercentage->text().toDouble();
    mInfillPercentage = infillPercentage;
    send_print_parameters();
}

void MainWindow::on_extrusionWidth_returnPressed()
{
    double extrusionWidth = mMainWindowUI->extrusionWidth->text().toDouble();
    mExtrusionWidth = extrusionWidth;
    send_print_parameters();
}

void MainWindow::send_print_parameters()
{
    mOSGWidget->set_print_parameters(mNeedleDiameter,mExtrusionMultiplier,mInfillPercentage,mExtrusionWidth,mLayerHeight);

}

void MainWindow::on_shapeWidth_returnPressed()
{
    mShapeWidth  = mMainWindowUI->shapeWidth->text().toDouble();
    send_shape_parameters();
}

void MainWindow::on_shapeLength_returnPressed()
{
    mShapeLength = mMainWindowUI->shapeLength->text().toDouble();
    send_shape_parameters();
}

void MainWindow::on_shapeHeight_returnPressed()
{
    mShapeHeight = mMainWindowUI->shapeHeight->text().toDouble();
    send_shape_parameters();
}

void MainWindow::send_shape_parameters()
{
    MainWindow::mOSGWidget->set_shape_size(mShapeWidth,mShapeLength,mShapeHeight);
}



