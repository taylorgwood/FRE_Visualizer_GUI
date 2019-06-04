#include "wireframe.h"
#include <osg/Geometry>
#include <osg/Material>
#include <osg/Geode>
#include <osg/PositionAttitudeTransform>
#include <osg/ShapeDrawable>
#include <osg/BlendFunc>

Wireframe::Wireframe()
{
}

Wireframe::~Wireframe()
{
}

osg::Node* Wireframe::create_wireframe(osg::Vec4 &color, float height, float width, float topWidth, float length)
{
    osg::Vec3Array* v = new osg::Vec3Array;
    v->resize( 8 );
    (*v)[0].set( -width/2,    -length/2, -height/2 );
    (*v)[1].set(  width/2,    -length/2, -height/2 );
    (*v)[2].set(  width/2,     length/2, -height/2 );
    (*v)[3].set( -width/2,     length/2, -height/2 );
    (*v)[4].set( -width/2,    -length/2,  height/2 );
    (*v)[5].set(  width/2-(width-topWidth), -length/2,  height/2 );
    (*v)[6].set(  width/2-(width-topWidth),  length/2,  height/2 );
    (*v)[7].set( -width/2,     length/2,  height/2 );

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
    geode->addDrawable(geom);

//    osg::StateSet* stateSet = geode->getOrCreateStateSet();
//    stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
//    stateSet->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);
//    osg::Material* material = new osg::Material;
//    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
//    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
//    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
//    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
//    geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );

    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform;
//    transform->setScale(scaleFactor);
    transform->setPosition(osg::Vec3d(0,0,0));

    transform->addChild(geode);
    return transform;
}

osg::Node* Wireframe::draw_print_path(osg::Vec4Array* color, osg::Vec3Array* vertexData, double shapeWidth, double shapeLength, double shapeHeight)
{
    osg::Geometry* animatedLinesGeom = new osg::Geometry;
    osg::DrawArrays* drawAnimatedArrayLines = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
    animatedLinesGeom->addPrimitiveSet(drawAnimatedArrayLines);
    animatedLinesGeom->setVertexArray(vertexData);
    drawAnimatedArrayLines->setFirst(0);
    int numberOfPoints = static_cast<int>(vertexData->size());
    drawAnimatedArrayLines->setCount(numberOfPoints);

    animatedLinesGeom->setColorArray(color, osg::Array::BIND_PER_VERTEX);

    osg::Geode* animatedGeode = new osg::Geode;
    animatedGeode->addDrawable(animatedLinesGeom);;

    osg::StateSet* stateSet = animatedGeode->getOrCreateStateSet();
    stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    stateSet->setAttributeAndModes(new osg::BlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);

    osg::Material* material = new osg::Material;
    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );

    animatedGeode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    animatedGeode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform;
//    transform->setPosition(osg::Vec3d(0,0,0));
    transform->addChild(animatedGeode);
    return transform;
}
