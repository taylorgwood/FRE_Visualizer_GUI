#ifndef SHAPEUPDATECALLBACK_H
#define SHAPEUPDATECALLBACK_H

#include <osg/MatrixTransform>
#include <osg/Matrix>
class Vector3;
class PhysicsObject;

class ShapeUpdateCallback: public osg::NodeCallback
{
public:
    ShapeUpdateCallback(bool& update, float radius);
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

//    void update_physics();

//    osg::Vec3d convert_Vector3_to_osgVec3d(Vector3 shapeLocation);
//    Vector3 convert_osgVec3d_to_Vector3(osg::Vec3d shapeLocation);
    double get_timestep() const;
//    void box_collision(double boxSize, double coefficientOfRestitution);

private:


protected:
    double mTimeStep{1.0/30.0};
//    PhysicsObject *mObject;
//    Vector3 mPosition{0,0,0};
//    Vector3 mVelocity{0,0,0};
//    Vector3 mAcceleration{0,0,-9.8};
    bool& mUpdate;
//    std::vector<PhysicsObject*> *mObjectList {nullptr};

};

#endif // SHAPEUPDATECALLBACK_H
