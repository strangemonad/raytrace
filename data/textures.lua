--[[ 
A simple scene demonstrating the various texturing capabilities of the ray 
tracer.
]]

require('common/materials')

scene_root = gr.node("root")

s = gr.nh_sphere('s', {0, 0, 0}, 180)
s:rotate('x', 130)
s:translate(-300, 300, 0)
scene_root:add_child(s)

cyl = gr.cylinder('cyl', {0,0,0}, 120, 250)
cyl:rotate('x', -90)
cyl:rotate('z', 30)
cyl:translate(350, 220, 0)
scene_root:add_child(cyl)

co = gr.cone('co', {0,0,0}, 120, 250)
co:rotate('x', -120)
co:rotate('z', 30)
co:translate(-250, -350, 0)
scene_root:add_child(co)

p = gr.paraboloid('p', {0,0,0}, 120, 250)
p:rotate('x', -70)
p:rotate('z', 30)
p:translate(400, -400, 0)
scene_root:add_child(p)

c = gr.nh_box('c', {0, 0, 0}, 200)
c:rotate('x', 30)
c:rotate('y', 30)
c:translate(-100, -50, 0)
scene_root:add_child(c)

mainLight = gr.light({0, 1000, 10000}, {0.7, 0.7, 0.7}, {1, 0, 0})
diffuseLight = gr.light({0, 1000, 1000}, {0.4, 0.3, 0.3}, {1, 0, 0})

gr.enableBoundsChecking()
gr.enableStratifiedSampling(5)
gr.enableJitteredSampling()

     

s:set_material(plainUV)
cyl:set_material(plainUV)
co:set_material(plainUV)
p:set_material(plainUV)
c:set_material(plainUV)
gr.render(scene_root, 'Textures-500x500-uvTexture.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})


s:set_material(bwCheckers4)
cyl:set_material(bwCheckers4)
co:set_material(bwCheckers4)
p:set_material(bwCheckers4)
c:set_material(bwCheckers4)
gr.render(scene_root, 'Textures-500x500-checkerTexture4.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
     
s:set_material(bwCheckers10)
cyl:set_material(bwCheckers10)
co:set_material(bwCheckers10)
p:set_material(bwCheckers10)
c:set_material(bwCheckers10)
gr.render(scene_root, 'Textures-500x500-checkerTexture10.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     

s:set_material(bwCheckers30)
cyl:set_material(bwCheckers30)
co:set_material(bwCheckers30)
p:set_material(bwCheckers30)
c:set_material(bwCheckers30)
gr.render(scene_root, 'Textures-500x500-checkerTexture10.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     

gr.enableStratifiedSampling(20)
s:set_material(bwCheckers100)
cyl:set_material(bwCheckers100)
co:set_material(bwCheckers100)
p:set_material(bwCheckers100)
c:set_material(bwCheckers100)
gr.render(scene_root, 'Textures-500x500-checkerTexture100.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})


gr.enableStratifiedSampling(5)
s:set_material(dof)
cyl:set_material(dof)
co:set_material(dof)
p:set_material(dof)
c:set_material(dof)
gr.render(scene_root, 'Textures-500x500-imageTexture.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
     
s:set_material(sandStoneSingle)
cyl:set_material(sandStoneSingle)
co:set_material(sandStoneSingle)
p:set_material(sandStoneSingle)
c:set_material(sandStoneSingle)
gr.render(scene_root, 'Textures-500x500-stoneStretched.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})

s:set_material(targetTile)
cyl:set_material(targetTile)
co:set_material(targetTile)
p:set_material(targetTile)
c:set_material(targetTile)
gr.render(scene_root, 'Textures-500x500-target.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
     
s:set_material(targetAppleTile)
cyl:set_material(targetAppleTile)
co:set_material(targetAppleTile)
p:set_material(targetAppleTile)
c:set_material(targetAppleTile)
gr.render(scene_root, 'Textures-500x500-targetApple.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
     
s:set_material(sandStoneTiled)
cyl:set_material(sandStoneTiled)
co:set_material(sandStoneTiled)
p:set_material(sandStoneTiled)
c:set_material(sandStoneTiled)
gr.render(scene_root, 'Textures-500x500-stoneTiled.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})