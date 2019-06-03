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
    transform->setPosition(osg::Vec3d(0,0,0));

    transform->addChild(geode);
    return transform;
}

osg::Node* Wireframe::draw_print_path(osg::Vec4Array* color, osg::Vec3Array* vertexData, int animationCount)
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
    transform->addChild(animatedGeode);
    return transform;
}
