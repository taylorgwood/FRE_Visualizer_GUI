#include "mainwindow.h"
#include "ui_mainwindowform.h"
#include "osgwidget.h"

#include <QDockWidget>
#include <QFileDialog>
#include <QColorDialog>
#include <QColor>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    mMainWindowUI{new Ui::MainWindowForm}
{
    mMainWindowUI->setupUi(this);
    QWidget::setWindowIcon(QIcon(":/myicons/myicon.ico"));
    mShape = new Shape();
    mGcode = new Gcode();
    mOSGWidget = new OSGWidget{mShape, this};

    this->setCentralWidget(mOSGWidget);

    set_volume_label();
    set_default_colors();
    set_default_settings();
    on_simplifyResolution_clicked(true);
    set_default_object_size();
    set_default_print_parameters();

    on_resetSettingsButton_clicked();
    on_resetObjectSizeButton_clicked();
    on_resetParametersButton_clicked();
    redraw_and_refresh_information();
    on_resetSettingsButton_clicked();
    on_resetObjectSizeButton_clicked();
    on_resetParametersButton_clicked();
    unsigned int numberOfPoints = static_cast<unsigned int> (mShape->get_simplified_point_list().size());
    mOSGWidget->set_animation_count(numberOfPoints-1);
    redraw_and_refresh_information();
}

MainWindow::~MainWindow()
{
    delete mMainWindowUI;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionStart_2_triggered()
{
    bool On{1};
    MainWindow::mOSGWidget->toggle_start(On);
}

void MainWindow::on_actionStop_2_triggered()
{
    bool Off{0};
    MainWindow::mOSGWidget->toggle_start(Off);
}

void MainWindow::on_actionView_Cylinders_triggered()
{
    view_cylinders();
}

void MainWindow::on_actionView_Print_Path_triggered()
{
    view_print_path();
}

void MainWindow::on_actionView_Wireframe_triggered()
{
    view_wireframe();
}

void MainWindow::on_actionView_Axes_triggered()
{
    view_axes();
}

void MainWindow::view_axes()
{
    bool viewAxes = mMainWindowUI->actionView_Axes->isChecked();
    MainWindow::mOSGWidget->view_axes(viewAxes);
    mOSGWidget->redraw();
}

void MainWindow::view_wireframe()
{
    bool viewWireframe = mMainWindowUI->actionView_Wireframe->isChecked();
    MainWindow::mOSGWidget->view_wireframe(viewWireframe);
    mOSGWidget->redraw();
}

void MainWindow::view_cylinders()
{
    bool viewCylinders = mMainWindowUI->actionView_Cylinders->isChecked();
    MainWindow::mOSGWidget->view_cylinders(viewCylinders);
    mOSGWidget->redraw();
}

void MainWindow::view_print_path()
{
    bool viewPrintPath = mMainWindowUI->actionView_Print_Path->isChecked();
    MainWindow::mOSGWidget->view_print_path(viewPrintPath);
    mOSGWidget->redraw();
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    int printParameters{0};
    int printPath{1};
    bool unchecked{0};
    bool checked{1};
    if (index == printParameters)
    {
        mMainWindowUI->actionView_Print_Path->setChecked(checked);
        mMainWindowUI->actionView_Cylinders->setChecked(unchecked);
    }
    if (index == printPath)
    {
        mMainWindowUI->actionView_Print_Path->setChecked(checked);
        mMainWindowUI->actionView_Cylinders->setChecked(unchecked);
    }
    view_cylinders();
    view_print_path();
    redraw_and_refresh_information();
}


//--------------------------------------------------------------------
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

void MainWindow::on_infillAngle_returnPressed()
{
    double infillAngle = mMainWindowUI->infillAngle->text().toDouble();
    mShape->set_infill_angle(infillAngle);
    redraw_and_refresh_information();
}

void MainWindow::on_extrusionWidth_returnPressed()
{
    double extrusionWidth = mMainWindowUI->extrusionWidth->text().toDouble();
    mShape->set_extrusion_width(extrusionWidth);
    redraw_and_refresh_information();
}

void MainWindow::on_materialResolution_returnPressed()
{
    double materialResolution = mMainWindowUI->materialResolution->text().toDouble();
    mShape->set_resolution_length(materialResolution);
}

void MainWindow::on_resetParametersButton_clicked()
{
    mShape = new Shape();
    mOSGWidget->reset_shape(mShape);
    set_default_print_parameters();
    on_applyParametersButton_clicked();
    on_objectSizeButton_clicked();
    redraw_and_refresh_information();
    reset_print_parameter_labels();
}

void MainWindow::on_applyParametersButton_clicked()
{
    double layerHeight = mMainWindowUI->layerHeight->text().toDouble();
    double extrusionMultiplier = mMainWindowUI->extrusionMultiplier->text().toDouble();
    double infillPercentage = mMainWindowUI->infillPercentage->text().toDouble();
    double infillAngle = mMainWindowUI->infillAngle->text().toDouble();
    double extrusionWidth = mMainWindowUI->extrusionWidth->text().toDouble();
    double materialResolution = mMainWindowUI->materialResolution->text().toDouble();
    mShape->reset_layer_height(layerHeight);
    mShape->set_extrusion_multiplier(extrusionMultiplier);
    mShape->set_infill_percentage(infillPercentage);
    mShape->set_infill_angle(infillAngle);
    mShape->set_extrusion_width(extrusionWidth); // change this to have auto adjust in the set function
    mShape->set_resolution_length(materialResolution);
    on_objectSizeButton_clicked();
    redraw_and_refresh_information();
}

void MainWindow::set_default_print_parameters()
{
    mNeedleDiameter = 0.26;
    mLayerHeight = 0.26;
    mExtrusionMultiplier = 1.0;
    mInfillPercentage = 80;
    mInfillAngle = 45;
    mExtrusionWidth = 0.26;
    mMaterialResolution = 1;
}

void MainWindow::reset_print_parameter_labels()
{
    mMainWindowUI->needleDiameterDisplay->setText(QString::number(mNeedleDiameter));
    mMainWindowUI->needleGauge->setValue(25);
    mMainWindowUI->layerHeight->setText(QString::number(mLayerHeight));
    mMainWindowUI->extrusionMultiplier->setText(QString::number(mExtrusionMultiplier));
    mMainWindowUI->infillPercentage->setText(QString::number(mInfillPercentage));
    mMainWindowUI->infillAngle->setText(QString::number(mInfillAngle));
    mMainWindowUI->extrusionWidth->setText(QString::number(mExtrusionWidth));
    mMainWindowUI->materialResolution->setText(QString::number(mMaterialResolution));
}

void MainWindow::set_layer_height_label()
{
    double trueLayerHeight = mShape->get_layer_height();
    mMainWindowUI->adjustedLayerHeightDisplay->setText(QString::number(trueLayerHeight));
}

void MainWindow::set_extrusion_width_label()
{
//    double trueExtrusionWidth = mShape->get_layer(1)->get_extrusion_width();
    //    mMainWindowUI->adjustedExtrusionWidthDisplay->setText(QString::number(trueExtrusionWidth));
}

void MainWindow::on_autoAdjustLayersButton_clicked(bool checked)
{
    mShape->set_auto_adjust_layer(checked);
    on_applyParametersButton_clicked();
    redraw_and_refresh_information();
}


//--------------------------------------------------------------------

void MainWindow::on_shapeWidth_returnPressed()
{
    double shapeWidth  = mMainWindowUI->shapeWidth->text().toDouble();
    mShape->set_width(shapeWidth);
    redraw_and_refresh_information();
}

void MainWindow::on_topWidth_returnPressed()
{
    double topWidth  = mMainWindowUI->topWidth->text().toDouble();
    mShape->set_top_width(topWidth);
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
    mShape->reset_height(shapeHeight);
    redraw_and_refresh_information();
}

void MainWindow::on_objectSizeButton_clicked()
{
    double shapeWidth  = mMainWindowUI->shapeWidth->text().toDouble();
    double topWidth    = mMainWindowUI->topWidth->text().toDouble();
    double shapeLength = mMainWindowUI->shapeLength->text().toDouble();
    double shapeHeight = mMainWindowUI->shapeHeight->text().toDouble();
    mShape->set_width(shapeWidth);
    mShape->set_top_width(topWidth);
    mShape->set_length(shapeLength);
    mShape->reset_height(shapeHeight);
    redraw_and_refresh_information();
}

void MainWindow::on_resetObjectSizeButton_clicked()
{
    set_default_object_size();
    on_objectSizeButton_clicked();
    //    redraw_and_refresh_information();
    reset_object_size_labels();
}

void MainWindow::set_default_object_size()
{
    mShapeWidth = 10;
    mTopWidth = 10;
    mShapeLength = 10;
    mShapeHeight = 10;
}

void MainWindow::reset_object_size_labels()
{
    mMainWindowUI->shapeWidth->setText(QString::number(mShapeWidth));
    mMainWindowUI->topWidth->setText(QString::number(mTopWidth));
    mMainWindowUI->shapeLength->setText(QString::number(mShapeLength));
    mMainWindowUI->shapeHeight->setText(QString::number(mShapeHeight));
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



//--------------------------------------------------------------------

void MainWindow::redraw_and_refresh_information()
{
    mShape->refresh();
    mOSGWidget->redraw();
    set_volume_label();
    set_layer_height_label();
    set_extrusion_width_label();
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
    view_cylinders();
    view_print_path();
    mOSGWidget->redraw();
}


//--------------------------------------------------------------------

void MainWindow::on_actionExport_G_code_triggered()
{
    on_applySettingsButton_clicked();
    on_objectSizeButton_clicked();
    on_applyParametersButton_clicked();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"",tr(""));
    Shape exportShape = *mShape;
    std::string exportFileName = fileName.toStdString();
    mGcode->generate_file(exportShape,exportFileName);
}

void MainWindow::on_actionExport_G_code_2_triggered()
{
    on_actionExport_G_code_triggered();
}


void MainWindow::on_animationSliderVertical_sliderMoved(int position)
{
    double doublePosition = static_cast<double>(position);
    double scaledPosition = doublePosition/100000;
    unsigned int numberOfPoints = static_cast<unsigned int>(mShape->get_point_list().size());
    if (mOSGWidget->get_simplify_point_list())
    {
        numberOfPoints = static_cast<unsigned int>(mShape->get_simplified_point_list().size());
    }
    unsigned int doubleNumberOfPoints = numberOfPoints*2;
    unsigned int animationCount = static_cast<unsigned int>(floor(scaledPosition*doubleNumberOfPoints));
    mOSGWidget->set_animation_count(animationCount);
    mOSGWidget->redraw();
}

void MainWindow::on_animateButton_toggled(bool checked)
{
    MainWindow::mOSGWidget->toggle_start(checked);
}


//--------------------------------------------------------------------

void MainWindow::on_colorButtonA_clicked()
{
    mColorA = QColorDialog::getColor();
    set_color_A(mColorA);
}

void MainWindow::on_colorButtonB_clicked()
{
    mColorB = QColorDialog::getColor();
    set_color_B(mColorB);
}

void MainWindow::set_color_A(QColor color)
{
    mOSGWidget->set_color_A(color);
    //    mMainWindowUI->selectedColorA->setText(color.name());
    mMainWindowUI->selectedColorA->setPalette(QPalette(color));
    mMainWindowUI->selectedColorA->setAutoFillBackground(true);
}

void MainWindow::set_color_B(QColor color)
{
    mOSGWidget->set_color_B(color);
    //    mMainWindowUI->selectedColorB->setText(color.name());
    mMainWindowUI->selectedColorB->setPalette(QPalette(color));
    mMainWindowUI->selectedColorB->setAutoFillBackground(true);
}

void MainWindow::set_default_colors()
{
    set_color_A(mColorA);
    set_color_B(mColorB);
}


//--------------------------------------------------------------------

void MainWindow::on_layerRetractionA_returnPressed()
{
    double layerRetractionADistance = mMainWindowUI->layerRetractionA->text().toDouble();
    mGcode->set_travel_A_retraction_distance(layerRetractionADistance);
}

void MainWindow::on_layerRetractionB_returnPressed()
{
    double layerRetractionBDistance = mMainWindowUI->layerRetractionB->text().toDouble();
    mGcode->set_travel_B_retraction_distance(layerRetractionBDistance);
}

void MainWindow::on_materialSwitchRetraction_returnPressed()
{
    double materialSwitchRetractionDistance = mMainWindowUI->materialSwitchRetraction->text().toDouble();
    mGcode->set_material_switch_retraction_distance(materialSwitchRetractionDistance);
}

void MainWindow::on_travelJump_returnPressed()
{
    double travelJumpDistance = mMainWindowUI->travelJump->text().toDouble();
    mGcode->set_travel_jump(travelJumpDistance);
}

void MainWindow::on_travelJog_returnPressed()
{
    double travelJog = mMainWindowUI->travelJog->text().toDouble();
    mGcode->set_travel_jog(travelJog);
}

void MainWindow::on_startPrintPlunge_returnPressed()
{
    double startPrintPlunge = mMainWindowUI->startPrintPlunge->text().toDouble();
    mGcode->set_start_print_plunge_distance(startPrintPlunge);
}

void MainWindow::on_finishPrintJump_returnPressed()
{
    double finishPrintJump = mMainWindowUI->finishPrintJump->text().toDouble();
    mGcode->set_finish_print_jump_distance(finishPrintJump);
}

void MainWindow::on_finishPrintJog_returnPressed()
{
    double finishPrintJog = mMainWindowUI->finishPrintJog->text().toDouble();
    mGcode->set_finish_print_jog_distance(finishPrintJog);
}

void MainWindow::on_printSpeed_returnPressed()
{
    double printSpeed = mMainWindowUI->printSpeed->text().toDouble();
    mGcode->set_print_speed(printSpeed);
}

void MainWindow::on_travelSpeed_returnPressed()
{
    double travelSpeed = mMainWindowUI->travelSpeed->text().toDouble();
    mGcode->set_travel_speed(travelSpeed);
}

void MainWindow::on_syringeDiameter_returnPressed()
{
    double syringeDiameter = mMainWindowUI->syringeDiameter->text().toDouble();
    mGcode->set_syringe_diameter(syringeDiameter);
}

void MainWindow::on_applySettingsButton_clicked()
{
    on_layerRetractionA_returnPressed();
    on_layerRetractionB_returnPressed();
    on_materialSwitchRetraction_returnPressed();
    on_travelJump_returnPressed();
    on_travelJog_returnPressed();
    on_startPrintPlunge_returnPressed();
    on_finishPrintJump_returnPressed();
    on_finishPrintJog_returnPressed();
    on_printSpeed_returnPressed();
    on_travelSpeed_returnPressed();
    on_syringeDiameter_returnPressed();
}

void MainWindow::on_resetSettingsButton_clicked()
{
    set_default_settings();
    reset_settings_labels();
    on_applySettingsButton_clicked();
}

void MainWindow::set_default_settings()
{
    mLayerRetractionDistance.at(0) = 0;
    mLayerRetractionDistance.at(1) = 0;
    mMaterialSwitchRetractionDistance = 0;
    mTravelJumpDistance = 0;
    mTravelJogDistance = 0;
    mStartPlungeDistance = 15;
    mFinishJumpDistance = 50;
    mFinishJogDistance  = 50;
    mPrintSpeed = 12;
    mTravelSpeed = 12;
    mSyringeDiameter = 14.9;
}

void MainWindow::reset_settings_labels()
{
    mMainWindowUI->layerRetractionA->setText(QString::number(mLayerRetractionDistance.at(0)));
    mMainWindowUI->layerRetractionB->setText(QString::number(mLayerRetractionDistance.at(1)));
    mMainWindowUI->materialSwitchRetraction->setText(QString::number(mMaterialSwitchRetractionDistance));
    mMainWindowUI->travelJump->setText(QString::number(mTravelJumpDistance));
    mMainWindowUI->travelJog->setText(QString::number(mTravelJogDistance));
    mMainWindowUI->startPrintPlunge->setText(QString::number(mStartPlungeDistance));
    mMainWindowUI->finishPrintJump->setText(QString::number(mFinishJumpDistance));
    mMainWindowUI->finishPrintJog->setText(QString::number(mFinishJogDistance));
    mMainWindowUI->syringeDiameter->setText(QString::number(mSyringeDiameter));
    mMainWindowUI->printSpeed->setText(QString::number(mPrintSpeed));
    mMainWindowUI->travelSpeed->setText(QString::number(mTravelSpeed));
}

void MainWindow::on_simplifyResolution_clicked(bool checked)
{
    mGcode->set_simplify_point_list(checked);
    mOSGWidget->set_simplify_point_list(checked);
}

void MainWindow::on_animationSliderVertical_valueChanged(int value)
{
    on_animationSliderVertical_sliderMoved(value);
}

void MainWindow::on_singleMaterial_clicked(bool checked)
{
    mSingleMaterial = checked;
    mMainWindowUI->extruderAEnabled->setEnabled(checked);
    mMainWindowUI->extruderBEnabled->setEnabled(checked);
    mGcode->set_single_material(checked);
}

void MainWindow::on_extruderAEnabled_clicked(bool checked)
{
    mExtruderAEnabled = checked;
    std::vector<bool> extruderChoice = mGcode->get_extruder_choice();
//    if (mMainWindowUI->extruderBEnabled->isChecked())
//    {
//        on_extruderBEnabled_clicked(true);
//        extruderChoice.at(1) = true;
//    }
    extruderChoice.at(0) = checked;
    mGcode->set_extruder_choice(extruderChoice);
}

void MainWindow::on_extruderBEnabled_clicked(bool checked)
{
    mExtruderBEnabled = checked;
    std::vector<bool> extruderChoice = mGcode->get_extruder_choice();
//    if (mMainWindowUI->extruderAEnabled->isChecked())
//    {
//        on_extruderAEnabled_clicked(true);
//        extruderChoice.at(0) = true;
//    }
    extruderChoice.at(1) = checked;
    mGcode->set_extruder_choice(extruderChoice);
}

