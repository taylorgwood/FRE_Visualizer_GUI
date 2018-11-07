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
    void on_layerHeight_returnPressed();
    void on_needleGauge_valueChanged(int arg1);
    void on_objectSizeButton_clicked();

private:
    Ui::MainWindowForm *mMainWindowUI;
    OSGWidget *mOSGWidget{nullptr};
    PrintShape *mPrintShape{nullptr};
//    ShapeUpdateCallback *mShapeUpdateCallback{nullptr};

};

#endif // MAINWINDOW_H
