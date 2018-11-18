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
    double extrusionMultiplier{1.0};
    double infillPercentage{100};
    double extrusionWidth{0.26};
    double layerHeight{0.26};
    set_print_parameters(needleDiameter, extrusionMultiplier, infillPercentage, extrusionWidth, layerHeight);
}

void PrintShape::set_print_parameters(double needleDiameter, double extrusionMultiplier, double infillPercentage, double extrusionWidth, double layerHeight)
{
    set_needle_diameter(needleDiameter);
    set_extrusion_multiplier(extrusionMultiplier);
    set_infill_percentage(infillPercentage);
    set_extrusion_width(extrusionWidth);
    set_layer_height(layerHeight);
}

int PrintShape::get_number_of_layers()
{
    int numberOfLayers = floor(mShapeHeight/mLayerHeight);
    return numberOfLayers;
}

int PrintShape::get_number_of_XYlayers()
{
    int numberOfXYLayers = floor(mShapeHeight/mLayerHeight/2);
    return numberOfXYLayers;
}

int PrintShape::get_number_of_cylinders_per_X_layer()
{
    double extrusionWidthModified = calculate_modified_extrusion_width();
    int numberOfXCylindersPerLayer = floor(mShapeWidth/extrusionWidthModified);
    return numberOfXCylindersPerLayer;
}

int PrintShape::get_number_of_cylinders_per_Y_layer()
{
    double extrusionWidthModified = calculate_modified_extrusion_width();
    int numberOfYCylindersPerLayer = floor(mShapeLength/extrusionWidthModified);
    return numberOfYCylindersPerLayer;
}

double*** PrintShape::create_point_array()
{
    double*** emptyPointArray = create_empty_point_array();
    double*** pointArray = fill_point_array(emptyPointArray);
    return pointArray;
}

double*** PrintShape::create_center_of_cylinder_array()
{
    double*** emptyCylinderArray = create_empty_cylinder_array();
    double*** cylinderArray = fill_cylinder_array(emptyCylinderArray);
    return cylinderArray;
}

double*** PrintShape::create_empty_point_array()
{
    int numberOfPointsPerXLayer = get_number_of_cylinders_per_X_layer()*2;
    int numberOfPointsPerYLayer = get_number_of_cylinders_per_Y_layer()*2;
    int numberOfPointsPerXYLayer = numberOfPointsPerXLayer+numberOfPointsPerYLayer;
    int numberOfXYLayers = get_number_of_XYlayers();
    int numberOfOrthogonalDirections{3};

    double ***pointArray{nullptr};
    pointArray = new double**[numberOfPointsPerXYLayer];
    for (int r{0}; r < numberOfPointsPerXYLayer; r++)
    {
        pointArray[r] = new double*[numberOfXYLayers];
        for (int c{0}; c<numberOfXYLayers; c++)
        {
            pointArray[r][c] = new double[numberOfOrthogonalDirections];
        }
    }
    return pointArray;
}

double*** PrintShape::create_empty_cylinder_array()
{
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
    return centerOfCylinderArray;
}

double*** PrintShape::fill_point_array(double*** &pointArray)
{
    int numberOfPointsPerXLayer = get_number_of_cylinders_per_X_layer()*2;
    int numberOfPointsPerYLayer = get_number_of_cylinders_per_Y_layer()*2;
    int numberOfPointsPerXYLayer = numberOfPointsPerXLayer+numberOfPointsPerYLayer;
    int numberOfXYLayers = get_number_of_XYlayers();

    float diameterOfPrint = get_diameter_of_print();
    double radiusOfPrint = diameterOfPrint/2;

    double xLocation{radiusOfPrint};
    double yLocation{radiusOfPrint};
    double zLocation{radiusOfPrint};

    for (int r{0}; r<numberOfPointsPerXYLayer; r++)
    {
        for(int c{0}; c<numberOfXYLayers; c++)
        {
//            int pointCount{r};
//            int layerCount{c};
//            double extrusionWidthModified = calculate_modified_extrusion_width();
//            zLocation = radiusOfPrint+(layerCount*mLayerHeight*2);
//            bool switchback{true};

//            if (pointCount<numberOfPointsPerXLayer)
//            {
//                if (switchback == true)
//                {
//                    xLocation = 0;
//                    yLocation = radiusOfPrint+pointCount*extrusionWidthModified;
//                }
//                else
//                {
//                    xLocation = mShapeLength;
//                    yLocation = radiusOfPrint+pointCount*extrusionWidthModified;
//                }
//                switchback = !switchback;

//            }
//            else
//            {
//                int cylinderYCount = pointCount-numberOfXCylindersPerLayer;
//                double alongSide{mShapeLength/2-sizeAdjustForRadiusOfPrint};
//                double nextToPreviousCylinder{-cylinderYCount*extrusionWidthModified};
//                xLocation = alongSide+nextToPreviousCylinder;
//                double aboveXLayer{mLayerHeight};
//                zLocation += aboveXLayer;
//            }
            pointArray[r][c][1] = xLocation;
            pointArray[r][c][2] = yLocation;
            pointArray[r][c][3] = zLocation;
        }
    }
    return pointArray;
}

double*** PrintShape::fill_cylinder_array(double*** &centerOfCylinderArray)
{
    int numberOfLayers = get_number_of_layers();
    int numberOfXCylindersPerLayer = get_number_of_cylinders_per_X_layer();
    int numberOfYCylindersPerLayer = get_number_of_cylinders_per_Y_layer();
    int numberOfCylindersPerLayer = numberOfXCylindersPerLayer+numberOfYCylindersPerLayer;
    int numberOfXYLayers = get_number_of_XYlayers();

    float diameterOfPrint = get_diameter_of_print();

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
            double extrusionWidthModified = calculate_modified_extrusion_width();

            if (cylinderCount<numberOfXCylindersPerLayer)
            {
                double alongSide{mShapeWidth/2-sizeAdjustForRadiusOfPrint};
                double nextToPreviousCylinder{-cylinderCount*extrusionWidthModified};
                yLocation = alongSide+nextToPreviousCylinder;

            }
            else
            {
                int cylinderYCount = cylinderCount-numberOfXCylindersPerLayer;
                double alongSide{mShapeLength/2-sizeAdjustForRadiusOfPrint};
                double nextToPreviousCylinder{-cylinderYCount*extrusionWidthModified};
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
    double volumePrintPerLayer = calculate_volume_per_layer();
    double modifiedExtrusionWidth = calculate_modified_extrusion_width();
    float diameterOfPrint = sqrt(volumePrintPerLayer*4*modifiedExtrusionWidth/(mShapeWidth*mShapeLength*pi));
    return diameterOfPrint;
}

double PrintShape::calculate_volume_per_layer()
{
    double infillRatio = mInfillPercentage/100;
    double volumePrintPerLayer = mShapeWidth*mShapeLength*mLayerHeight*infillRatio*mExtrusionMultiplier;
    return volumePrintPerLayer;
}

double PrintShape::calculate_modified_extrusion_width()
{
    double infillRatio = mInfillPercentage/100;
    double extrusionWidthModified = mExtrusionWidth/infillRatio;
    return extrusionWidthModified;
}

void PrintShape::toggle_auto_adjust(bool checked)
{
    mAutoAdjust = checked;
}

//void PrintShape::adjust_parameters()
//{
//    if (mAutoAdjust==true)
//    {
//        mExtrusionWidth = number
//    }
//}
