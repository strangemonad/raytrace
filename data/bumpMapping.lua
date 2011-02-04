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

     
     
s:set_material(checkEmbossedGreenPlastic)
cyl:set_material(checkEmbossedGreenPlastic)
co:set_material(checkEmbossedGreenPlastic)
p:set_material(checkEmbossedGreenPlastic)
c:set_material(checkEmbossedGreenPlastic)
gr.render(scene_root, 'BumpMapping-500x500-checkEmboss.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
     
s:set_material(appleEmbossGreenTile)
cyl:set_material(appleEmbossGreenTile)
co:set_material(appleEmbossGreenTile)
p:set_material(appleEmbossGreenTile)
c:set_material(appleEmbossGreenTile)
gr.render(scene_root, 'BumpMapping-500x500-appleEmbossGreen.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
s:set_material(appleEmbossAppleTile)
cyl:set_material(appleEmbossAppleTile)
co:set_material(appleEmbossAppleTile)
p:set_material(appleEmbossAppleTile)
c:set_material(appleEmbossAppleTile)
gr.render(scene_root, 'BumpMapping-500x500-appleEmbossApple.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
s:set_material(sandStoneTiled)
cyl:set_material(sandStoneTiled)
co:set_material(sandStoneTiled)
p:set_material(sandStoneTiled)
c:set_material(sandStoneSingle)
gr.render(scene_root, 'BumpMapping-500x500-sandStoneNoBump.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
s:set_material(sandStoneBumppedTiled)
cyl:set_material(sandStoneBumppedTiled)
co:set_material(sandStoneBumppedTiled)
p:set_material(sandStoneBumppedTiled)
c:set_material(sandStoneBumpped)
gr.render(scene_root, 'BumpMapping-500x500-sandStone.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})