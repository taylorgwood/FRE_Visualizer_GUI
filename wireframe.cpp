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

osg::Node* Wireframe::create_line(Shape* shape)
{
    std::vector<Point> pointList = shape->get_points();
    size_t totalNumberOfPoints = pointList.size();


    osg::Vec4Array* c = new osg::Vec4Array;
    c->resize(totalNumberOfPoints);
    osg::Geometry* linesGeom = new osg::Geometry;
    osg::DrawArrays* drawArrayLines = new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP);
    linesGeom->addPrimitiveSet(drawArrayLines);
    osg::Vec3Array* vertexData = new osg::Vec3Array;
    linesGeom->setVertexArray(vertexData);

    int count{0};
    for (int i{0}; i< totalNumberOfPoints; i++)
    {
                        Point point = pointList[i];
                        float xLocation = point.get_x();
                        float yLocation = point.get_y();
                        float zLocation = point.get_z();
                        vertexData->push_back(osg::Vec3(xLocation,yLocation,zLocation));
                        float  R = 0.5f;
                        float  G = 0.5f;
                        float  B = point.get_material();
                        float  A = 0.5;
                        (*c)[count].set(R,G,B,A);
                        mLastPoint = point;
                        count ++;
    }
    //    std::vector<Layer*> layerList = shape->get_layer_list();
    //    int numberOfLayers = shape->get_number_of_layers();
//        int count{0};
    //    for (int i{0}; i< numberOfLayers; i++)
    //    {
    //        Layer* layer = layerList[i];
    //        std::vector<Path*> pathList = layer->get_path_list();
    //        int numberOfPaths = layer->get_number_of_paths();
    //        for (int j{0}; j<numberOfPaths; j++)
    //        {
    //            Path* path = pathList[j];
    //            std::vector<Point*> pointList = path->get_point_list();
    //            int numberOfPoints = path->get_number_of_points();
    //            for (int k{0}; k<numberOfPoints; k++)
    //            {
    //                Point* point = pointList[k];
    //                float xLocation = point->get_x();
    //                float yLocation = point->get_y();
    //                float zLocation = point->get_z();
    //                vertexData->push_back(osg::Vec3(xLocation,yLocation,zLocation));
    //                float  R = 0.5f;
    //                float  G = 0.5f;
    //                float  B = point->get_material();
    //                float  A = 0.5;
    //                (*c)[count].set(R,G,B,A);
    //                mLastPoint = point;
    //                count ++;
    //            }
    //        }
    //    }

    drawArrayLines->setFirst(0);
    drawArrayLines->setCount(totalNumberOfPoints);

    linesGeom->setColorArray(c, osg::Array::BIND_PER_VERTEX);

    osg::Geode* geode = new osg::Geode;
    geode->addDrawable( linesGeom );

    geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );
    geode->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    osg::PositionAttitudeTransform* transform = new osg::PositionAttitudeTransform;
    osg::Vec3d scaleFactor{1,1,1};
    transform->setScale(scaleFactor);

    transform->addChild(geode);
    return transform;
}
