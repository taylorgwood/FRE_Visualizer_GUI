#include "wireframe.h"
#include <osg/Geometry>
#include <osg/Material>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>

Wireframe::Wireframe()
{
}

Wireframe::~Wireframe()
{
}

osg::Node* Wireframe::create_wireframe(osg::Vec4 &color, osg::Vec3d &scaleFactor)
{
    osg::Vec3Array* v = new osg::Vec3Array;
    v->resize( 8 );
    (*v)[0].set( -1.f, -1.f, -1.f );
    (*v)[1].set(  1.f, -1.f, -1.f );
    (*v)[2].set(  1.f,  1.f, -1.f );
    (*v)[3].set( -1.f,  1.f, -1.f );
    (*v)[4].set( -1.f, -1.f,  1.f );
    (*v)[5].set(  1.f, -1.f,  1.f );
    (*v)[6].set(  1.f,  1.f,  1.f );
    (*v)[7].set( -1.f,  1.f,  1.f );

    osg::Geometry* geom = new osg::Geometry;
    geom->setUseDisplayList( false );
    geom->setVertexArray( v );

    osg::Vec4Array* c = new osg::Vec4Array;
    c->push_back(color);
    geom->setColorArray( c, osg::Array::BIND_OVERALL );

    GLushort idxLines[8] = {0, 4, 1, 5, 2, 6, 3, 7};
    GLushort idxLoop1[4] = {0, 1, 2, 3};
    GLushort idxLoop2[4] = {4, 5, 6, 7};
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINES, 8, idxLines ) );
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoop1 ) );
    geom->addPrimitiveSet( new osg::DrawElementsUShort( osg::PrimitiveSet::LINE_LOOP, 4, idxLoop2 ) );

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( geom );

    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform;
    transform->setScale(scaleFactor);

    transform->addChild(geode);
    return transform;
}

osg::Node* Wireframe::draw_print_path(Shape* shape)
{
    osg::Vec4Array* color = get_color_data(shape);
    osg::Geometry* linesGeom = new osg::Geometry;
    osg::DrawArrays* drawArrayLines = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
    linesGeom->addPrimitiveSet(drawArrayLines);
    osg::Vec3Array* vertexData = get_vertex_data(shape);
    linesGeom->setVertexArray(vertexData);

    drawArrayLines->setFirst(0);
    drawArrayLines->setCount(vertexData->size());

    linesGeom->setColorArray(color, osg::Array::BIND_PER_VERTEX);

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(linesGeom);

    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform;
    osg::Vec3d scaleFactor{1,1,1};
    transform->setScale(scaleFactor);

    transform->addChild(geode);
    return transform;
}

osg::Vec3Array* Wireframe::get_vertex_data(Shape* shape)
{
    double shapeHeight = shape->get_height();
    double shapeWidth  = shape->get_width();
    double shapeLength = shape->get_length();

    std::vector<Point> shapePointList = shape->get_points();
    size_t totalNumberOfPoints = shapePointList.size();
    osg::Vec3Array* vertexData = new osg::Vec3Array;

    for (int i{0}; i< totalNumberOfPoints; i++)
    {
        Point point = shapePointList[i];
        float xLocation = point.get_x()-shapeLength/2;
        float yLocation = point.get_y()-shapeWidth/2;
        float zLocation = point.get_z()-shapeHeight/2;
        vertexData->push_back(osg::Vec3(xLocation,yLocation,zLocation));
    }
    return vertexData;
}

osg::Vec4Array* Wireframe::get_color_data(Shape* shape)
{
    std::vector<Point> shapePointList = shape->get_points();
    size_t totalNumberOfPoints = shapePointList.size();
    osg::Vec4Array* color = new osg::Vec4Array;
    for (int i{0}; i< totalNumberOfPoints; i++)
    {
        Point point = shapePointList[i];
        float material = point.get_material();
        float R = material;
        float G = material;
        float B = material;
        float A = 1.0;
        osg::Vec4 currentColor{R,G,B,A};
        color->push_back(currentColor);
    }
    return color;
}
