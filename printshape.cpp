#include "osgwidget.h"
#include "printshape.h"


//ShapeUpdateCallback::ShapeUpdateCallback(PrintShape* printShape, bool &update, std::vector<PrintShape*> *shapeList) :
//    mUpdate {update},
//    mShapeList {shapeList}
PrintShape::PrintShape()
{
}

float PrintShape::get_diameter_of_print() const
{
    return mDiameterOfPrint;
}

void PrintShape::set_needle_diameter(const double needleDiameter)
{
    mNeedleDiameter = needleDiameter;
}

double PrintShape::get_needle_diameter() const
{
    return mNeedleDiameter;
}

void PrintShape::set_extrusion_multiplier(const double extrusionMultiplier)
{
    mExtrusionMultiplier = extrusionMultiplier;
}

double PrintShape::get_extrusion_multiplier() const
{
    return mExtrusionMultiplier;
}

void PrintShape::set_infill_percentage(const double infillPercentage)
{
    mInfillPercentage = infillPercentage;
}

double PrintShape::get_infill_percentage() const
{
    return mInfillPercentage;
}

void PrintShape::set_extrusion_width(const double extrusionWidth)
{
    mExtrusionWidth = extrusionWidth;
}

double PrintShape::get_extrusion_width() const
{
    return mExtrusionWidth;
}

void PrintShape::set_layer_height(const float layerHeight)
{
    mLayerHeight = layerHeight;
}

float PrintShape::get_layer_height() const
{
    return mLayerHeight;
}

void PrintShape::set_diameter_of_print(const double diameterOfPrint)
{
    mDiameterOfPrint = diameterOfPrint;
}

void PrintShape::set_shape_size(const double shapeWidth, const double shapeLength, const double shapeHeight)
{
    mShapeWidth  = shapeWidth;
    mShapeLength = shapeLength;
    mShapeHeight = shapeHeight;
    //    update();
}

float PrintShape::get_shape_width() const
{
    return mShapeWidth;
}

float PrintShape::get_shape_length() const
{
    return mShapeLength;
}

float PrintShape::get_shape_height() const
{
    return mShapeHeight;
}

void PrintShape::set_default_parameters()
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

void PrintShape::set_print_parameters(double dP, double nD, double eM, double iF, double eW, double lH)
{
    set_diameter_of_print(dP);
    set_needle_diameter(nD);
    set_extrusion_multiplier(eM);
    set_infill_percentage(iF);
    set_extrusion_width(eW);
    set_layer_height(lH);
}

double PrintShape::calculate_number_of_layers()
{
    double numberOfLayers = floor(mShapeHeight/mLayerHeight);
    return numberOfLayers;
}

double PrintShape::calculate_number_of_cylinders_per_layer()
{
    double numberOfCylindersPerLayer = floor(mShapeWidth/mExtrusionWidthCalculated);
    return numberOfCylindersPerLayer;
}

int PrintShape::calculate_number_of_cylinders()
{
    int numberOfCylindersPerLayer = floor(mShapeWidth/mExtrusionWidthCalculated);
    int numberOfLayers = floor(mShapeHeight/mLayerHeight);
    int numberOfCylinders = numberOfCylindersPerLayer*numberOfLayers;
    return numberOfCylinders;
}

double*** PrintShape::create_center_of_cylinder_array()
{
    int numberOfCylindersPerLayer = calculate_number_of_cylinders_per_layer();
    int numberOfLayers = calculate_number_of_layers();
    int numberOfOrthogonalDirections{3};
    double ***centerOfCylinderArray{nullptr};
    centerOfCylinderArray = new double**[numberOfCylindersPerLayer];
    for (int r{0}; r < numberOfCylindersPerLayer; r++) // 1:1:numberOfCircles
    {
        centerOfCylinderArray[r] = new double*[numberOfLayers];
        for (int c{0}; c<numberOfLayers; c++)
        {
            centerOfCylinderArray[r][c] = new double[numberOfOrthogonalDirections];
        }
    }

    for (int r{0}; r<numberOfCylindersPerLayer; r++)
    {
        for(int c{0}; c<numberOfLayers; c++)
        {
            int cylinderCount{r};
            int layerCount{c};
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

void PrintShape::calculate_layer_properties()
{
    double volumePrintPerLayer = mShapeWidth*mShapeLength*mShapeHeight*mInfillPercentage*mExtrusionMultiplier;
    double volumeSyringePerLayer = volumePrintPerLayer;
    double diameterOfSyringe{14.9};
    double areaSyringe = pi/4*diameterOfSyringe*diameterOfSyringe;
    double heightSyringePerLayerCalculated = volumeSyringePerLayer/areaSyringe;
    double extrusionWidthCalculated = mExtrusionWidth/mInfillPercentage;
    double diameterOfPrint = sqrt(volumePrintPerLayer*4*extrusionWidthCalculated/(mShapeWidth*mShapeLength*pi));
    set_diameter_of_print(diameterOfPrint);
}

//void PrintShape::create_all_cylinders(double ***centerOfCylinderArray, double numberOfCylindersPerLayer, double numberOfLayers)
//{
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
//}

//void PrintShape::create_cylinder_in_x_direction(int numberOfCylinders)
//{
//    //    for(int i=0; i<numberOfCylinders; i++)
//    //    {
//    //        float diameterOfPrint = get_diameter_of_print();
//    //        float length{mShapeLength};
//    //        osg::Vec3 shapePosition{0,0,0};
//    //        osg::Vec4 shapeRGBA = {1.0,1.0,0,0.2};

//    //        osg::Quat rotation = rotate_about_y_axis();
//    //        OSGWidget osgwidget;
//    //        osgwidget.create_cylinder(shapePosition, diameterOfPrint, length, rotation, shapeRGBA);
//    //    }
//    //    OSGWidget::update();
//}
