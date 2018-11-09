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
    send_updated_parameters();
}

void MainWindow::on_layerHeight_returnPressed()
{
    double layerHeight = mMainWindowUI->layerHeight->text().toDouble();
    MainWindow::mPrintShape->set_layer_height(layerHeight);
    MainWindow::mOSGWidget->update();
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
    double shapeWidth  = mMainWindowUI->shapeWidth->text().toDouble();
    double shapeLength = mMainWindowUI->shapeLength->text().toDouble();
    double shapeHeight = mMainWindowUI->shapeHeight->text().toDouble();
    MainWindow::mOSGWidget->set_shape_size(shapeWidth,shapeLength,shapeHeight);
}

void MainWindow::on_clearButton_clicked()
{
    mOSGWidget->clear_window();
}

void MainWindow::on_redrawButton_clicked()
{
    mOSGWidget->redraw();
}

void MainWindow::on_autoUpdateButton_clicked(bool checked)
{
    MainWindow::mOSGWidget->toggle_start(checked);
}

void MainWindow::send_updated_parameters()
{
    mOSGWidget->set_print_parameters(mDiameterOfPrint,mNeedleDiameter,mExtrusionMultiplier,mInfillPercentage,mExtrusionWidth,mLayerHeight);

}
