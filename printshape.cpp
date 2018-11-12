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

void PrintShape::set_print_parameters(double needleDiameter, double extrusionMultiplier, double infillPercentage, double extrusionWidth, double layerHeight)
{
    set_needle_diameter(needleDiameter);
    set_extrusion_multiplier(extrusionMultiplier);
    set_infill_percentage(infillPercentage);
    set_extrusion_width(extrusionWidth);
    set_layer_height(layerHeight);
}

int PrintShape::get_number_of_XYlayers()
{
    int numberOfXYLayers = floor(mShapeHeight/mLayerHeight/2);
    return numberOfXYLayers;
}

int PrintShape::get_number_of_cylinders_per_X_layer()
{
    int numberOfXCylindersPerLayer = floor(mShapeWidth/mExtrusionWidthCalculated);
    return numberOfXCylindersPerLayer;
}

int PrintShape::get_number_of_cylinders_per_Y_layer()
{
    int numberOfYCylindersPerLayer = floor(mShapeLength/mExtrusionWidthCalculated);
    return numberOfYCylindersPerLayer;
}

double*** PrintShape::create_center_of_cylinder_array()
{
    get_diameter_of_print();
    int numberOfXCylindersPerLayer = get_number_of_cylinders_per_X_layer();
    int numberOfYCylindersPerLayer = get_number_of_cylinders_per_Y_layer();
    int numberOfCylindersPerLayer = numberOfXCylindersPerLayer+numberOfYCylindersPerLayer;
    int numberOfXYLayers = get_number_of_XYlayers();
    int numberOfOrthogonalDirections{3};

    double ***centerOfCylinderArray{nullptr};
    centerOfCylinderArray = new double**[numberOfCylindersPerLayer];
    for (int r{0}; r < numberOfCylindersPerLayer; r++)
    {
        centerOfCylinderArray[r] = new double*[numberOfXYLayers];
        for (int c{0}; c<numberOfXYLayers; c++)
        {
            centerOfCylinderArray[r][c] = new double[numberOfOrthogonalDirections];
        }
    }

    for (int r{0}; r<numberOfCylindersPerLayer; r++)
    {
        for(int c{0}; c<numberOfXYLayers; c++)
        {
            int cylinderCount{r};
            int layerCount{c};
            double xLocation{0};
            double yLocation{0};
            double zLocation{0};
            double alongBottom{-mShapeHeight/2};
            double abovePreviousLayers{layerCount*mLayerHeight*2};
            double sizeAdjustForRadiusOfPrint = get_diameter_of_print()/2;
            zLocation = alongBottom+sizeAdjustForRadiusOfPrint+abovePreviousLayers;

            if (cylinderCount<numberOfXCylindersPerLayer)
            {
                double alongSide{mShapeWidth/2-sizeAdjustForRadiusOfPrint};
                double nextToPreviousCylinder{-cylinderCount*mExtrusionWidthCalculated};
                yLocation = alongSide+nextToPreviousCylinder;

            }
            else
            {
                int cylinderYCount = cylinderCount-numberOfXCylindersPerLayer;
                double alongSide{mShapeLength/2-sizeAdjustForRadiusOfPrint};
                double nextToPreviousCylinder{-cylinderYCount*mExtrusionWidthCalculated};
                xLocation = alongSide+nextToPreviousCylinder;
                double aboveXLayer{mLayerHeight};
                zLocation += aboveXLayer;
            }
            centerOfCylinderArray[r][c][1] = xLocation;
            centerOfCylinderArray[r][c][2] = yLocation;
            centerOfCylinderArray[r][c][3] = zLocation;
        }
    }
    return centerOfCylinderArray;
}

float PrintShape::get_diameter_of_print()
{
    double infillRatio = mInfillPercentage/100;
    double volumePrintPerLayer = mShapeWidth*mShapeLength*mLayerHeight*infillRatio*mExtrusionMultiplier;
    double volumeSyringePerLayer = volumePrintPerLayer;
    double diameterOfSyringe{14.9};
    double areaSyringe = pi/4*diameterOfSyringe*diameterOfSyringe;
    double heightSyringePerLayerCalculated = volumeSyringePerLayer/areaSyringe;
    mExtrusionWidthCalculated = mExtrusionWidth/infillRatio;
    float diameterOfPrint = sqrt(volumePrintPerLayer*4*mExtrusionWidthCalculated/(mShapeWidth*mShapeLength*pi));
    return diameterOfPrint;
}

