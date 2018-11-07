#include "osgwidget.h"
#include "shapeupdatecallback.h"

//ShapeUpdateCallback::ShapeUpdateCallback(PrintShape* printShape, bool &update, std::vector<PrintShape*> *shapeList) :
//    mUpdate {update},
//    mShapeList {shapeList}
ShapeUpdateCallback::ShapeUpdateCallback(bool &update) :
    mUpdate {update}
{
//    mPosition = cylinder->get_position();
//    mLength   = cylinder->get_length();
//    mColor    = cylinder->get_color();
//    mDiameter = cylinder->get_diameter();
//    mPosition = sphere->get_position();
//    mVelocity = sphere->get_velocity();
//    mAcceleration = sphere->get_acceleration();
//    mObject = new PhysicsObject(mPosition, mVelocity, mAcceleration, radius, mass, cOfR, fluidDensity);

//    mObjectList->push_back(mObject);
}

void ShapeUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
    if (mUpdate)
    {
//        update_physics();

//        Vector3 shapeLocation = mPosition;
//        osg::Vec3d shapeLocationVec3d = convert_Vector3_to_osgVec3d(shapeLocation);
//        osg::MatrixTransform *matrixTransform = dynamic_cast<osg::MatrixTransform *> (node);
//        osg::Matrixd translate = osg::Matrix::translate(shapeLocationVec3d);
//        matrixTransform->setMatrix(translate);
    }
    traverse(node, nv);
}

//void SphereUpdateCallback::update_physics()
//{
//    mObject->enable_drag_force(true);
//    mObject->pass_in_object_list(mObjectList);
//    mObject->update(get_timestep());
//    mObject->sphere_collision_loop();
//    mPosition = mObject->get_position();
//    mVelocity = mObject->get_velocity();
//    mAcceleration = mObject->get_acceleration();
//}

double ShapeUpdateCallback::get_timestep() const
{
    return mTimeStep;
}

float ShapeUpdateCallback::get_diameter_of_print() const
{
    return mDiameterOfPrint;
}

void ShapeUpdateCallback::set_needle_diameter(const double needleDiameter)
{
    mNeedleDiameter = needleDiameter;
}

double ShapeUpdateCallback::get_needle_diameter() const
{
    return mNeedleDiameter;
}

void ShapeUpdateCallback::set_extrusion_multiplier(const double extrusionMultiplier)
{
    mExtrusionMultiplier = extrusionMultiplier;
}

double ShapeUpdateCallback::get_extrusion_multiplier() const
{
    return mExtrusionMultiplier;
}

void ShapeUpdateCallback::set_infill_percentage(const double infillPercentage)
{
    mInfillPercentage = infillPercentage;
}

double ShapeUpdateCallback::get_infill_percentage() const
{
    return mInfillPercentage;
}

void ShapeUpdateCallback::set_extrusion_width(const double extrusionWidth)
{
    mExtrusionWidth = extrusionWidth;
}

double ShapeUpdateCallback::get_extrusion_width() const
{
    return mExtrusionWidth;
}

void ShapeUpdateCallback::set_layer_height(const float layerHeight)
{
    mLayerHeight = layerHeight;
}

float ShapeUpdateCallback::get_layer_height() const
{
    return mLayerHeight;
}

void ShapeUpdateCallback::set_diameter_of_print(const double diameterOfPrint)
{
    mDiameterOfPrint = diameterOfPrint;
}

void ShapeUpdateCallback::set_shape_size(const double shapeWidth, const double shapeLength, const double shapeHeight)
{
    mShapeWidth  = shapeWidth;
    mShapeLength = shapeLength;
    mShapeHeight = shapeHeight;
//    update();
}

float ShapeUpdateCallback::get_shape_width() const
{
    return mShapeWidth;
}

float ShapeUpdateCallback::get_shape_length() const
{
    return mShapeLength;
}

float ShapeUpdateCallback::get_shape_height() const
{
    return mShapeHeight;
}

void ShapeUpdateCallback::set_default_parameters()
{
    double diameterOfPrint{0.26};
    double needleDiameter{0.26};
    double extrusionMuliplier{1.0};
    double infillPercentage{100};
    double extrusionWidth{0.26};
    double layerHeight{0.26};
    double shapeWidth{10};
    double shapeLength{10};
    double shapeHeight{10};
    set_diameter_of_print(diameterOfPrint);
    set_needle_diameter(needleDiameter);
    set_extrusion_multiplier(extrusionMuliplier);
    set_infill_percentage(infillPercentage);
    set_extrusion_width(extrusionWidth);
    set_layer_height(layerHeight);
    set_shape_size(shapeWidth,shapeLength,shapeHeight);
}

void ShapeUpdateCallback::calculate_layer_properties()
{
    double volumePrintPerLayer = mShapeWidth*mShapeLength*mLayerHeight*mInfillPercentage*mExtrusionMultiplier;
    double volumeSyringePerLayer = volumePrintPerLayer;
    double areaOfSyringe = pi/4*mDiameterOfSyringe*mDiameterOfSyringe;
    double syringeExtrusionHeightPerLayer = volumeSyringePerLayer/areaOfSyringe;
    double mExtrusionWidthCalculated = mExtrusionMultiplier/mInfillPercentage;
    double diameterOfPrint = sqrt(volumePrintPerLayer*4*mExtrusionWidthCalculated/(mShapeWidth*mShapeLength*pi));
    set_diameter_of_print(diameterOfPrint);
    //    create_shape_location_vector();

    double numberOfLayers = floor(mShapeHeight/mLayerHeight);
    double numberOfCylindersPerLayer = floor(mShapeWidth/mExtrusionWidthCalculated);
    double*** centerOfCylinderArray = create_center_of_cylinder_array(numberOfLayers, numberOfCylindersPerLayer);
    create_all_cylinders(centerOfCylinderArray, numberOfCylindersPerLayer, numberOfLayers);
}

double*** ShapeUpdateCallback::create_center_of_cylinder_array(double numberOfLayers, double numberOfCylindersPerLayer)
{
    unsigned int numberOfOrthogonalDirections{3};
    double ***centerOfCylinderArray{nullptr};
    centerOfCylinderArray = new double**[numberOfCylindersPerLayer];
    for (unsigned int r{0}; r < numberOfCylindersPerLayer; r++) // 1:1:numberOfCircles
    {
        centerOfCylinderArray[r] = new double*[numberOfLayers];
        for (unsigned int c{0}; c<numberOfLayers; c++)
        {
            centerOfCylinderArray[r][c] = new double[numberOfOrthogonalDirections];
        }
    }

    for (unsigned int r{0}; r<numberOfCylindersPerLayer; r++)
    {
        for(unsigned int c{0}; c<numberOfLayers; c++)
        {
            unsigned int cylinderCount{r};
            unsigned int layerCount{c};
            float xLocation{0};
            float yLocation{0};
            float zLocation{c*mLayerHeight};
            if ((layerCount%2) == 0)
            {
                xLocation = cylinderCount*mExtrusionWidthCalculated;
                yLocation = -mShapeWidth/2;
            }
            else
            {
                xLocation = -mShapeLength/2;
                yLocation = cylinderCount*mExtrusionWidthCalculated;
            }
            centerOfCylinderArray[r][c][1] = xLocation;
            centerOfCylinderArray[r][c][2] = yLocation;
            centerOfCylinderArray[r][c][3] = zLocation;
        }
    }
    return centerOfCylinderArray;
}

void ShapeUpdateCallback::create_all_cylinders(double ***centerOfCylinderArray, double numberOfCylindersPerLayer, double numberOfLayers)
{
//    for (unsigned int r{0}; r<numberOfCylindersPerLayer; r++)
//    {
//        for(unsigned int c{0}; c<numberOfLayers; c++)
//        {
//            float diameterOfPrint = get_diameter_of_print();
//            float length{mShapeLength};
//            float positionX = centerOfCylinderArray[r][c][1];
//            float positionY = centerOfCylinderArray[r][c][2];
//            float positionZ = centerOfCylinderArray[r][c][3];

//            osg::Vec3 shapePosition{positionX,positionY,positionZ};
//            osg::Vec4 shapeRGBA = {1.0,1.0,0,0.2};

//            osg::Quat rotation = rotate_about_y_axis();
//            OSGWidget osgwidget;
//            osgwidget.create_cylinder(shapePosition, diameterOfPrint, length, rotation, shapeRGBA);
//        }
//    }
//    OSGWidget::update();
}

void ShapeUpdateCallback::create_cylinder_in_x_direction(int numberOfCylinders)
{
//    for(int i=0; i<numberOfCylinders; i++)
//    {
//        float diameterOfPrint = get_diameter_of_print();
//        float length{mShapeLength};
//        osg::Vec3 shapePosition{0,0,0};
//        osg::Vec4 shapeRGBA = {1.0,1.0,0,0.2};

//        osg::Quat rotation = rotate_about_y_axis();
//        OSGWidget osgwidget;
//        osgwidget.create_cylinder(shapePosition, diameterOfPrint, length, rotation, shapeRGBA);
//    }
//    OSGWidget::update();
}


