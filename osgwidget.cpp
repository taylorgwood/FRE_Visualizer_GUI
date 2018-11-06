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
    osg::Vec4 clearColorRGBA{0.2f,0.2f,0.2f,0.1f};
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

    osg::Vec3d cameraHomePosition{0.0,-20.0,3.0};
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

osg::ShapeDrawable *OSGWidget::create_graphic_sphere(osg::Vec3 shapePosition, float sphereRadius, osg::Vec4 shapeRGBA)
{
    osg::Sphere* sphere = new osg::Sphere(shapePosition, sphereRadius);
    osg::ShapeDrawable* newShape = new osg::ShapeDrawable(sphere);
    newShape->setColor(shapeRGBA);
    newShape->setName("Sphere");
    return newShape;
}

osg::Geode *OSGWidget::create_geometry_node(osg::ShapeDrawable* newShape)
{
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(newShape);
    osg::StateSet* stateSet = geode->getOrCreateStateSet();
    osg::Material* material = new osg::Material;
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    return geode;
}

void OSGWidget::create_new_wireframe()
{
    osg::Vec4 wireframeColorRGBA{0.2f,0.5f,1.f,0.1f};
    osg::Vec3d scaleFactor{5.f,5.f,5.f};
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


void OSGWidget::create_sphere(osg::Vec3 shapePosition, float sphereRadius, osg::Vec4 shapeRGBA)
{
    osg::Vec3 origin{0,0,0};
    osg::ShapeDrawable *newShape = create_graphic_sphere(origin, sphereRadius, shapeRGBA);
    osg::Geode *geode = create_geometry_node(newShape);
    animate_object(geode, shapePosition, sphereRadius);
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

void OSGWidget::populate_spheres(int numberOfSpheres)
{
    for(int i=0; i<1; i++)
    {
        float sphereRadius{1.0};
        osg::Vec3 shapePosition{0,0,0};
        osg::Vec4 shapeRGBA = {1,0,0,1};

        create_sphere(shapePosition, sphereRadius, shapeRGBA);
    }
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
//    populate_spheres(1);




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
