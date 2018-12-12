#ifndef WIREFRAME_H
#define WIREFRAME_H
#include <osg/Node>
#include <QVector4D>
#include "shape.h"

class Wireframe
{
public:
    Wireframe();
    ~Wireframe();
    osg::Node* create_wireframe(osg::Vec4 &color, osg::Vec3d &scaleFactor);
    osg::Node* draw_print_path(Shape *shape);
    osg::Vec3Array* get_vertex_data(Shape* shape);

protected:

private:
    Point mLastPoint{0,0,0};

};


#endif
