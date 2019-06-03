#include <random>
#include "osgwidget.h"
#include "wireframe.h"

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
#include <osgUtil/Optimizer>

void OSGWidget::set_up_environment()
{
    float aspectRatio = static_cast<float>( this->width() ) / static_cast<float>( this->height() );
    int pixelRatio   = this->devicePixelRatio();
    mView = create_scene(aspectRatio,pixelRatio);
    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = create_manipulator();
    set_original_home_position(manipulator);
    set_manipulator_to_viewer(manipulator);
}

void OSGWidget::set_up_min_graphics_window()
{
    this->setFocusPolicy(Qt::StrongFocus);
    int minWindowWidth{100};
    int minWindowHeight{100};
    this->setMinimumSize(minWindowWidth, minWindowHeight);
    this->setMouseTracking(true);
}

osg::Camera *OSGWidget::create_camera(float aspectRatio, int pixelRatio)
{
    osg::Camera* camera = new osg::Camera;
    int viewportOriginX{0};
    int viewportOriginY{0};
    camera->setViewport(viewportOriginX, viewportOriginY, this->width() * pixelRatio, this->height() * pixelRatio);
    osg::Vec4 clearColorRGBA{0.00f,0.00f,0.15f,0.1f};
    camera->setClearColor(clearColorRGBA);

    float viewingAngle{45};
    float nearestViewDistance{1};
    float farthestViewDistance{1000};
    camera->setProjectionMatrixAsPerspective(viewingAngle, aspectRatio, nearestViewDistance, farthestViewDistance);
    camera->setGraphicsContext(mGraphicsWindow);
    return camera;
}

osg::ref_ptr<osgGA::TrackballManipulator> OSGWidget::create_manipulator()
{
    osg::ref_ptr<osgGA::TrackballManipulator> manipulator = new osgGA::TrackballManipulator;
    manipulator->setAllowThrow(false);
    return manipulator;
}

void OSGWidget::set_original_home_position(osg::ref_ptr<osgGA::TrackballManipulator> manipulator)
{
    osg::Vec3d cameraHomePosition{20.0,20.0,10.0};
    osg::Vec3d cameraHomeViewDirection{0,0,0};
    osg::Vec3d cameraHomeUpPosition{0,0,1};
    manipulator->setHomePosition(cameraHomePosition,cameraHomeViewDirection,cameraHomeUpPosition);
}

void OSGWidget::set_manipulator_to_viewer(osg::ref_ptr<osgGA::TrackballManipulator> manipulator)
{
    mView->setCameraManipulator(manipulator);
    mViewer->addView(mView);
    mViewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
    mViewer->realize();
    mView->home();
}

osgViewer::View *OSGWidget::create_scene(float aspectRatio, int pixelRatio)
{
    osg::Camera *camera = create_camera(aspectRatio,pixelRatio);
    osgViewer::View *view = new osgViewer::View;
    view->setCamera(camera);
    view->setSceneData(mRoot.get());
    view->addEventHandler(new osgViewer::StatsHandler);
    view->getEventQueue()->keyPress('s');
    return view;
}

int OSGWidget::set_up_timer()
{
    double framesPerSecond{30};
    double timeStep = 1.0/framesPerSecond;
    double timerDurationInMilliSeconds{timeStep * 1000};
    int timerID = startTimer(timerDurationInMilliSeconds, Qt::TimerType::PreciseTimer);
    return timerID;}

void OSGWidget::toggle_start(bool on)
{
    mSimulationOn = on;
}

void OSGWidget::clear_window()
{
    mRoot->removeChildren(0, mRoot->getNumChildren());
    this->update();
}

void OSGWidget::redraw()
{
    clear_window();
    if (mViewWireframe)
    {
        draw_wireframe();
    }
    if (mViewAxes)
    {
        create_axes();
    }
    if (mViewCylinders)
    {
        draw_cylinders();
    }
    if (mViewPrintPath)
    {
        draw_print_path();
    }
    update();
}

void OSGWidget::view_axes(bool On)
{
    mViewAxes = On;
}

void OSGWidget::view_wireframe(bool On)
{
    mViewWireframe = On;
}

void OSGWidget::view_cylinders(bool On)
{
    mViewCylinders = On;
}

void OSGWidget::view_print_path(bool On)
{
    mViewPrintPath = On;
}

void OSGWidget::reset_shape(Shape* newShape)
{
    mShape = newShape;
}

void OSGWidget::set_color_A(QColor color)
{
    mColorA = color;
}

QColor OSGWidget::get_color_A()
{
    return mColorA;
}

void OSGWidget::set_color_B(QColor color)
{
    mColorB = color;
}

QColor OSGWidget::get_color_B()
{
    return mColorB;
}

void OSGWidget::set_animation_count(unsigned int animationCount)
{
    unsigned int numberOfPoints = static_cast<unsigned int>(mShape->get_points().size());
    if (animationCount > numberOfPoints)
    {
        animationCount = 0;
    }
    mAnimationCount = animationCount;
    draw_print_path();
}

void OSGWidget::draw_print_path()
{
    Wireframe newWireframe;
    osg::Vec4Array* colorArray = get_color_data_array(mShape);
    osg::Vec3Array* vertexData = get_vertex_data_array(mShape);
    unsigned int numberOfPoints = static_cast<unsigned int>(mShape->get_points().size());
    if (mAnimationCount > numberOfPoints)
    {
        mAnimationCount = 0;
        if (mSimulationOn == false)
        {
            mAnimationCount = numberOfPoints;
        }
    }

    for (unsigned int i{mAnimationCount}; i<numberOfPoints; i++)
    {
        float A{0.2f};
        colorArray->at(i).a() = A;
    }

    osg::Node* animatedPrintPath = newWireframe.draw_print_path(colorArray, vertexData, mAnimationCount);
    mRoot->addChild(animatedPrintPath);
}

void OSGWidget::draw_cylinders()
{
    std::vector<osg::Vec3>* pathMidpoint = get_path_midpoint_locations(mShape);
    std::vector<osg::Vec3>* scaleData = get_path_scale_data(mShape);
    std::vector<osg::Quat>* rotationData = get_path_rotation_data(mShape);
    osg::ShapeDrawable* unitCylinder = create_unit_graphic_cylinder();
    float xLocation{0};
    float yLocation{0};
    float zLocation{0};
    osg::Vec3f center(xLocation,yLocation,zLocation);


    std::vector<Path>* shapePathList = mShape->get_path_list();
    unsigned int totalNumberOfPaths = static_cast<unsigned int>(shapePathList->size());
    osg::Group *allShapes = new osg::Group();

    float count{0};
    for (unsigned int i{0}; i<totalNumberOfPaths; i++)
    {
        osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform;
        osg::Vec3 scaleFactor = scaleData->at(i);
        osg::Vec3 position = pathMidpoint->at(i);
        osg::Quat rotation = rotationData->at(i);
        transform->setScale(scaleFactor);
//        transform->setPivotPoint(center);
        transform->setAttitude(rotation);
        transform->setPosition(position);
        transform->addChild(unitCylinder);
        allShapes->addChild(transform);
        count ++;
    }

    osg::StateSet* stateSet = allShapes->getOrCreateStateSet();
    stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    stateSet->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);

    osg::Material* material = new osg::Material;
    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );

    mRoot->addChild(allShapes);

    osgUtil::Optimizer optimizer;
    optimizer.optimize(mRoot);
}

osg::ShapeDrawable *OSGWidget::create_unit_graphic_cylinder()
{
    osg::Vec3 shapePosition{0,0,0};
    float radius{1};
    float height{1};
    osg::Vec4 shapeRGBA{0.8f,0.8f,1.0f,0.5f};
    osg::Cylinder* cylinder = new osg::Cylinder(shapePosition, radius, height);
    osg::ShapeDrawable* unitCylinder = new osg::ShapeDrawable(cylinder);
    unitCylinder->setColor(shapeRGBA);
    return unitCylinder;
}

void OSGWidget::draw_wireframe()
{
    osg::Vec4 wireframeColorRGBA{0.8f,0.8f,1.0f,0.1f};
    float scaleFactorX = mShape->get_width()/2;
    float scaleFactorY = mShape->get_length()/2;
    float scaleFactorZ = mShape->get_height()/2;
    osg::Vec3d scaleFactor = {scaleFactorX,scaleFactorY,scaleFactorZ};
    Wireframe newWireframe;
    osg::Node* wireframe = newWireframe.create_wireframe(wireframeColorRGBA, scaleFactor);
    mRoot->addChild(wireframe);
}

void OSGWidget::create_axis(const osg::Vec3& shapePosition, float radius, float height, const osg::Quat& rotation, const osg::Vec4& shapeRGBA)
{
    osg::ShapeDrawable *newShape = create_unit_graphic_cylinder();
    newShape->setColor(shapeRGBA);
    osg::StateSet* stateSet = newShape->getOrCreateStateSet();
    stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    stateSet->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);
    osg::Material* material = new osg::Material;
    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform;
    transform->setScale({radius,radius,height});
    transform->setPosition(shapePosition);
    transform->setAttitude(rotation);
    transform->addChild(newShape);
    mRoot->addChild(transform);
}

void OSGWidget::create_axes()
{
    float radius{0.1};
    float height{.5};
    osg::Vec3 shapePosition1{height/2,0,0};
    osg::Vec3 shapePosition2{0,height/2,0};
    osg::Vec3 shapePosition3{0,0,height/2};
    float moveDistanceX = mShape->get_length()/2;
    float moveDistanceY = mShape->get_width()/2;
    float moveDistanceZ = mShape->get_height()/2;
    shapePosition1 += {moveDistanceX,0,0};
    shapePosition2 += {0,moveDistanceY,0};
    shapePosition3 += {0,0,moveDistanceZ};
    osg::Vec4 red = {0.5,0,0,0.5};
    osg::Vec4 green = {0,0.5,0,0.5};
    osg::Vec4 blue = {0,0,0.5,0.5};

    double angleInDegrees = 90;
    double angleInRadians = osg::DegreesToRadians(angleInDegrees);
    osg::Vec3 rotationaxis1{0,1,0};
    osg::Vec3 rotationaxis2{1,0,0};
    osg::Vec3 rotationaxis3{0,0,1};
    osg::Quat rotation1{angleInRadians,rotationaxis1};
    osg::Quat rotation2{angleInRadians,rotationaxis2};
    osg::Quat rotation3{angleInRadians,rotationaxis3};
    create_axis(shapePosition1, radius, height, rotation1, red);
    create_axis(shapePosition2, radius, height, rotation2, green);
    create_axis(shapePosition3, radius, height, rotation3, blue);
    update();
}

osg::Vec3Array* OSGWidget::get_vertex_data_array(Shape* shape)
{
    double shapeHeight = shape->get_height();
    double shapeWidth  = shape->get_width();
    double shapeLength = shape->get_length();

    std::vector<Point> shapePointList = shape->get_points();
    unsigned int numberOfPoints = static_cast<unsigned int>(shapePointList.size());
    osg::Vec3Array* vertexData = new osg::Vec3Array{numberOfPoints};
    for (unsigned int i{0}; i<numberOfPoints; i++)
    {
        Point point = shapePointList.at(i);
        float xLocation = static_cast<float>(point.get_x()-shapeLength/2);
        float yLocation = static_cast<float>(point.get_y()-shapeWidth/2);
        float zLocation = static_cast<float>(point.get_z()-shapeHeight/2);
        vertexData->at(i) = (osg::Vec3(xLocation,yLocation,zLocation));
    }
    return vertexData;

}

osg::Vec4Array* OSGWidget::get_color_data_array(Shape* shape)
{
    std::vector<Point> shapePointList = shape->get_points();
    unsigned int numberOfPoints = static_cast<unsigned int>(shapePointList.size());
    osg::Vec4Array* color = new osg::Vec4Array{numberOfPoints};
    QColor colorA = get_color_A();
    QColor colorB = get_color_B();
    for (unsigned int i{0}; i< numberOfPoints; i++)
    {
        Point point = shapePointList[i];
        float material = static_cast<float>(point.get_material());
        float multiplierColorA = material/255;
        float multiplierColorB = (1-material)/255;
        float R = colorA.red()*multiplierColorA + colorB.red()*multiplierColorB;
        float G = colorA.green()*multiplierColorA + colorB.green()*multiplierColorB;
        float B = colorA.blue()*multiplierColorA + colorB.blue()*multiplierColorB;
        float A = 0.9f;
        osg::Vec4 currentColor{R,G,B,A};
        color->at(i) = (currentColor);
    }
    return color;
}

std::vector<osg::Vec3> *OSGWidget::get_path_midpoint_locations(Shape* shape)
{
    double shapeHeight = shape->get_height();
    double shapeWidth  = shape->get_width();
    double shapeLength = shape->get_length();

    std::vector<Path>* shapePathList = shape->get_path_list();
    unsigned int totalNumberOfPaths = static_cast<unsigned int>(shapePathList->size());
    std::vector<osg::Vec3>* midpoint = new std::vector<osg::Vec3>(totalNumberOfPaths);

    int numberOfLayers = shape->get_number_of_layers();
    unsigned int count{0};
    for (int j{0}; j<numberOfLayers; j++)
    {
        int layerNumber{j};
        Layer* layer = shape->get_layer(layerNumber);
        std::vector<Path*> layerPathList = layer->get_path_list();
        unsigned int numberOfPaths = static_cast<unsigned int> (layerPathList.size());
        for (unsigned int i{0}; i< numberOfPaths; i++)
        {
            unsigned int pathNumber{i};
            Path* path  = layerPathList[pathNumber];
            Point start = path->get_start();
            Point end = path->get_end();
            float xLocation{0};
            float yLocation{0};
            if (layerNumber%2 == 0)
            {
                xLocation = 0;//-shapeLength/2;
                yLocation = end.get_y()-shapeLength/2;
            }
            else
            {
                xLocation = start.get_x()-shapeWidth/2;
                yLocation = 0;//-shapeWidth/2;
            }
            //            float xMidpoint = static_cast<float>(start.get_x()-end.get_x());
            //            float yMidpoint = static_cast<float>(start.get_y()-end.get_y());
            //            float zMidpoint = static_cast<float>(start.get_z()-end.get_z());
            //            float xLocation = 0; //(xMidpoint)+shapeWidth/2;
            //            float yLocation = -shapeWidth/2;
            float zLocation = static_cast<float>(start.get_z())-shapeHeight/2;
            midpoint->at(count) = osg::Vec3(xLocation,yLocation,zLocation);
            count++;
        }
    }
    return midpoint;
}

std::vector<osg::Vec3>* OSGWidget::get_path_scale_data(Shape* shape)
{
    std::vector<Path>* shapePathList = shape->get_path_list();
    unsigned int totalNumberOfPaths = static_cast<unsigned int>(shapePathList->size());
    std::vector<osg::Vec3>* scaleData = new std::vector<osg::Vec3>(totalNumberOfPaths);

    for (unsigned int i{0}; i< totalNumberOfPaths; i++)
    {
        Path path = shapePathList->at(i);
        float diameter = static_cast<float>(path.get_diameter());
        float radius = diameter/2;
        float length = static_cast<float>(path.get_length());
        osg::Vec3 scaleFactor{radius,radius,length};
        scaleData->at(i) = (scaleFactor);
    }
    return scaleData;
}

std::vector<osg::Quat>* OSGWidget::get_path_rotation_data(Shape* shape)
{
    std::vector<Path>* shapePathList = shape->get_path_list();
    unsigned int totalNumberOfPaths = static_cast<unsigned int>(shapePathList->size());
    std::vector<osg::Quat>* rotationData= new std::vector<osg::Quat>(totalNumberOfPaths);

    for (unsigned int i{0}; i< totalNumberOfPaths; i++)
    {
        Path path = shapePathList->at(i);
        Point start = path.get_start();
        Point end = path.get_end();
        Point pointVector = start-end;
        Point direction = pointVector.normalize();
        Point initialOrientation(0,0,1);
        Point orthogonalVector = initialOrientation.cross(direction);
        osg::Quat rotation = get_rotation(orthogonalVector);
        rotationData->at(i) = (rotation);
    }
    return rotationData;
}

osg::Quat OSGWidget::get_rotation(Point orthagonalVector)
{
    float x = static_cast<float>(orthagonalVector.get_x());
    float y = static_cast<float>(orthagonalVector.get_y());
    float z = static_cast<float>(orthagonalVector.get_z());
    //    double absoluteValueX = abs(x);
    //    double absoluteValueY = abs(y);
    osg::Vec3 rotationAxis = {x,y,z};
    //    if (absoluteValueX>absoluteValueY)
    //    {
    //        rotationAxis = {0,1,0};
    //    }
    //    else
    //    {
    //        rotationAxis = {1,0,0};
    //    }
    // a.dot(^b) = |a|cos(theta)
    // theta = acos(a.dot(^b)/|a|)
//    Point a = direction;
//    Point b(0,1,0);
//    double magA = a.get_magnitude();
//    double angleInRadians = std::acos(a.dot(b)/(magA));
    double angleInDegrees = 90;
    double angleInRadians = osg::DegreesToRadians(angleInDegrees);
    osg::Quat rotation{angleInRadians,rotationAxis};
    //    double w{0};
    //    osg::Vec4d vec4d(pointVector.get_x(),pointVector.get_y(),pointVector.get_z(),w);
    //    osg::Quat rotation2{vec4d};
    return rotation;
}

osg::Quat OSGWidget::get_rotation_about_x_axis()
{
    double angleInDegrees = 90;
    double angleInRadians = osg::DegreesToRadians(angleInDegrees);
    osg::Vec3 rotationAxis{1,0,0};
    osg::Quat rotation{angleInRadians,rotationAxis};
    return rotation;
}

osg::Quat OSGWidget::get_rotation_about_y_axis()
{
    double angleInDegrees = 90;
    double angleInRadians = osg::DegreesToRadians(angleInDegrees);
    osg::Vec3 rotationAxis{0,1,0};
    osg::Quat rotation{angleInRadians,rotationAxis};
    return rotation;
}


OSGWidget::OSGWidget(Shape* newShape, QWidget* parent, Qt::WindowFlags flags):
    QOpenGLWidget{parent,flags},
    mGraphicsWindow{new osgViewer::GraphicsWindowEmbedded{this->x(),this->y(),this->width(),this->height()}},
    mViewer{new osgViewer::CompositeViewer}
{
    mRoot = new osg::Group;
    mShape = newShape;
    set_up_environment();
    set_up_min_graphics_window();
    redraw();

    mTimerId = set_up_timer();
}

OSGWidget::~OSGWidget()
{
    killTimer(mTimerId);
}

void OSGWidget::timerEvent(QTimerEvent *)
{
    if (mSimulationOn)
    {
        update();
        redraw();
        mAnimationCount++;
        int numberOfPoints = static_cast<int>(mShape->get_points().size());
        if (mAnimationCount > numberOfPoints)
        {
            mAnimationCount = 0;
        }
    }
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
