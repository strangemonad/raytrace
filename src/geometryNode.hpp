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


#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include "material.hpp"
#include "sceneNode.hpp"

class Primitive;


class GeometryNode 
   : public SceneNode 
{
public:
   GeometryNode(const std::string& name, 
                Primitive *primitive = NULL,
                Material *material = NULL);
   
   GeometryNode(const GeometryNode& otherNode);
   GeometryNode& operator =(const GeometryNode& otherNode);
   
   virtual ~GeometryNode();
   
   virtual void prepare(const Transform &wolrdToSuperNode);
   
   const Material *getMaterial() const;
   void setMaterial(Material *material);
   
   virtual size_t intersect(const Ray &r, 
                            NodeIntersection *nIntersectionNear,
                            double *tHitFar = NULL,
                            NodeIntersection *nIntersectionFar = NULL) const;
   virtual bool intersects(const Ray &r, double *transmissiveness = NULL) const;
   
private:
   Primitive *mPrimitive;
   
protected:
   Material *mMaterial;
};

#endif // GEOMETRY_NODE_HPP
