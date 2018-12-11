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
    osg::Node* create_line(Shape *shape);

protected:

private:

};


#endif
