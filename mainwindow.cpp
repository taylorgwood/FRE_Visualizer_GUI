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

void MainWindow::on_runSimulationButton_clicked()
{
    bool On{1};
    MainWindow::mOSGWidget->toggle_start(On);
}

void MainWindow::on_stopSimulationButton_clicked()
{
    bool Off{0};
    MainWindow::mOSGWidget->toggle_start(Off);
}
