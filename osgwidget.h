#ifndef MEEN_570_OSGWIDGET
#define MEEN_570_OSGWIDGET

#include <QOpenGLWidget>
#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osgText/Text>
#include "shape.h"

class OSGWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    OSGWidget(Shape *newShape, QWidget* parent = nullptr, Qt::WindowFlags f = 0);

    virtual ~OSGWidget();
    void   toggle_start(bool on);
    void   set_view_along_x_axis();
    void   create_osg_cylinder(const osg::Vec3 &shapePosition, float radius, float height, const osg::Quat &rotation, const osg::Vec4 &shapeRGBA);
    void   create_cylinders();
//    osg::Node* draw_cylinders();
    osg::Vec3Array* get_vertex_data(Shape* shape);
    osg::Vec4Array* get_color_data(Shape* shape);
    osg::Quat rotate_about_x_axis();
    osg::Quat rotate_about_y_axis();
    void   clear_window();
    void   redraw();
    void   view_axes(bool On);
    void   view_wireframe(bool On);
    void   draw_print_path();

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
    osg::ref_ptr<osgGA::TrackballManipulator> create_manipulator();
    void set_original_home_position(osg::ref_ptr<osgGA::TrackballManipulator> manipulator);
    void set_manipulator_to_viewer(osg::ref_ptr<osgGA::TrackballManipulator> manipulator);
    osg::Camera *create_camera(float aspectRatio, int pixelRatio);
    osgViewer::View *create_scene(float aspectRatio, int pixelRatio);
    osg::ShapeDrawable *create_graphic_cylinder(const osg::Vec3 &shapeLocation, float radius, float height, const osg::Quat &rotation, const osg::Vec4 &shapeRGBA);
//    osg::Geode *create_geometry_node(osg::ShapeDrawable* newShape);
    void create_axes();
    void draw_wireframe();
    void set_up_min_graphics_window();
    int  set_up_timer();

    bool   mSimulationOn{false};
    Shape *mShape{nullptr};
    int    mRedrawCount{0};
    bool   mAxesOn{1};
    bool   mWireframeOn{1};
    double const pi{3.14159};

};

#endif
