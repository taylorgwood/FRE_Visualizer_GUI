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
    osg::Node* draw_print_path(Shape *shape, osg::Vec4Array *color, osg::Vec3Array *vertexData);

protected:

private:
    Point mLastPoint{0,0,0};

};


#endif
