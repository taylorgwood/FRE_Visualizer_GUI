#ifndef MEEN_570_OSGWIDGET
#define MEEN_570_OSGWIDGET

#include <QOpenGLWidget>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osgText/Text>
//#include "shapeupdatecallback.h"
#include "printshape.h"
class PrintShape;

class OSGWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    OSGWidget(QWidget* parent = 0,
              Qt::WindowFlags f = 0);

    virtual ~OSGWidget();
    void  toggle_start(bool on);
    void  toggle_stop(bool off);
    void  set_view_along_x_axis();
    void  create_osg_cylinder(osg::Vec3 shapePosition, float radius, float height, osg::Quat rotation, osg::Vec4 shapeRGBA);
    void create_cylinders();
    osg::Quat rotate_about_x_axis();
    osg::Quat rotate_about_y_axis();
    void clear_window();
    void redraw();
    void set_shape_size(const double shapeWidth, const double shapeLength, const double shapeHeight);
    void set_print_parameters(double dP, double nD, double eM, double iF, double eW, double lH);
    //    void reset_parameters(UserInput userInput);

protected:
    virtual void paintEvent(QPaintEvent* paintEvent);
    virtual void paintGL();
    virtual void resizeGL(int width, int height);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    unsigned int mouseButtonEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual bool event(QEvent* event);
    void repaint_osg_graphics_after_interaction(QEvent* event);
    void timerEvent(QTimerEvent *);


private:
    virtual void on_resize(int width, int height);
    osgGA::EventQueue* getEventQueue() const;

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGraphicsWindow;
    osg::ref_ptr<osgViewer::CompositeViewer> mViewer;
    osg::ref_ptr<osgViewer::View> mView;
    osg::ref_ptr<osg::Group> mRoot;
    int mTimerId{0};

    void set_up_environment();
    void create_manipulator_and_viewer();
    osg::Camera *create_camera(float aspectRatio, int pixelRatio);
    osgViewer::View *create_scene(float aspectRatio, int pixelRatio);
    osg::ShapeDrawable *create_graphic_cylinder(osg::Vec3 shapeLocation, float radius, float height, osg::Quat rotation, osg::Vec4 shapeRGBA);
    osg::Geode *create_geometry_node(osg::ShapeDrawable* newShape);
    void create_axes();
    void create_new_wireframe();
    void set_up_min_graphics_window();
    int  set_up_timer();

    bool   mSimulationOn{false};
    //    ShapeUpdateCallback *mShapeUpdateCallback{nullptr};
    PrintShape *mPrintShape{nullptr};
    std::vector<PrintShape*> *mShapeList;
    int mRedrawCount{0};

};

#endif
