#include <random>
#include "osgwidget.h"
#include "wireframe.h"
//#include "vector3d.h"
//#include "physicsObject.h"
#include "sphereupdatecallback.h"

#include <osg/Camera>
#include <osg/Geode>
#include <osg/Material>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/BlendFunc>
#include <osgViewer/ViewerEventHandlers>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <cassert>
#include <QKeyEvent>
#include <QPainter>
#include <QWheelEvent>

void OSGWidget::set_up_environment()
{
    float aspectRatio = static_cast<float>( this->width() ) / static_cast<float>( this->height() );
    int pixelRatio   = this->devicePixelRatio();
    mView = create_scene(aspectRatio,pixelRatio);
    create_manipulator_and_viewer();
}

osg::Camera *OSGWidget::create_camera(float aspectRatio, int pixelRatio)
{
    osg::Camera* camera = new osg::Camera;
    int viewportOriginX{0};
    int viewportOriginY{0};
    camera->setViewport(viewportOriginX, viewportOriginY, this->width() * pixelRatio, this->height() * pixelRatio);
    osg::Vec4 clearColorRGBA{0.95f,0.95f,0.95f,0.1f};
    camera->setClearColor(clearColorRGBA);

    float viewingAngle{45};
    float nearestViewDistance{1};
    float farthestViewDistance{1000};
    camera->setProjectionMatrixAsPerspective(viewingAngle, aspectRatio, nearestViewDistance, farthestViewDistance);
    camera->setGraphicsContext(mGraphicsWindow);

    return camera;
}

void OSGWidget::create_manipulator_and_viewer()
{
    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
    manipulator->setAllowThrow(false);

    osg::Vec3d cameraHomePosition{20.0,20.0,10.0};
    osg::Vec3d cameraHomeViewDirection{0,0,0};
    osg::Vec3d cameraHomeUpPosition{0,0,1};
    manipulator->setHomePosition(cameraHomePosition,cameraHomeViewDirection,cameraHomeUpPosition);

    mView->setCameraManipulator(manipulator);
    mViewer->addView(mView);
    mViewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
    mViewer->realize();
    mView->home();
}

void OSGWidget::set_up_min_graphics_window()
{
    this->setFocusPolicy(Qt::StrongFocus);
    int minWindowWidth{100};
    int minWindowHeight{100};
    this->setMinimumSize(minWindowWidth, minWindowHeight);
    this->setMouseTracking(true);
}

osgViewer::View *OSGWidget::create_scene(float aspectRatio, int pixelRatio)
{
    osg::Camera *camera = create_camera(aspectRatio,pixelRatio);
    osgViewer::View *view = new osgViewer::View;
    view->setCamera(camera);
    view->setSceneData(mRoot.get());
    view->addEventHandler(new osgViewer::StatsHandler);
    return view;
}

int OSGWidget::set_up_timer()
{
    double framesPerSecond{30};
    double timeStep = 1.0/framesPerSecond;
    double timerDurationInMilliSeconds{timeStep * 1000};
    int timerID = startTimer(timerDurationInMilliSeconds);
    return timerID;
}

osg::ShapeDrawable *OSGWidget::create_graphic_cylinder(osg::Vec3 shapePosition, float radius, float height, osg::Quat rotation, osg::Vec4 shapeRGBA)
{
    osg::Cylinder* cylinder = new osg::Cylinder(shapePosition, radius, height);
    cylinder->setRotation(rotation);
    osg::ShapeDrawable* newShape = new osg::ShapeDrawable(cylinder);
    newShape->setColor(shapeRGBA);
    newShape->setName("Cylinder");
    return newShape;
}

osg::Geode *OSGWidget::create_geometry_node(osg::ShapeDrawable* newShape)
{
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(newShape);
    osg::StateSet* stateSet = geode->getOrCreateStateSet();
    stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    stateSet->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);
    osg::Material* material = new osg::Material;
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    return geode;
}

void OSGWidget::create_new_wireframe()
{
    osg::Vec4 wireframeColorRGBA{0.2f,0.5f,1.f,0.1f};
    float scaleFactorX = mShapeLength/2;
    float scaleFactorY = mShapeWidth/2;
    float scaleFactorZ = mShapeHeight/2;
    osg::Vec3d scaleFactor = {scaleFactorX,scaleFactorY,scaleFactorZ};
    osg::Node* wireFrame = create_wireframe(wireframeColorRGBA, scaleFactor);
    mRoot->addChild(wireFrame);
}

void OSGWidget::animate_object(osg::Geode *geode, osg::Vec3 shapePosition, float sphereRadius)
{
    osg::MatrixTransform *transform = new osg::MatrixTransform;
    //    Vector3 shapePositionVector3{0,0,0};
    //    shapePositionVector3.set_x(shapePosition.x());
    //    shapePositionVector3.set_y(shapePosition.y());
    //    shapePositionVector3.set_z(shapePosition.z());

    //    PhysicsObject* sphere = new PhysicsObject(shapePositionVector3, initialVelocity, mGravity);
    transform->setMatrix(osg::Matrix::translate(shapePosition));
    transform->setUpdateCallback(new SphereUpdateCallback(mSimulationOn, sphereRadius));
    transform->addChild(geode);
    mRoot->addChild(transform);
}


void OSGWidget::create_cylinder(osg::Vec3 shapePosition, float radius, float height, osg::Quat rotation, osg::Vec4 shapeRGBA)
{
    osg::Vec3 origin{0,0,0};
    osg::ShapeDrawable *newShape = create_graphic_cylinder(origin, radius, height, rotation, shapeRGBA);
    osg::Geode *geode = create_geometry_node(newShape);
    animate_object(geode, shapePosition, radius);
}

//void OSGWidget::populate_spheres(int numberOfSpheres)
//{
//    for(int i=0; i<numberOfSpheres; i++)
//    {
//        float sphereRadius = get_random_float(mRadiusMin,mRadiusMax);
//        float tolerance{0.01};
//        osg::Vec3 shapePosition = get_random_osgVec3(mPositonMin+sphereRadius+tolerance, mPositionMax-sphereRadius-tolerance);
//        Vector3 shapePositionVector3{0,0,0};
//        shapePositionVector3.set_x(shapePosition.x());
//        shapePositionVector3.set_y(shapePosition.y());
//        shapePositionVector3.set_z(shapePosition.z());
//        bool sphereLocationValid = check_sphere_location(shapePositionVector3, sphereRadius);
//        if (!sphereLocationValid)
//        {
//            populate_spheres(1);
//        }
//        osg::Vec4 shapeRGBA = get_random_color();
//        Vector3 initialVelocity = get_random_Vector3(mVelocityMin, mVelocityMax);
//        double cOfR = get_random_double(mCOfRMin, mCOfRMax);
//        double mass = get_random_double(mMassMin,mMassMax);
//        create_sphere(shapePosition, sphereRadius, initialVelocity, shapeRGBA, cOfR, mass);
//    }
//    update();
//}

osg::Quat OSGWidget::rotate_about_x_axis()
{
    double angleInDegrees = 90;
    double angleInRadians = osg::DegreesToRadians(angleInDegrees);
    osg::Vec3 rotationAxis{1,0,0};
    osg::Quat rotation{angleInRadians,rotationAxis};
    return rotation;
}

osg::Quat OSGWidget::rotate_about_y_axis()
{
    double angleInDegrees = 90;
    double angleInRadians = osg::DegreesToRadians(angleInDegrees);
    osg::Vec3 rotationAxis{0,1,0};
    osg::Quat rotation{angleInRadians,rotationAxis};
    return rotation;
}

float OSGWidget::get_diameter_of_print() const
{
    return mDiameterOfPrint;
}

void OSGWidget::set_needle_diameter(const double needleDiameter)
{
    mNeedleDiameter = needleDiameter;
}

double OSGWidget::get_needle_diameter() const
{
    return mNeedleDiameter;
}

void OSGWidget::set_extrusion_multiplier(const double extrusionMultiplier)
{
    mExtrusionMultiplier = extrusionMultiplier;
}

double OSGWidget::get_extrusion_multiplier() const
{
    return mExtrusionMultiplier;
}

void OSGWidget::set_infill_percentage(const double infillPercentage)
{
    mInfillPercentage = infillPercentage;
}

double OSGWidget::get_infill_percentage() const
{
    return mInfillPercentage;
}

void OSGWidget::set_extrusion_width(const double extrusionWidth)
{
    mExtrusionWidth = extrusionWidth;
}

double OSGWidget::get_extrusion_width() const
{
    return mExtrusionWidth;
}

void OSGWidget::set_layer_height(const double layerHeight)
{
    mLayerHeight = layerHeight;
}

double OSGWidget::get_layer_height() const
{
    return mLayerHeight;
}

void OSGWidget::create_cylinder_in_x_direction(int numberOfCylinders)
{
    for(int i=0; i<1; i++)
    {
        float diameterOfPrint = get_diameter_of_print();
        float length{mShapeLength};
        osg::Vec3 shapePosition{0,0,0};
        osg::Vec4 shapeRGBA = {1.0,1.0,0,0.2};

        osg::Quat rotation = rotate_about_y_axis();
        //        osg::Vec3 centeredAtZeroCompensation = {0,0,0};
        //        shapePosition = shapePosition-centeredAtZeroCompensation;
        create_cylinder(shapePosition, diameterOfPrint, length, rotation, shapeRGBA);
    }
    update();
}

void OSGWidget::create_axes()
{
    float radius{0.1};
    float height{1};
    osg::Vec3 shapePosition1{height/2,0,0};
    osg::Vec3 shapePosition2{0,height/2,0};
    osg::Vec3 shapePosition3{0,0,height/2};
    float moveDistanceX = mShapeLength/2;
    float moveDistanceY = mShapeWidth/2;
    float moveDistanceZ = mShapeHeight/2;
    shapePosition1 += {moveDistanceX,0,0};
    shapePosition2 += {0,moveDistanceY,0};
    shapePosition3 += {0,0,moveDistanceZ};
    osg::Vec4 shapeRGBA1 = {1.0,0,0,0.1};
    osg::Vec4 shapeRGBA2 = {0,1.0,0,0.1};
    osg::Vec4 shapeRGBA3 = {0,0,1.0,0.1};

    double angleInDegrees = 90;
    double angleInRadians = osg::DegreesToRadians(angleInDegrees);
    osg::Vec3 rotationaxis1{0,1,0};
    osg::Vec3 rotationaxis2{1,0,0};
    osg::Vec3 rotationaxis3{0,0,1};
    osg::Quat rotation1{angleInRadians,rotationaxis1};
    osg::Quat rotation2{angleInRadians,rotationaxis2};
    osg::Quat rotation3{angleInRadians,rotationaxis3};
    create_cylinder(shapePosition1, radius, height, rotation1, shapeRGBA1);
    create_cylinder(shapePosition2, radius, height, rotation2, shapeRGBA2);
    create_cylinder(shapePosition3, radius, height, rotation3, shapeRGBA3);

    update();
}

void OSGWidget::toggle_start(bool on)
{
    mSimulationOn = on;
}

void OSGWidget::toggle_stop(bool off)
{
    mSimulationOn = off;
}

OSGWidget::OSGWidget(QWidget* parent, Qt::WindowFlags flags):
    QOpenGLWidget{parent,flags},
    mGraphicsWindow{new osgViewer::GraphicsWindowEmbedded{this->x(),this->y(),this->width(),this->height()}},
    mViewer{new osgViewer::CompositeViewer}
{
    mRoot = new osg::Group;
    set_up_environment();
    create_new_wireframe();
    set_up_min_graphics_window();
    create_axes();
    create_cylinder_in_x_direction(1);

    mTimerId = set_up_timer();
}

OSGWidget::~OSGWidget()
{
    killTimer(mTimerId);
}

void OSGWidget::timerEvent(QTimerEvent *)
{
    if (mSimulationOn)
        update();
}

void OSGWidget::paintEvent(QPaintEvent*)
{
    this->makeCurrent();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    this->paintGL();

    painter.end();
    this->doneCurrent();
}

void OSGWidget::paintGL()
{
    mViewer->frame();
}

void OSGWidget::resizeGL(int width, int height)
{
    this->getEventQueue()->windowResize(this->x(), this->y(), width, height);
    mGraphicsWindow->resized(this->x(), this->y(), width, height);
    this->on_resize(width, height);
}

void OSGWidget::keyPressEvent(QKeyEvent* event)
{
    QString keyString   = event->text();
    const char* keyData = keyString.toLocal8Bit().data();

    if(event->key() == Qt::Key_H)
    {
        mView->home();
        return;
    }
    this->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KeySymbol(*keyData) );
}

void OSGWidget::keyReleaseEvent(QKeyEvent* event)
{
    QString keyString   = event->text();
    const char* keyData = keyString.toLocal8Bit().data();

    this->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KeySymbol(*keyData));
}

void OSGWidget::mouseMoveEvent(QMouseEvent* event)
{
    auto pixelRatio = this->devicePixelRatio();
    this->getEventQueue()->mouseMotion(static_cast<float>(event->x()*pixelRatio),
                                       static_cast<float>(event->y()*pixelRatio));
}

unsigned int OSGWidget::mouseButtonEvent(QMouseEvent* event)
{
    unsigned int button{0};
    unsigned int leftButton{1};
    unsigned int middleButton{2};
    unsigned int rightButton{3};

    switch(event->button())
    {
    case Qt::LeftButton:
        button = leftButton;
        break;

    case Qt::MiddleButton:
        button = middleButton;
        break;

    case Qt::RightButton:
        button = rightButton;
        break;

    default:
        break;
    }
    return button;
}

void OSGWidget::mousePressEvent(QMouseEvent* event)
{
    unsigned int button{0};
    button = mouseButtonEvent(event);
    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseButtonPress(static_cast<float>(event->x()*pixelRatio),
                                            static_cast<float>(event->y()*pixelRatio),
                                            button);
}

void OSGWidget::mouseReleaseEvent(QMouseEvent* event)
{
    unsigned int button{0};
    button = mouseButtonEvent(event);
    auto pixelRatio = this->devicePixelRatio();

    this->getEventQueue()->mouseButtonRelease(static_cast<float>(pixelRatio*event->x()),
                                              static_cast<float>(pixelRatio*event->y()),
                                              button);
}

void OSGWidget::wheelEvent( QWheelEvent* event )
{
    event->accept();
    int delta = event->delta();

    osgGA::GUIEventAdapter::ScrollingMotion motion = delta > 0 ?   osgGA::GUIEventAdapter::SCROLL_UP
                                                                 : osgGA::GUIEventAdapter::SCROLL_DOWN;
    this->getEventQueue()->mouseScroll(motion);
}

void OSGWidget::on_resize(int width, int height)
{
    std::vector<osg::Camera*> cameras;
    mViewer->getCameras(cameras);

    auto pixelRatio = this->devicePixelRatio();
    cameras[0]->setViewport(0, 0, width*pixelRatio, height*pixelRatio);
}

osgGA::EventQueue* OSGWidget::getEventQueue() const
{
    osgGA::EventQueue* eventQueue = mGraphicsWindow->getEventQueue();

    if(eventQueue)
        return eventQueue;
    else
        throw std::runtime_error("Unable to obtain valid event queue");
}

bool OSGWidget::event(QEvent* event)
{
    bool handled = QOpenGLWidget::event(event);
    repaint_osg_graphics_after_interaction(event);
    return handled;
}

void OSGWidget::repaint_osg_graphics_after_interaction(QEvent* event)
{
    if (!mSimulationOn)
    {
        switch(event->type())
        {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseMove:
        case QEvent::Wheel:
            this->update();
            break;

        default:
            break;
        }
    }
}
