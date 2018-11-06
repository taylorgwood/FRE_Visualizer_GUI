#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include "osgwidget.h"
//#include "vector3d.h"

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


//void MainWindow::on_gravityButton_clicked()
//{
//    double x = mMainWindowUI->gravityX->text().toDouble();
//    double y = mMainWindowUI->gravityY->text().toDouble();
//    double z = mMainWindowUI->gravityZ->text().toDouble();
//    Vector3 gravity{x,y,z};
//    MainWindow::mOSGWidget->set_gravity(gravity);
//}


//void MainWindow::on_defaultButton_clicked()
//{
//    double radiusMin = mMainWindowUI->radiusMin->text().toFloat();
//    double radiusMax = mMainWindowUI->radiusMax->text().toFloat();
//    double massMin = mMainWindowUI->massMin->text().toDouble();
//    double massMax = mMainWindowUI->massMax->text().toDouble();
//    double cOfRMin = mMainWindowUI->cOfRMin->text().toDouble();
//    double cOfRMax = mMainWindowUI->cOfRMax->text().toDouble();
//    double velocityMin = mMainWindowUI->velocityMin->text().toDouble();
//    double velocityMax = mMainWindowUI->velocityMax->text().toDouble();
//    MainWindow::mOSGWidget->set_random_ranges(radiusMin,radiusMax,massMin,massMax,cOfRMin,cOfRMax,velocityMin,velocityMax);
//}

//void MainWindow::on_populateSpheres_clicked()
//{
//    int numberOfSpheres = mMainWindowUI->numberOfSpheres->text().toInt();
//    MainWindow::mOSGWidget->create_cylinder_in_x_direction(numberOfSpheres);
//}

//void MainWindow::on_runSimulationButton_clicked()
//{
//    bool On{1};
//    MainWindow::mOSGWidget->toggle_start(On);
//}

//void MainWindow::on_stopSimulationButton_clicked()
//{
//    bool Off{0};
//    MainWindow::mOSGWidget->toggle_start(Off);
//}

void MainWindow::on_extrusionMultiplierSlider_sliderMoved(int position)
{

}

void MainWindow::on_layerHeight_returnPressed()
{
    double layerHeight = mMainWindowUI->layerHeight->text().toDouble();
    MainWindow::mOSGWidget->set_layer_height(layerHeight);
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
    MainWindow::mOSGWidget->set_needle_diameter(needleDiameter);
    MainWindow::mOSGWidget->update();
}
