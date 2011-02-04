--[[ 
A simple scene demonstrating the various texturing capabilities of the ray 
tracer.
]]

require('common/materials')

scene_root = gr.node("root")

s = gr.nh_sphere('s', {0, 0, 0}, 300)
s:rotate('x', 130)
s:rotate('y', 50)
scene_root:add_child(s)

mainLight = gr.light({0, 1000, 10000}, {0.7, 0.7, 0.7}, {1, 0, 0})
diffuseLight = gr.light({0, 1000, 1000}, {0.4, 0.3, 0.3}, {1, 0, 0})

gr.enableStratifiedSampling(5)
gr.enableJitteredSampling()

s:set_material(gridMap)
gr.render(scene_root, 'BumpedSphere-500x500-stretched.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})

s:set_material(gridMapTiled)
gr.render(scene_root, 'BumpedSphere-500x500-tiled.png', 500, 500,
	  {0, 0, 1200}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})