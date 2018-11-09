#include "osgwidget.h"
#include "printshape.h"


PrintShape::PrintShape()
{
}

PrintShape::~PrintShape()
{
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

void PrintShape::set_shape_size(const double shapeWidth, const double shapeLength, const double shapeHeight)
{
    mShapeWidth  = shapeWidth;
    mShapeLength = shapeLength;
    mShapeHeight = shapeHeight;
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
    double needleDiameter{0.26};
    double extrusionMuliplier{1.0};
    double infillPercentage{100};
    double extrusionWidth{0.26};
    double layerHeight{0.26};
    double shapeWidth{10};
    double shapeLength{10};
    double shapeHeight{10};
    set_needle_diameter(needleDiameter);
    set_extrusion_multiplier(extrusionMuliplier);
    set_infill_percentage(infillPercentage);
    set_extrusion_width(extrusionWidth);
    set_layer_height(layerHeight);
    set_shape_size(shapeWidth,shapeLength,shapeHeight);
}

void PrintShape::set_print_parameters(double nD, double eM, double iF, double eW, double lH)
{
    set_needle_diameter(nD);
    set_extrusion_multiplier(eM);
    set_infill_percentage(iF);
    set_extrusion_width(eW);
    set_layer_height(lH);
}

int PrintShape::calculate_number_of_layers()
{
    int numberOfLayers = floor(mShapeHeight/mLayerHeight);
    return numberOfLayers;
}

//int PrintShape::calculate_number_of_X_layers()
//{
//    //    int numberOfLayers = calculate_number_of_layers();
//    int numberOfXLayers = floor(mShapeHeight/mLayerHeight);
//    return numberOfXLayers;
//}

//int PrintShape::calculate_number_of_Y_layers()
//{
//    //    int numberOfLayers = calculate_number_of_layers();
//    int numberOfYLayers = floor(mShapeHeight/mLayerHeight);
//    return numberOfYLayers;
//}

int PrintShape::calculate_number_of_cylinders_per_X_layer()
{
    int numberOfXCylindersPerLayer = floor(mShapeWidth/mExtrusionWidthCalculated);
    return numberOfXCylindersPerLayer;
}

int PrintShape::calculate_number_of_cylinders_per_Y_layer()
{
    int numberOfYCylindersPerLayer = floor(mShapeLength/mExtrusionWidthCalculated);
    return numberOfYCylindersPerLayer;
}

double*** PrintShape::create_center_of_cylinder_array()
{

    calculate_diameter_of_print();
    int numberOfXCylindersPerLayer = calculate_number_of_cylinders_per_X_layer();
    int numberOfYCylindersPerLayer = calculate_number_of_cylinders_per_Y_layer();
    int numberOfCylindersPerLayer = numberOfXCylindersPerLayer+numberOfYCylindersPerLayer;
    int numberOfLayers = calculate_number_of_layers();
    int numberOfOrthogonalDirections{3};

    double ***centerOfCylinderArray{nullptr};
    centerOfCylinderArray = new double**[numberOfCylindersPerLayer];
    for (int r{0}; r < numberOfCylindersPerLayer; r++)
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
            float zLocation{0};
            if (cylinderCount<numberOfXCylindersPerLayer)
            {
                yLocation = mShapeWidth/2-cylinderCount*mExtrusionWidthCalculated;
                zLocation = -mShapeHeight/2+layerCount*mLayerHeight;
            }
            else
            {
                int cylinderYCount = cylinderCount-numberOfXCylindersPerLayer;
                xLocation = mShapeLength/2-cylinderYCount*mExtrusionWidthCalculated;
                zLocation = -mShapeHeight/2+(layerCount+0.5)*mLayerHeight;
            }
            centerOfCylinderArray[r][c][1] = xLocation;
            centerOfCylinderArray[r][c][2] = yLocation;
            centerOfCylinderArray[r][c][3] = zLocation;
        }
    }
    return centerOfCylinderArray;
}

float PrintShape::calculate_diameter_of_print()
{
    double infillRatio = mInfillPercentage/100;
    double volumePrintPerLayer = mShapeWidth*mShapeLength*mShapeHeight*infillRatio*mExtrusionMultiplier;
    double volumeSyringePerLayer = volumePrintPerLayer;
    double diameterOfSyringe{14.9};
    double areaSyringe = pi/4*diameterOfSyringe*diameterOfSyringe;
    double heightSyringePerLayerCalculated = volumeSyringePerLayer/areaSyringe;
    double extrusionWidthCalculated = mExtrusionWidth/mInfillPercentage;
    float diameterOfPrint = sqrt(volumePrintPerLayer*4*extrusionWidthCalculated/(mShapeWidth*mShapeLength*pi));
    return diameterOfPrint;
}

