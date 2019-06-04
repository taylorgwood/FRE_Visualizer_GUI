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
    osg::Node* create_wireframe(osg::Vec4 &color, float height, float width, float topWidth, float length);
    osg::Node* draw_print_path(osg::Vec4Array *color, osg::Vec3Array *vertexData, double shapeWidth, double shapeLength, double shapeHeight);

protected:

private:
    Point mLastPoint{0,0,0};

};


#endif
