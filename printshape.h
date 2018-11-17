#ifndef PRINTSHAPE_H
#define PRINTSHAPE_H

#include <osg/MatrixTransform>
#include <osg/Matrix>

class PrintShape
{
public:
    ~PrintShape();
    PrintShape();

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
    float  get_diameter_of_print();
    double   calculate_volume_per_layer();
    void   set_default_parameters();
    void   set_print_parameters(double needleDiameter, double extrusionMultiplier, double infillPercentage, double extrusionWidth, double layerHeight);
    int    get_number_of_XYlayers();
    int    get_number_of_cylinders_per_X_layer();
    int    get_number_of_cylinders_per_Y_layer();
    double*** create_center_of_cylinder_array();
    double*** create_empty_cylinder_array();
    double*** fill_cylinder_array(double*** &centerOfCylinderArray);
//    void   create_all_cylinders(double*** &centerOfCylinderArray, double numberOfCylindersPerLayer, double numberOfLayers);
    void   set_shape_size(const double objectWidth, const double objectLength, const double objectHeight);
    float  get_shape_width() const;
    float  get_shape_length() const;
    float  get_shape_height() const;
    double calculate_modified_extrusion_width();
    void   toggle_auto_adjust(bool checked);

private:

protected:
    double const pi{3.14159};
    double mNeedleDiameter{0.26};
    double mExtrusionMultiplier{0.785};
    double mInfillPercentage{100};
    double mExtrusionWidth{0.26};
//    double mExtrusionWidthCalculated{mExtrusionWidth};
    float  mLayerHeight{0.26f};
    float  mShapeWidth{10};
    float  mShapeHeight{10};
    float  mShapeLength{10};
    std::vector<PrintShape*> *mShapeList {nullptr};
    bool   mAutoAdjust{false};

};

#endif // PRINTSHAPE_H
