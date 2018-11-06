//#include "vector3d.h"
#include "osgwidget.h"
#include "sphereupdatecallback.h"
//#include "physicsworld.h"

SphereUpdateCallback::SphereUpdateCallback(bool &update, float radius) :
    mUpdate {update}
{
//    mPosition = sphere->get_position();
//    mVelocity = sphere->get_velocity();
//    mAcceleration = sphere->get_acceleration();
//    mObject = new PhysicsObject(mPosition, mVelocity, mAcceleration, radius, mass, cOfR, fluidDensity);

//    mObjectList->push_back(mObject);
}

void SphereUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
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

//osg::Vec3d SphereUpdateCallback::convert_Vector3_to_osgVec3d(Vector3 shapeLocation)
//{
//    osg::Vec3d shapeLocationVec3d{0,0,0};
//    shapeLocationVec3d.x() = shapeLocation.get_x();
//    shapeLocationVec3d.y() = shapeLocation.get_y();
//    shapeLocationVec3d.z() = shapeLocation.get_z();
//    return shapeLocationVec3d;
//}

//Vector3 SphereUpdateCallback::convert_osgVec3d_to_Vector3(osg::Vec3d shapeLocation)
//{
//    Vector3 shapeLocationVector3{0,0,0};
//    shapeLocationVector3.set_x(shapeLocation.x());
//    shapeLocationVector3.set_y(shapeLocation.y());
//    shapeLocationVector3.set_z(shapeLocation.z());
//    return shapeLocationVector3;
//}

double SphereUpdateCallback::get_timestep() const
{
    return mTimeStep;
}
