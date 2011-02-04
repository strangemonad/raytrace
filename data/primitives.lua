


--[[ 
A simple scene demonstrating the primitives that the ray tracer is capable of.

The scene will be rendered several times to demonstrate a few of the levels of
sampling.
]]

require('common/materials')

scene_root = gr.node("root")

s = gr.nh_sphere('s', {-300, 300, 0}, 180)
s:set_material(paleGreenPlastic)
scene_root:add_child(s)

cyl = gr.cylinder('cyl', {0,0,0}, 120, 250)
cyl:rotate('x', -90)
cyl:rotate('z', 30)
cyl:translate(350, 220, 0)
cyl:set_material(paleGreenPlastic)
scene_root:add_child(cyl)

co = gr.cone('co', {0,0,0}, 120, 250)
co:set_material(paleGreenPlastic)
co:rotate('x', -120)
co:rotate('z', 30)
co:translate(-250, -350, 0)
scene_root:add_child(co)

p = gr.paraboloid('p', {0,0,0}, 120, 250)
p:set_material(paleGreenPlastic)
p:rotate('x', -70)
p:rotate('z', 30)
p:translate(400, -400, 0)
scene_root:add_child(p)

c = gr.nh_box('c', {0, 0, 0}, 200)
c:set_material(paleGreenPlastic)
c:rotate('x', 30)
c:rotate('y', 30)
c:translate(-100, -50, 0)
scene_root:add_child(c)

mainLight = gr.light({0, 1000, 10000}, {0.7, 0.7, 0.7}, {1, 0, 0})
diffuseLight = gr.light({0, 1000, 1000}, {0.4, 0.3, 0.3}, {1, 0, 0})

gr.enableBoundsChecking()

gr.render(scene_root, 'Primitives-500x500-s1.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})

gr.enableStratifiedSampling(2)
gr.render(scene_root, 'Primitives-500x500-s2.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(3)
gr.render(scene_root, 'Primitives-500x500-s3.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(4)
gr.render(scene_root, 'Primitives-500x500-s4.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})


gr.enableJitteredSampling()
gr.disableStratifiedSampling()
gr.render(scene_root, 'Primitives-500x500-s1j.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})

gr.enableStratifiedSampling(2)
gr.render(scene_root, 'Primitives-500x500-s2j.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(3)
gr.render(scene_root, 'Primitives-500x500-s3j.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(4)
gr.render(scene_root, 'Primitives-500x500-s4j.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(10)
gr.render(scene_root, 'Primitives-1000x1000-reference.png', 1000, 1000,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     