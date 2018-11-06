#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>

class OutputWindow;
class OSGWidget;
class SphereUpdateCallback;
class Vector3;

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
//    void on_gravityButton_clicked();
//    void on_defaultButton_clicked();
//    void on_populateSpheres_clicked();
    void on_stopSimulationButton_clicked();
    void on_runSimulationButton_clicked();

private:
    Ui::MainWindowForm *mMainWindowUI;
    OSGWidget *mOSGWidget{nullptr};
    SphereUpdateCallback *mSphereUpdateCallback{nullptr};

};

#endif // MAINWINDOW_H
