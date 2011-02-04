/*
 *  scene.hpp
 *  A4
 *
 *  Contains a view independent representation of a scene. This can be thought
 *  as the data model of a scene to render.
 *
 *  View specifics of a particular render are specified by Camera and the size
 *  requested when triggering the render.
 *
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */

#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>

#include "prefs.hpp"
#include "ray.hpp"
#include "sceneNode.hpp"

class Colour;
class Light;
class SceneNode;

class Scene
{
public: 
   Scene(SceneNode& root, 
         std::list<Light*>& lights, 
         Colour ambientColour = Colour());
   
   Scene(const Scene& otherScene);
   Scene& operator =(const Scene& otherScene);
   
   Colour getAmbient() const;

   virtual ~Scene();
   
   void prepare();
   
   bool intersect(const Ray &r, NodeIntersection *nIntersection) const; 
   bool intersects(const Ray &r, double *transmissiveness = NULL) const; 
   
   Colour getColourAtIntersection(const Ray &rIncident,
                                  const NodeIntersection &ni,
                                  const Colour &cEnvironment,
                                  uint8_t rayRecursion = Prefs::sharedPrefs().rayRecursions) const;

private:
   SceneNode& mRoot;
   std::list<Light*>& mLights;
   Colour mAmbientColour;
   
   Vector computeReflection(const Vector &incident,
                              const PrimitivePatch &pPatch) const;

   
   Colour diffuseLight(const Point &p, const Normal &n) const;
   Colour specularLight(const Point &p,
                        const Normal &n,
                        const Vector &view,
                        double phongCoeff) const;
};

#endif // SCENE_HPP
