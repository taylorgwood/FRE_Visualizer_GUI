#ifndef PRINTSHAPE_H
#define PRINTSHAPE_H

#include <osg/MatrixTransform>
#include <osg/Matrix>

class PrintShape
{
public:
    ~PrintShape();
    PrintShape();

    float  get_diameter_of_print() const;
    void   set_needle_diameter(const double needleDiameter);
    double get_needle_diameter() const;
    void   set_extrusion_multiplier(const double extrusionMultiplier);
    double get_extrusion_multiplier() const;
    void   set_infill_percentage(const double infillPercentage);
    double get_infill_percentage() const;
    void   set_extrusion_width(const double extrusionWidth);
    double get_extrusion_width() const;
    void   set_layer_height(const float layerHeight);
    float  get_layer_height() const;
    void   set_diameter_of_print(const double diameterOfPrint);
    void   calculate_layer_properties();
    void   set_default_parameters();
    void   set_print_parameters(double dP, double nD, double eM, double iF, double eW, double lH);
    int    calculate_number_of_layers();
    int    calculate_number_of_cylinders_per_X_layer();
//    int    calculate_number_of_X_cylinders();
    int    calculate_number_of_cylinders_per_Y_layer();
//    int    calculate_number_of_Y_cylinders();
    double ***create_center_of_X_cylinder_array();
    double ***create_center_of_Y_cylinder_array();
    void   create_all_cylinders(double ***centerOfCylinderArray, double numberOfCylindersPerLayer, double numberOfLayers);
    void   set_shape_size(const double objectWidth, const double objectLength, const double objectHeight);
    float  get_shape_width() const;
    float  get_shape_length() const;
    float  get_shape_height() const;


private:


protected:
    double pi{3.14159}; const
    double mDiameterOfSyringe{14.9};
    float  mDiameterOfPrint{0.26f};
    double mNeedleDiameter{0.26};
    double mExtrusionMultiplier{1.0};
    double mInfillPercentage{100};
    double mExtrusionWidth{0.26};
    double mExtrusionWidthCalculated{mExtrusionWidth};
    float  mLayerHeight{0.26f};
    float  mShapeWidth{10};
    float  mShapeHeight{10};
    float  mShapeLength{10};
//    bool&  mUpdate;
    std::vector<PrintShape*> *mShapeList {nullptr};
    //    std::vector<PhysicsObject*> *mObjectList {nullptr};

};

#endif // PRINTSHAPE_H
