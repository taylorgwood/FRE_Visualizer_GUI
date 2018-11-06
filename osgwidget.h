#ifndef MEEN_570_OSGWIDGET
#define MEEN_570_OSGWIDGET

#include <QOpenGLWidget>

#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/CompositeViewer>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osgText/Text>
//#include "physicsObject.h"
//class Vector3;
//class PhysicsObject;

class OSGWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    OSGWidget(QWidget* parent = 0,
              Qt::WindowFlags f = 0);

    virtual ~OSGWidget();
    void create_cylinder_in_x_direction(int numberOfCylinders);
    void toggle_start(bool on);
    void toggle_stop(bool off);
    //    bool check_sphere_location(Vector3 shapePositionVector3, float sphereRadius);
    osg::Quat rotate_about_x_axis();
    osg::Quat rotate_about_y_axis();
    float get_diameter_of_print() const;
    void set_needle_diameter(const double needleDiameter);
    double get_needle_diameter() const;
    void set_extrusion_multiplier(const double extrusionMultiplier);
    double get_extrusion_multiplier() const;
    void set_infill_percentage(const double infillPercentage);
    double get_infill_percentage() const;
    void set_extrusion_width(const double extrusionWidth);
    double get_extrusion_width() const;
    void set_layer_height(const float layerHeight);
    float get_layer_height() const;
    void set_diameter_of_print(const double diameterOfPrint);
    void figure_out_math();
    double*** create_center_of_cylinder_array(double numberOfLayers, double numberOfCylindersPerLayer);
    void create_all_cylinders(double ***centerOfCylinderArray, double numberOfCylindersPerLayer, double numberOfLayers);

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
    void create_cylinder(osg::Vec3 shapePosition, float radius, float height, osg::Quat rotation, osg::Vec4 shapeRGBA);
    void create_new_wireframe();
    void animate_object(osg::Geode *geode, osg::Vec3 shapeLocation, float sphereRadius);
    void set_up_min_graphics_window();
    int  set_up_timer();

    //    PhysicsObject mObject;
    double pi{3.14159}; const
    double mDiameterOfSyringe{14.9};
    float  mDiameterOfPrint{0.26};
    double mNeedleDiameter{0.26};
    double mExtrusionMultiplier{1.0};
    double mInfillPercentage{100};
    double mExtrusionWidth{1.0};
    double mExtrusionWidthCalculated{mExtrusionWidth};
    float  mLayerHeight{0.26};
    float  mShapeWidth{10};
    float  mShapeHeight{10};
    float  mShapeLength{4};

    bool   mSimulationOn{false};
    //    std::vector<PhysicsObject*> *mObjectList;

};

#endif
