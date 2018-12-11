#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include "osgwidget.h"

#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    mMainWindowUI{new Ui::MainWindowForm}
{
    mMainWindowUI->setupUi(this);
    mShape = new Shape();
    mOSGWidget = new OSGWidget{mShape, this};
    this->setCentralWidget(mOSGWidget);
    set_volume_label();
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

void MainWindow::view_axes()
{
    bool viewAxes = mMainWindowUI->actionView_axes->isChecked();
    MainWindow::mOSGWidget->view_axes(viewAxes);
    mOSGWidget->redraw();
}

void MainWindow::view_wireframe()
{
    bool viewWireframe = mMainWindowUI->actionView_wireframe->isChecked();
    MainWindow::mOSGWidget->view_wireframe(viewWireframe);
    mOSGWidget->redraw();
}

void MainWindow::on_needleGauge_valueChanged(int arg1)
{    
    int needleGauge{arg1};
    set_needle_diameter(needleGauge);
    set_needle_diameter_label();
}

void MainWindow::set_needle_diameter(int needleGauge)
{
    double needleDiameter{0.26};

    switch (needleGauge)
    {
    case 20:
        needleDiameter = 0.603;
        break;
    case 21:
        needleDiameter = 0.514;
        break;
    case 22:
        needleDiameter = 0.413;
        break;
    case 23:
        needleDiameter = 0.337;
        break;
    case 24:
        needleDiameter = 0.311;
        break;
    case 25:
        needleDiameter = 0.260;
        break;
    case 26:
        needleDiameter = 0.260;
        break;
    case 27:
        needleDiameter = 0.210;
        break;
    case 28:
        needleDiameter = 0.184;
        break;
    case 29:
        needleDiameter = 0.184;
        break;
    case 30:
        needleDiameter = 0.159;
    }
    mNeedleDiameter = needleDiameter;
}

void MainWindow::set_needle_diameter_label()
{
    double needleDiameterDouble = mNeedleDiameter;
    QString needleDiameterText = QString::number(needleDiameterDouble);
    mMainWindowUI->needleDiameterDisplay->setText(needleDiameterText);
}

void MainWindow::on_objectSizeButton_clicked()
{
    double shapeWidth  = mMainWindowUI->shapeWidth->text().toDouble();
    double shapeLength = mMainWindowUI->shapeLength->text().toDouble();
    double shapeHeight = mMainWindowUI->shapeHeight->text().toDouble();
    mShape->set_width(shapeWidth);
    mShape->set_length(shapeLength);
    mShape->set_height(shapeHeight);
    redraw_and_refresh_information();
}

void MainWindow::on_applyParametersButton_clicked()
{ 
    double layerHeight = mMainWindowUI->layerHeight->text().toDouble();
    double extrusionMultiplier = mMainWindowUI->extrusionMultiplier->text().toDouble();
    double infillPercentage = mMainWindowUI->infillPercentage->text().toDouble();
    double extrusionWidth = mMainWindowUI->extrusionWidth->text().toDouble();
    mShape->reset_layer_height(layerHeight);
    mShape->set_extrusion_multiplier(extrusionMultiplier);
    mShape->set_infill_percentage(infillPercentage);
    mShape->set_extrusion_width(extrusionWidth); // change this to have auto adjust in the set function
    redraw_and_refresh_information();
}

void MainWindow::on_clearButton_clicked()
{
    mOSGWidget->clear_window();
}

void MainWindow::on_redrawButton_clicked()
{
    redraw_and_refresh_information();
    redraw_and_refresh_information();
    view_axes();
    view_wireframe();
    mOSGWidget->redraw();
}

void MainWindow::on_autoUpdateButton_clicked(bool checked)
{
    //    MainWindow::mOSGWidget->toggle_start(checked);
}

void MainWindow::on_layerHeight_returnPressed()
{
    double layerHeight = mMainWindowUI->layerHeight->text().toDouble();
    mShape->reset_layer_height(layerHeight);
    redraw_and_refresh_information();
}

void MainWindow::on_extrusionMultiplier_returnPressed()
{
    double extrusionMultiplier = mMainWindowUI->extrusionMultiplier->text().toDouble();
    mShape->set_extrusion_multiplier(extrusionMultiplier);
    redraw_and_refresh_information();
}

void MainWindow::on_infillPercentage_returnPressed()
{
    double infillPercentage = mMainWindowUI->infillPercentage->text().toDouble();
    mShape->set_infill_percentage(infillPercentage);
    redraw_and_refresh_information();
}

void MainWindow::on_extrusionWidth_returnPressed()
{
    double extrusionWidth = mMainWindowUI->extrusionWidth->text().toDouble();
    mShape->set_extrusion_width(extrusionWidth);
    redraw_and_refresh_information();
}

void MainWindow::on_shapeWidth_returnPressed()
{
    double shapeWidth  = mMainWindowUI->shapeWidth->text().toDouble();
    mShape->set_width(shapeWidth);
    redraw_and_refresh_information();
}

void MainWindow::on_shapeLength_returnPressed()
{
    double shapeLength = mMainWindowUI->shapeLength->text().toDouble();
    mShape->set_length(shapeLength);
    redraw_and_refresh_information();
}

void MainWindow::on_shapeHeight_returnPressed()
{
    double shapeHeight = mMainWindowUI->shapeHeight->text().toDouble();
    mShape->set_height(shapeHeight);
    redraw_and_refresh_information();
}

void MainWindow::redraw_and_refresh_information()
{
    mOSGWidget->redraw();
    set_volume_label();
}

void MainWindow::set_volume_label()
{
    double shapeWidth  = mMainWindowUI->shapeWidth->text().toDouble();
    double shapeLength = mMainWindowUI->shapeLength->text().toDouble();
    double shapeHeight = mMainWindowUI->shapeHeight->text().toDouble();
    double objectVolumeDouble = shapeHeight*shapeLength*shapeWidth;
    QString objectVolumeText = QString::number(objectVolumeDouble);
    mMainWindowUI->objectVolume->setText(objectVolumeText);

    double infillPercentage = mMainWindowUI->infillPercentage->text().toDouble();
    double infillRatio = infillPercentage/100;
    double extrusionMultiplier = mMainWindowUI->extrusionMultiplier->text().toDouble();
    double extrudedVolumeDouble = objectVolumeDouble*infillRatio*extrusionMultiplier;
    QString extrudedVolumeText = QString::number(extrudedVolumeDouble);
    mMainWindowUI->extrudedVolume->setText(extrudedVolumeText);
}


void MainWindow::on_resetParametersButton_clicked()
{
    set_default_print_parameters();
    redraw_and_refresh_information();
    reset_print_parameter_labels();
}

void MainWindow::on_resetObjectSizeButton_clicked()
{
    set_default_object_size();
    redraw_and_refresh_information();
    reset_object_size_labels();
}

void MainWindow::set_default_print_parameters()
{
    mNeedleDiameter = 0.26;
    mExtrusionMultiplier = 0.785;
    mInfillPercentage = 100;
    mExtrusionWidth = 0.26;
    mLayerHeight = 0.26;
}

void MainWindow::set_default_object_size()
{
    mShapeWidth = 10;
    mShapeLength = 10;
    mShapeHeight = 10;
}

void MainWindow::reset_print_parameter_labels()
{
    mMainWindowUI->needleDiameterDisplay->setText(QString::number(mNeedleDiameter));
    mMainWindowUI->needleGauge->setValue(25);
    mMainWindowUI->extrusionMultiplier->setText(QString::number(mExtrusionMultiplier));
    mMainWindowUI->infillPercentage->setText(QString::number(mInfillPercentage));
    mMainWindowUI->extrusionWidth->setText(QString::number(mExtrusionWidth));
    mMainWindowUI->layerHeight->setText(QString::number(mLayerHeight));
}

void MainWindow::reset_object_size_labels()
{
    mMainWindowUI->shapeWidth->setText(QString::number(mShapeWidth));
    mMainWindowUI->shapeLength->setText(QString::number(mShapeLength));
    mMainWindowUI->shapeHeight->setText(QString::number(mShapeHeight));
}

void MainWindow::on_autoAdjustLayersButton_clicked(bool checked)
{
    mShape->set_auto_adjust_layer(checked);
    redraw_and_refresh_information();
}

void MainWindow::on_autoAdjustWidthButton_clicked(bool checked)
{
    mShape->set_auto_adjust_path(checked);
    redraw_and_refresh_information();
}
