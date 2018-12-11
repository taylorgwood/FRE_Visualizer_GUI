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

//void Wireframe::set_object_size(const double objectWidth, const double objectLength, const double objectHeight)
//{
//    mShapeWidth  = objectWidth;
//    mShapeLength = objectLength;
//    mShapeHeight = objectHeight;
//    //    update();
//}

//float Wireframe::get_shape_width() const
//{
//    return mShapeWidth;
//}

//float Wireframe::get_shape_length() const
//{
//    return mShapeLength;
//}

//float Wireframe::get_shape_height() const
//{
//    return mShapeHeight;
//}
