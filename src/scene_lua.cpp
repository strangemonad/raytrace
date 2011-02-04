//
// CS488 - Introduction to Computer Graphics
//
// scene_lua.cpp
//
// Everything that's needed to parse a scene file using Lua.
// You don't necessarily have to understand exactly everything that
// goes on here, although it will be useful to have a reasonable idea
// if you wish to add new commands to the scene format.
//
// Lua interfaces with C/C++ using a special stack. Everytime you want
// to get something from lua, or pass something back to lua (e.g. a
// return value), you need to use this stack. Thus, most of the lua_
// and luaL_ functions actually manipulate the stack. All the
// functions beginning with "lua_" are part of the Lua C API itself,
// whereas the "luaL_" functions belong to a library of useful
// functions on top of that called lauxlib.
//
// This file consists of a bunch of C function declarations which
// implement functions callable from Lua. There are also two tables
// used to set up the interface between Lua and these functions, and
// the main "driver" function, import_lua, which calls the lua
// interpreter and sets up all the state.
//
// Note that each of the function declarations follow the same format:
// they take as their only argument the current state of the lua
// interpreter, and return the number of values returned back to lua.
//
// For more information see the book "Programming In Lua," available
// online at http://www.lua.org/pil/, and of course the Lua reference
// manual at http://www.lua.org/manual/5.0/.
//
// http://lua-users.org/wiki/LauxLibDocumentation provides a useful
// documentation of the "lauxlib" functions (beginning with luaL_).
//
// -- University of Waterloo Computer Graphics Lab 2005

#include <iostream>
#include <cctype>
#include <cstring>
#include <string>
#include <cstdio>
#include <vector>

#include "cone.hpp"
#include "constructiveGeometryNode.hpp"
#include "cube.hpp"
#include "cylinder.hpp"
#include "defs.hpp"
#include "geometryNode.hpp"
#include "image.hpp"
#include "light.hpp"
#include "lua488.hpp"
#include "material.hpp"
#include "paraboloid.hpp"
#include "perspectiveCamera.hpp"
#include "point.hpp"
#include "primitive.hpp"
#include "prefs.hpp"
#include "rayTracer.hpp"
#include "scene.hpp"
#include "scene_lua.hpp"
#include "sceneNode.hpp"
#include "sphere.hpp"
#include "stats.hpp"
#include "texture.hpp"
#include "vector.hpp"


/*
 * Types
 */

// You may wonder, for the following types, why we use special "_ud"
// types instead of, for example, just allocating SceneNodes directly
// from lua. Part of the answer is that Lua is a C api. It doesn't
// call any constructors or destructors for you, so it's easier if we
// let it just allocate a pointer for the node, and handle
// allocation/deallocation of the node itself. Another (perhaps more
// important) reason is that we will want SceneNodes to stick around
// even after lua is done with them, after all, we want to pass them
// back to the program. If we let Lua allocate SceneNodes directly,
// we'd lose them all when we are done parsing the script. This way,
// we can easily keep around the data, all we lose is the extra
// pointers to it.

/*
 * The "userdata" type for a node. Objects of this type will be
 * allocated by Lua to represent nodes.
 */
struct gr_node_ud
{
   SceneNode* node;
};


/*
 * The "userdata" type for a texture. Objects of this type will be
 * allocated by Lua to represent texture.
 */
struct gr_texture_ud 
{
   Texture* texture;
};


/*
 * The "userdata" type for a material. Objects of this type will be
 * allocated by Lua to represent materials.
 */
struct gr_material_ud 
{
   Material* material;
};

/*
 * The "userdata" type for a light. Objects of this type will be
 * allocated by Lua to represent lights.
 */
struct gr_light_ud 
{
   Light* light;
};

/*
 * Useful function to retrieve and check an n-tuple of numbers.
 */
template<typename T>
void get_tuple(lua_State* L, int arg, T* data, int n)
{
   luaL_checktype(L, arg, LUA_TTABLE);
   luaL_argcheck(L, luaL_getn(L, arg) == n, arg, "N-tuple expected");
   for (int i = 1; i <= n; i++) {
      lua_rawgeti(L, arg, i);
      data[i - 1] = luaL_checknumber(L, -1);
      lua_pop(L, 1);
   }
}


/*
 * LUA API functions
 */

// Create a node
extern "C"
int gr_node_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;
   
   const char* name = luaL_checkstring(L, 1);
   data->node = new SceneNode(name);
   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}


// Create a sphere node
extern "C"
int gr_sphere_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;
   
   const char* name = luaL_checkstring(L, 1);
   data->node = new GeometryNode(name, new Sphere());
   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}


// gr.cylinder('name', {xpos, ypos, zpos}, radius, length)
extern "C"
int gr_cylinder_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;
   
   const char* name = luaL_checkstring(L, 1);
   
   double center[3];
   get_tuple(L, 2, center, 3);
   
   double radius = luaL_checknumber(L, 3);
   double length = luaL_checknumber(L, 4);
   data->node = new GeometryNode(name, new Cylinder(radius, length));
   data->node->translate(Vector(center[0], center[1], center[2]));
   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}

// gr.cone('name', {xpos, ypos, zpos}, radius, length)
extern "C"
int gr_cone_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;
   
   const char* name = luaL_checkstring(L, 1);
   
   double center[3];
   get_tuple(L, 2, center, 3);
   
   double radius = luaL_checknumber(L, 3);
   double length = luaL_checknumber(L, 4);
   data->node = new GeometryNode(name, new Cone(radius, length));
   data->node->translate(Vector(center[0], center[1], center[2]));
   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}


// gr.paraboloid('name', {xpos, ypos, zpos}, radius, length)
extern "C"
int gr_paraboloid_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;
   
   const char* name = luaL_checkstring(L, 1);
   
   double center[3];
   get_tuple(L, 2, center, 3);
   
   double radius = luaL_checknumber(L, 3);
   double length = luaL_checknumber(L, 4);
   data->node = new GeometryNode(name, new Paraboloid(radius, length));
   data->node->translate(Vector(center[0], center[1], center[2]));
   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}


// Create a cube node
extern "C"
int gr_cube_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;
   
   const char* name = luaL_checkstring(L, 1);
   data->node = new GeometryNode(name, new Cube());
   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}

// Create a non-hierarchical sphere node
extern "C"
int gr_nh_sphere_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;
   
   const char* name = luaL_checkstring(L, 1);
   
   double center[3];
   get_tuple(L, 2, center, 3);
   
   double radius = luaL_checknumber(L, 3);
   data->node = new GeometryNode(name, new Sphere(radius));
   data->node->translate(Vector(center[0], center[1], center[2]));
   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}

// Create a non-hierarchical box node
extern "C"
int gr_nh_box_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;
   
   const char* name = luaL_checkstring(L, 1);
   
   Point pos;
   get_tuple(L, 2, &pos[0], 3);
   
   double size = luaL_checknumber(L, 3);
   
   data->node = new GeometryNode(name, new Cube());
   data->node->scale(Vector(size, size, size));
   data->node->translate(Vector(pos[0], pos[1], pos[2]));

   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}

// Create a polygonal mesh node
/*extern "C"
int gr_mesh_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;
   
   const char* name = luaL_checkstring(L, 1);
   
   std::vector<Point> verts;
   std::vector< std::vector<int> > faces;
   
   luaL_checktype(L, 2, LUA_TTABLE);
   int vert_count = luaL_getn(L, 2);
   
   luaL_argcheck(L, vert_count >= 1, 2, "Tuple of vertices expected");
   
   for (int i = 1; i <= vert_count; i++) {
      lua_rawgeti(L, 2, i);
      
      Point vertex;
      get_tuple(L, -1, &vertex[0], 3);
      
      verts.push_back(vertex);
      lua_pop(L, 1);
   }
   
   luaL_checktype(L, 3, LUA_TTABLE);
   int face_count = luaL_getn(L, 3);
   
   luaL_argcheck(L, face_count >= 1, 3, "Tuple of faces expected");
   
   faces.resize(face_count);
   
   for (int i = 1; i <= face_count; i++) {
      lua_rawgeti(L, 3, i);
      
      luaL_checktype(L, -1, LUA_TTABLE);
      int index_count = luaL_getn(L, -1);uint8_t
      
      luaL_argcheck(L, index_count >= 3, 3, "Tuple of indices expected");
      
      faces[i - 1].resize(index_count);
      get_tuple(L, -1, &faces[i - 1][0], index_count);
      
      lua_pop(L, 1);
   }
   
   Mesh* mesh = new Mesh(verts, faces);
   GRLUA_DEBUG(*mesh);
   data->node = new GeometryNode(*(new std::string(name)), mesh);
   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}*/

// Make a point light
extern "C"
int gr_light_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_light_ud* data = (gr_light_ud*)lua_newuserdata(L, sizeof(gr_light_ud));
   data->light = 0;
   
   double pos[3];
   double col[3];
   double falloff[3];
   
   get_tuple(L, 1, pos, 3);
   get_tuple(L, 2, col, 3);
   get_tuple(L, 3, falloff, 3);
   
   AttentuationCoefficients att;
   att.quadraticAttenuation = falloff[0];
   att.linearAttenuation = falloff[1];
   att.constantAttenuation = falloff[3];
   
   
   data->light = new Light(*(new Colour(col[0], col[1], col[2])),
                           *(new Point(pos[0], pos[1], pos[2])),
                           att);
   
   luaL_newmetatable(L, "gr.light");
   lua_setmetatable(L, -2);
   
   return 1;
}


// Create a texture.
extern "C"
int gr_uv_texture_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_texture_ud* data = (gr_texture_ud*)lua_newuserdata(L, sizeof(gr_texture_ud));
   data->texture = 0;  
   
   data->texture = new UVTexture();
   
   luaL_newmetatable(L, "gr.texture");
   lua_setmetatable(L, -2);
   
   return 1;
}

extern "C"
int gr_constant_texture_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_texture_ud* data = (gr_texture_ud*)lua_newuserdata(L, sizeof(gr_texture_ud));
   data->texture = 0;
   
   double kD[3];
   get_tuple(L, 1, kD, 3);   
   
   data->texture = new ConstantTexture(Colour(kD[0], kD[1], kD[2]));
   
   luaL_newmetatable(L, "gr.texture");
   lua_setmetatable(L, -2);
   
   return 1;
}

extern "C"
int gr_checker_texture_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_texture_ud* data = (gr_texture_ud*)lua_newuserdata(L, sizeof(gr_texture_ud));
   data->texture = 0;
   
   gr_texture_ud* tData = (gr_texture_ud*)luaL_checkudata(L, 1, "gr.texture");
   luaL_argcheck(L, tData != 0, 1, "Texture expected");
   Texture *tADat = dynamic_cast<Texture *>(tData->texture);
   ASSERT(tADat);  
   
   tData = (gr_texture_ud*)luaL_checkudata(L, 2, "gr.texture");
   luaL_argcheck(L, tData != 0, 1, "Texture expected");
   Texture *tBDat = dynamic_cast<Texture *>(tData->texture);
   ASSERT(tBDat); 
   
   uint8_t frequency = luaL_checknumber(L, 3);

   data->texture = new CheckerTexture(tADat, tBDat, frequency);
   
   luaL_newmetatable(L, "gr.texture");
   lua_setmetatable(L, -2);
   
   return 1;
}

extern "C"
int gr_image_texture_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_texture_ud* data = (gr_texture_ud*)lua_newuserdata(L, sizeof(gr_texture_ud));
   data->texture = 0;
   
   const char* filename = luaL_checkstring(L, 1);
   Image *img = new Image();
   img->loadPng(std::string(filename));
   
   data->texture = new ImageTexture(img);
   
   luaL_newmetatable(L, "gr.texture");
   lua_setmetatable(L, -2);
   
   return 1;
}


// Create a material
extern "C"
int gr_material_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_material_ud* data = (gr_material_ud*)lua_newuserdata(L, sizeof(gr_material_ud));
   data->material = 0;
   
   gr_texture_ud* tData = (gr_texture_ud*)luaL_checkudata(L, 1, "gr.texture");
   luaL_argcheck(L, tData != 0, 1, "Texture expected");
   Texture *tD = dynamic_cast<Texture *>(tData->texture);
   ASSERT(tD);
   
   double kS[3];
   get_tuple(L, 2, kS, 3);
   
   double shininess = luaL_checknumber(L, 3);
   
   data->material = new RTMaterial(tD,
                                   Colour(kS[0], kS[1], kS[2]),
                                   shininess);
      
   luaL_newmetatable(L, "gr.material");
   lua_setmetatable(L, -2);
   
   return 1;
}


// Create a reflective transmissive material
extern "C"
int gr_rt_material_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_material_ud* data = (gr_material_ud*)lua_newuserdata(L, sizeof(gr_material_ud));
   data->material = 0;
   
   gr_texture_ud* aData = (gr_texture_ud*)luaL_checkudata(L, 1, "gr.texture");
   luaL_argcheck(L, aData != 0, 1, "Node expected");
   Texture *tD = dynamic_cast<Texture *>(aData->texture);
   ASSERT(tD);
   
   double kS[3];
   get_tuple(L, 2, kS, 3);
   
   double shininess = luaL_checknumber(L, 3);
   double reflectivity = min(max(luaL_checknumber(L, 4), 0.0), 1.0);
   double opacity = min(max(luaL_checknumber(L, 5), 0.0), 1.0);
   double indexOfRefraction = max(luaL_checknumber(L, 6), 0.0);

   
   data->material = new RTMaterial(tD,
                                   Colour(kS[0], kS[1], kS[2]),
                                   shininess,
                                   reflectivity,
                                   opacity,
                                   indexOfRefraction);
   
   luaL_newmetatable(L, "gr.material");
   lua_setmetatable(L, -2);
   
   return 1;
}


// Set a material's bump map. Should be a member funct instead call it as
// gr.setBumMap(material, texture)
extern "C"
int gr_material_set_bump_map_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_material_ud *selfData = (gr_material_ud*)luaL_checkudata(L, 1, "gr.material");
   luaL_argcheck(L, selfData != 0, 1, "Material expected");
   Material *self = dynamic_cast<Material*>(selfData->material);
   ASSERT(self);
      
   gr_texture_ud *textData = (gr_texture_ud*)luaL_checkudata(L, 2, "gr.texture");
   luaL_argcheck(L, textData != 0, 2, "Texture expected");
   Texture *t = dynamic_cast<Texture*>(textData->texture);
   ASSERT(t);
   
   self->setBumpMap(t);
   
   return 0;
}


// Add a child to a node
extern "C"
int gr_node_add_child_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* selfData = (gr_node_ud*)luaL_checkudata(L, 1, "gr.node");
   luaL_argcheck(L, selfData != 0, 1, "Node expected");
   SceneNode *self = dynamic_cast<SceneNode*>(selfData->node);
   ASSERT(self);
   
   gr_node_ud* childdata = (gr_node_ud*)luaL_checkudata(L, 2, "gr.node");
   luaL_argcheck(L, childdata != 0, 2, "Node expected");
   
   SceneNode* child = childdata->node;
   
   self->addChild(child);
   
   return 0;
}

// Set a node's material
extern "C"
int gr_node_set_material_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* selfdata = (gr_node_ud*)luaL_checkudata(L, 1, "gr.node");
   luaL_argcheck(L, selfdata != 0, 1, "Node expected");
   GeometryNode* self = dynamic_cast<GeometryNode*>(selfdata->node);
   ASSERT(self);
   
   luaL_argcheck(L, self != 0, 1, "Geometry node expected");
   
   gr_material_ud* matdata = (gr_material_ud*)luaL_checkudata(L, 2, "gr.material");
   luaL_argcheck(L, matdata != 0, 2, "Material expected");
   
   Material* material = matdata->material;
   
   self->setMaterial(material);
   
   return 0;
}

// Add a scaling transformation to a node.
extern "C"
int gr_node_scale_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* selfdata = (gr_node_ud*)luaL_checkudata(L, 1, "gr.node");
   luaL_argcheck(L, selfdata != 0, 1, "Node expected");
   
   SceneNode* self = selfdata->node;
   
   double values[3];
   
   for (int i = 0; i < 3; i++) {
      values[i] = luaL_checknumber(L, i + 2);
   }
   
   self->scale(Vector(values[0], values[1], values[2]));
   
   return 0;
}

// Add a translation to a node.
extern "C"
int gr_node_translate_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* selfdata = (gr_node_ud*)luaL_checkudata(L, 1, "gr.node");
   luaL_argcheck(L, selfdata != 0, 1, "Node expected");
   
   SceneNode* self = selfdata->node;
   
   double values[3];
   
   for (int i = 0; i < 3; i++) {
      values[i] = luaL_checknumber(L, i + 2);
   }
   
   self->translate(Vector(values[0], values[1], values[2]));
   
   return 0;
}

// Rotate a node.
extern "C"
int gr_node_rotate_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* selfdata = (gr_node_ud*)luaL_checkudata(L, 1, "gr.node");
   luaL_argcheck(L, selfdata != 0, 1, "Node expected");
   
   SceneNode* self = selfdata->node;
   
   const char* axis_string = luaL_checkstring(L, 2);
   
   luaL_argcheck(L, axis_string
                 && std::strlen(axis_string) == 1, 2, "Single character expected");
   char axis = std::tolower(axis_string[0]);
   
   luaL_argcheck(L, axis >= 'x' && axis <= 'z', 2, "Axis must be x, y or z");
   
   double angle = luaL_checknumber(L, 3);
   
   self->rotate(axis, angle);
   
   return 0;
}

// Garbage collection function for lua.
extern "C"
int gr_node_gc_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* data = (gr_node_ud*)luaL_checkudata(L, 1, "gr.node");
   luaL_argcheck(L, data != 0, 1, "Node expected");
   
   // Note that we don't delete the node here. This is because we still
   // want the scene to be around when we close the lua interpreter,
   // but at that point everything will be garbage collected.
   //
   // If data->node happened to be a reference-counting pointer, this
   // will in fact just decrease lua's reference to it, so it's not a
   // bad thing to include this line.
   data->node = 0;
   
   return 0;
}


// Render a scene
extern "C"
int gr_render_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   gr_node_ud* root = (gr_node_ud*)luaL_checkudata(L, 1, "gr.node");
   luaL_argcheck(L, root != 0, 1, "Root node expected");
   
   const char* filename = luaL_checkstring(L, 2);
   
   int width = (int)luaL_checknumber(L, 3);
   int height = (int)luaL_checknumber(L, 4);
   
   Point eye;
   Vector view, up;
   
   get_tuple(L, 5, &eye[0], 3);
   get_tuple(L, 6, &view[0], 3);
   get_tuple(L, 7, &up[0], 3);
   
   double fov = luaL_checknumber(L, 8);
   
   double ambient_data[3];
   get_tuple(L, 9, ambient_data, 3);
   Colour ambient(ambient_data[0], ambient_data[1], ambient_data[2]);
   
   luaL_checktype(L, 10, LUA_TTABLE);
   int light_count = luaL_getn(L, 10);
   
   luaL_argcheck(L, light_count >= 1, 10, "Tuple of lights expected");
   std::list<Light*> lights;
   for (int i = 1; i <= light_count; i++) {
      lua_rawgeti(L, 10, i);
      gr_light_ud* ldata = (gr_light_ud*)luaL_checkudata(L, -1, "gr.light");
      luaL_argcheck(L, ldata != 0, 10, "Light expected");
      
      lights.push_back(ldata->light);
      lua_pop(L, 1);
   }
   
   Scene* scene = new Scene(*root->node, lights, ambient);
   scene->prepare();
   
   // XXX come back and fix this for interactive.
   
   RayTracer* rt = new RayTracer();
   Camera *c = new PerspectiveCamera(eye, eye + view, up,
                                     fov, 0.1, 1000000, width, height,
                                     Prefs::sharedPrefs().lensRadius,
                                     Prefs::sharedPrefs().focalDistance); 
   /*
    * Choose a very large number for the far plane so that we can acomodate 
    * scenes with larger scales. This still won't work with all scenes.
    * the lua command really should be updated to pass in near and far.
    * Can't just use INFINITY because we get an error when constructing the 
    * perpective projection (ie the view volume must be finite).
    */
   
   Stats::sharedStats().nextRender(); // prints the last render's stats if any.
   std::cout << std::endl << std::endl << "Rendering " << filename << "..." << std::endl;   
   std::cout << Prefs::sharedPrefs();
   
   Stats::sharedStats().imageWidth = width;
   Stats::sharedStats().imageHeight = height;
   
   rt->render(*scene, *c, width, height);
   
   rt->getImage().savePng(*(new std::string(filename)));
   
   delete rt;
   return 0;
}


// CGNode = gr.union(name a, b)
extern "C"
int gr_union_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   const char* name = luaL_checkstring(L, 1);
   
   gr_node_ud* aData = (gr_node_ud*)luaL_checkudata(L, 2, "gr.node");
   luaL_argcheck(L, aData != 0, 1, "Node expected");
   GeometryNode* a = dynamic_cast<GeometryNode *>(aData->node);
   ASSERT(a);
   
   gr_node_ud* bData = (gr_node_ud*)luaL_checkudata(L, 3, "gr.node");
   luaL_argcheck(L, bData != 0, 2, "Node expected");   
   GeometryNode* b = dynamic_cast<GeometryNode *>(bData->node);
   ASSERT(b);

   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;   
   data->node = new ConstructiveGeometryNode(name, a, b, UNION);
   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}


// CGNode = gr.intersection(name, nodeA, nodeB)
extern "C"
int gr_intersection_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   const char* name = luaL_checkstring(L, 1);
   
   gr_node_ud* aData = (gr_node_ud*)luaL_checkudata(L, 2, "gr.node");
   luaL_argcheck(L, aData != 0, 1, "Node expected");
   GeometryNode* a = dynamic_cast<GeometryNode *>(aData->node);
   ASSERT(a);
   
   gr_node_ud* bData = (gr_node_ud*)luaL_checkudata(L, 3, "gr.node");
   luaL_argcheck(L, bData != 0, 2, "Node expected");   
   GeometryNode* b = dynamic_cast<GeometryNode *>(bData->node);
   ASSERT(b);
   
   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;   
   data->node = new ConstructiveGeometryNode(name, a, b, INTERSECTION);
   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}


// CGNode = gr.difference(name, nodeA, nodeB)
extern "C"
int gr_difference_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;
   
   const char* name = luaL_checkstring(L, 1);
   
   gr_node_ud* aData = (gr_node_ud*)luaL_checkudata(L, 2, "gr.node");
   luaL_argcheck(L, aData != 0, 1, "Node expected");
   GeometryNode* a = dynamic_cast<GeometryNode *>(aData->node);
   ASSERT(a);
   
   gr_node_ud* bData = (gr_node_ud*)luaL_checkudata(L, 3, "gr.node");
   luaL_argcheck(L, bData != 0, 2, "Node expected");   
   GeometryNode* b = dynamic_cast<GeometryNode *>(bData->node);
   ASSERT(b);
   
   
   gr_node_ud* data = (gr_node_ud*)lua_newuserdata(L, sizeof(gr_node_ud));
   data->node = 0;   
   data->node = new ConstructiveGeometryNode(name, a, b, DIFFERENCE);
   
   luaL_getmetatable(L, "gr.node");
   lua_setmetatable(L, -2);
   
   return 1;
}


// Enable/Disable properties

extern "C"
int gr_enable_bounds_checking_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;   
   Prefs::sharedPrefs().boundsChecking = true;
   return 0;
}
extern "C"
int gr_disable_bounds_checking_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;   
   Prefs::sharedPrefs().boundsChecking = false;
   return 0;
}


extern "C"
int gr_enable_stratified_sampling_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL; 
   uint8_t numStrata = luaL_checknumber(L, 1);
   Prefs::sharedPrefs().numStrata = numStrata == 0 ? 1 : numStrata;
   return 0;
}
extern "C"
int gr_disable_stratified_sampling_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;   
   Prefs::sharedPrefs().numStrata = 1;
   return 0;
}


extern "C"
int gr_enable_jittered_sampling_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL; 
   Prefs::sharedPrefs().jitterSamples = true;
   return 0;
}
extern "C"
int gr_disable_jittered_sampling_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;   
   Prefs::sharedPrefs().jitterSamples = false;
   return 0;
}


extern "C"
int gr_enable_depth_of_field_sampling_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL; 
   double lensRadius = luaL_checknumber(L, 1);
   double focalDistance = luaL_checknumber(L, 2);
   Prefs::sharedPrefs().lensRadius = lensRadius;
   Prefs::sharedPrefs().focalDistance = focalDistance;
   return 0;
}
extern "C"
int gr_disable_depth_of_field_sampling_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;   
   Prefs::sharedPrefs().lensRadius = 0.0;
   Prefs::sharedPrefs().focalDistance = 0.0;
   return 0;
}

extern "C"
int gr_enable_ray_recursions_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL; 
   uint8_t numRecursions = luaL_checknumber(L, 1);
   Prefs::sharedPrefs().rayRecursions = max(numRecursions, 0);
   return 0;
}
extern "C"
int gr_disable_ray_recursions_cmd(lua_State* L)
{
   GRLUA_DEBUG_CALL;   
   Prefs::sharedPrefs().rayRecursions = 0;
   return 0;
}


// This is where all the member functions for "gr.node" objects are
// declared. Since all the other objects (e.g. materials) are so
// simple, we only really need to make member functions for nodes.
//
// If you want to add a new member function for gr.node, add it
// here.
//
// We could have used inheritance in lua to match the inheritance
// between different node types, but it's easier to just give all
// nodes the same Lua type and then do any additional type checking in
// the appropriate member functions (e.g. gr_node_set_material_cmd
// ensures that the node is a GeometryNode, see above).
static const luaL_reg grlib_node_methods[] = {
   {"__gc", gr_node_gc_cmd},
   {"add_child", gr_node_add_child_cmd},
   {"set_material", gr_node_set_material_cmd},
   {"scale", gr_node_scale_cmd},
   {"rotate", gr_node_rotate_cmd},
   {"translate", gr_node_translate_cmd},
   {0, 0}
};


// This is where all the "global" functions in our library are
// declared.
// If you want to add a new non-member function, add it here.
static const luaL_reg grlib_functions[] = {
   {"node", gr_node_cmd},
   {"sphere", gr_sphere_cmd},
   {"material", gr_material_cmd},
   {"rtMaterial", gr_rt_material_cmd},
   {"constantTexture", gr_constant_texture_cmd},
   {"uvTexture", gr_uv_texture_cmd},
   {"checkerTexture", gr_checker_texture_cmd},
   {"imageTexture", gr_image_texture_cmd},
   
   // should be a member funct of material but couldn't figure out how to do it.
   {"setBumpMap", gr_material_set_bump_map_cmd},

   // New for assignment 4
   {"cube", gr_cube_cmd},
   {"nh_sphere", gr_nh_sphere_cmd},
   {"nh_box", gr_nh_box_cmd},
   //{"mesh", gr_mesh_cmd},
   {"light", gr_light_cmd},
   {"render", gr_render_cmd},
   
   {"cylinder", gr_cylinder_cmd},
   {"cone", gr_cone_cmd},
   {"paraboloid", gr_paraboloid_cmd},
   
   {"union", gr_union_cmd},
   {"intersection", gr_intersection_cmd},
   {"difference", gr_difference_cmd},
   
   {"enableBoundsChecking", gr_enable_bounds_checking_cmd},
   {"disableBoundsChecking", gr_disable_bounds_checking_cmd},
   {"enableStratifiedSampling", gr_enable_stratified_sampling_cmd},
   {"disableStratifiedSampling", gr_disable_stratified_sampling_cmd},  
   {"enableJitteredSampling", gr_enable_jittered_sampling_cmd},
   {"disableJitteredSampling", gr_disable_jittered_sampling_cmd},
   {"enableDepthOfField", gr_enable_depth_of_field_sampling_cmd},
   {"disableDepthOfField", gr_disable_depth_of_field_sampling_cmd},
   {"enableRayRecursions", gr_enable_ray_recursions_cmd},
   {"disableRayRecursions", gr_disable_ray_recursions_cmd},
   
   {0, 0}
};



/*
 *------------------------------------------------------------------------------
 * 
 * runLua --
 *     
 *     This function calls the lua interpreter to define the scene and raytrace 
 *     it as appropriate.
 *
 *------------------------------------------------------------------------------
 */

bool 
runLua(const std::string& filename)
{
   GRLUA_DEBUG("Importing scene from " << filename);
   
   // Start a lua interpreter
   lua_State* L = lua_open();
   
   GRLUA_DEBUG("Loading base libraries");
   
   // Load some base library
   luaopen_base(L);
   luaopen_io(L);
   luaopen_string(L);
   luaopen_math(L);
   luaopen_table(L);
   
   GRLUA_DEBUG("Setting up our functions");
   
   // Set up the metatable for gr.node
   luaL_newmetatable(L, "gr.node");
   lua_pushstring(L, "__index");
   lua_pushvalue(L, -2);
   lua_settable(L, -3);
   
   // Load the gr.node methods
   luaL_openlib(L, 0, grlib_node_methods, 0);
   
   // Load the gr functions
   luaL_openlib(L, "gr", grlib_functions, 0);
   
   GRLUA_DEBUG("Parsing the scene");
   
   // Now parse the actual scene
   if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0)) {
      std::cerr << "Error loading " << filename << ": " << lua_tostring(L, -1) << std::endl;
      return false;
   }
   GRLUA_DEBUG("Closing the interpreter");
   
   // Close the interpreter, free up any resources not needed
   lua_close(L);
   
   return true;
}
