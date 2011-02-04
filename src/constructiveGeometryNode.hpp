/*
 *  geometryNode.hpp
 *  A4
 *
 *  Nodes that are responsible for representing primitives in a scene.
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */


#ifndef CONSTRUCTIVE_GEOMETRY_NODE_HPP
#define CONSTRUCTIVE_GEOMETRY_NODE_HPP

#include "geometryNode.hpp"
#include "material.hpp"
#include "sceneNode.hpp"

typedef enum CSGOp{
   UNION = 0,
   INTERSECTION,
   DIFFERENCE
}CSGOp;
   

class ConstructiveGeometryNode 
   : public GeometryNode 
{
public:
   ConstructiveGeometryNode(const std::string& name, 
                            SceneNode *a,
                            SceneNode *b,
                            CSGOp operation,
                            Material *material = NULL);
   
   ConstructiveGeometryNode(const ConstructiveGeometryNode& otherNode);
   ConstructiveGeometryNode &operator=(const ConstructiveGeometryNode& otherNode);
   
   virtual ~ConstructiveGeometryNode();
   
   virtual void prepare(const Transform &wolrdToSuperNode);
   
   virtual size_t intersect(const Ray &r, 
                            NodeIntersection *nIntersectionNear,
                            double *tHitFar = NULL,
                            NodeIntersection *nIntersectionFar = NULL) const;
   virtual bool intersects(const Ray &r, double *transmissiveness = NULL) const;
   
private:
   SceneNode *mNodeA;
   SceneNode *mNodeB;
   
   CSGOp op;
};

#endif // CONSTRUCTIVE_GEOMETRY_NODE_HPP
