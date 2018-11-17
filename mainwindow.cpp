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

//void MainWindow::on_extrusionMultiplierSlider_sliderMoved(int position)
//{
//    double extrusionMultiplierPercentage = mMainWindowUI->extrusionMultiplierSlider->value();
//    mExtrusionMultiplier = extrusionMultiplierPercentage/100;
//    apply_print_parameters();
//    redraw_print_parameters();
//}

void MainWindow::on_needleGauge_valueChanged(int arg1)
{    
    int needleGauge{arg1};
    set_needle_diameter(needleGauge);
    apply_print_parameters();
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
    set_needle_diameter_label();
}

void MainWindow::set_needle_diameter_label()
{
    double needleDiameterDouble = mNeedleDiameter;
    QString needleDiameterText = QString::number(needleDiameterDouble);
    mMainWindowUI->needleDiameterDisplay->setText(needleDiameterText);
}

void MainWindow::on_objectSizeButton_clicked()
{
    mShapeWidth  = mMainWindowUI->shapeWidth->text().toDouble();
    mShapeLength = mMainWindowUI->shapeLength->text().toDouble();
    mShapeHeight = mMainWindowUI->shapeHeight->text().toDouble();
    apply_object_size_parameters();
    redraw_object_size_parameters();
    set_volume_label();
}

void MainWindow::on_applyParametersButton_clicked()
{

    mLayerHeight = mMainWindowUI->layerHeight->text().toDouble();
    mExtrusionMultiplier = mMainWindowUI->extrusionMultiplier->text().toDouble();
    mInfillPercentage = mMainWindowUI->infillPercentage->text().toDouble();
    mExtrusionWidth = mMainWindowUI->extrusionWidth->text().toDouble();
    apply_print_parameters();
    redraw_print_parameters();
    set_volume_label();
}

void MainWindow::on_clearButton_clicked()
{
    mOSGWidget->clear_window();
}

void MainWindow::on_redrawButton_clicked()
{
    redraw_print_parameters();
    redraw_object_size_parameters();
    view_axes();
    view_wireframe();
    mOSGWidget->redraw();
}

//void MainWindow::on_autoUpdateButton_clicked(bool checked)
//{
//    MainWindow::mOSGWidget->toggle_start(checked);
//}

void MainWindow::on_layerHeight_returnPressed()
{
    mLayerHeight = mMainWindowUI->layerHeight->text().toDouble();
    apply_print_parameters();
    redraw_print_parameters();
}

void MainWindow::on_extrusionMultiplier_returnPressed()
{
    mExtrusionMultiplier = mMainWindowUI->extrusionMultiplier->text().toDouble();
    apply_print_parameters();
    redraw_print_parameters();
}

void MainWindow::on_infillPercentage_returnPressed()
{
    mInfillPercentage = mMainWindowUI->infillPercentage->text().toDouble();
    apply_print_parameters();
    redraw_print_parameters();
}

void MainWindow::on_extrusionWidth_returnPressed()
{
    mExtrusionWidth = mMainWindowUI->extrusionWidth->text().toDouble();
    apply_print_parameters();
    redraw_print_parameters();
}

void MainWindow::apply_print_parameters()
{
    mOSGWidget->apply_print_parameters(mNeedleDiameter,mExtrusionMultiplier,mInfillPercentage,mExtrusionWidth,mLayerHeight);
}

void MainWindow::redraw_print_parameters()
{
    mOSGWidget->redraw();
    set_volume_label();
}

void MainWindow::on_shapeWidth_returnPressed()
{
    mShapeWidth  = mMainWindowUI->shapeWidth->text().toDouble();
    apply_object_size_parameters();
    redraw_object_size_parameters();
}

void MainWindow::on_shapeLength_returnPressed()
{
    mShapeLength = mMainWindowUI->shapeLength->text().toDouble();
    apply_object_size_parameters();
    redraw_object_size_parameters();
}

void MainWindow::on_shapeHeight_returnPressed()
{
    mShapeHeight = mMainWindowUI->shapeHeight->text().toDouble();
    apply_object_size_parameters();
    redraw_object_size_parameters();
}

void MainWindow::apply_object_size_parameters()
{
    MainWindow::mOSGWidget->apply_object_size(mShapeWidth,mShapeLength,mShapeHeight);
}

void MainWindow::redraw_object_size_parameters()
{
    mOSGWidget->redraw();
    set_volume_label();
}

void MainWindow::set_volume_label()
{
    double objectVolumeDouble = mShapeHeight*mShapeLength*mShapeWidth;
    QString objectVolumeText = QString::number(objectVolumeDouble);
    mMainWindowUI->objectVolume->setText(objectVolumeText);

    double infillRatio = mInfillPercentage/100;
    double extrudedVolumeDouble = objectVolumeDouble*infillRatio*mExtrusionMultiplier;
    QString extrudedVolumeText = QString::number(extrudedVolumeDouble);
    mMainWindowUI->extrudedVolume->setText(extrudedVolumeText);
}


void MainWindow::on_resetParametersButton_clicked()
{
    set_default_print_parameters();
    apply_print_parameters();
    redraw_print_parameters();
    reset_print_parameter_labels();
}

void MainWindow::on_resetObjectSizeButton_clicked()
{
    set_default_object_size();
    apply_object_size_parameters();
    redraw_object_size_parameters();
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

void MainWindow::on_autoAdjustButton_clicked(bool checked)
{
    mOSGWidget->toggle_auto_adjust(checked);
}
