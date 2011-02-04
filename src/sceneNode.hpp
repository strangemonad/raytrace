/*
 *  sceneNode.hpp
 *  A4
 *
 *  Base class for nodes comprising a hierarchical scene description.
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */


#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <iostream>
#include <list>

#include "colour.hpp"
#include "material.hpp"
#include "transform.hpp"
#include "primitive.hpp"

class Vector;
class GeometryNode;

/*
 * This is a bit of a misnomer. Instersection structs will only pertain to
 * a gemoetry node.
 */
typedef struct NodeIntersection 
{
	NodeIntersection() { mat = NULL; }
   NodeIntersection(const NodeIntersection &otherNode);
	
   PrimitivePatch pPatch;
   
   const Material *mat;
   
   // will need more stuff for the project
} NodeIntersection;


class SceneNode 
{
public:
   SceneNode(const std::string& name);
   
   SceneNode(const SceneNode& otherNode);
   SceneNode& operator=(const SceneNode& otherNode);
   
   virtual ~SceneNode();
   
   const std::string& getName() const;
   void setName(const std::string& newName);
   
   virtual void prepare(const Transform &wolrdToSuperNode);

   virtual const BBox &nodeBounds() const;
   virtual const BBox &worldBounds() const;

   const Transform &getTransform() const;   
   void setTransform(const Transform& t);
   
   void addChild(SceneNode* child);
   void removeChild(SceneNode* child);
   
   void rotate(char axis, double angle);
   void scale(const Vector& amount);
   void translate(const Vector &amount);
   
   virtual bool isJoint() const;
   
   virtual size_t intersect(const Ray &r, 
                            NodeIntersection *nIntersectionNear,
                            double *tHitFar = NULL,
                            NodeIntersection *nIntersectionFar = NULL) const;
   virtual bool intersects(const Ray &r, double *transmissiveness = NULL) const; 

   
   /*
    * XXX need a second intersect method that doesn't calculate the geometry 
    * and lighting/shading info for efficiency when casting things like shadow 
    * rays.
    */
   
protected:
      
   // Useful for picking
   int mID;
   std::string mName;
   
   // XXX this needs to become a transform.
   
   /*
    * Relative transformation applied onto all children nodes.
    */
   Transform mTrans;
   
   /*
    * Cached values set during prepare().
    */
   Transform mWolrdToNodeTrans;
   BBox mNodeBounds;
   BBox mWorldBounds;
   
   // Hierarchy
   typedef std::list<SceneNode*> ChildList;
   ChildList mChildren;
};

#endif // SCENE_NODE_HPP
