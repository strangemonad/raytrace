--[[ 
A simple scene demonstrating depth of a depth of field effect with a larger than
common fstop (f/22.5) to exagerate the effect.

The scene will be rendered several times to demonstrate a few of the levels of
sampling. Because the lens is sampled in a depth of field camera the smaller the 
lens radius (and hence the circle of confusion) the less samples are required.

For this example a fairly large number of samples is needed until an acceptable 
result is obtained. This problem could be addressed by having a more 
sofisticated image reconstruction filter rather than the less effective brute 
force approach of increasing the number of samples.

The first render is sampled with 1 stratum per pixel and 1 stratum per lens.
This produces an image with no depth of field effect since each sample ray goes
right through the center of the lens.
]]

require('common/materials')

scene_root = gr.node('root')


-- Cylinders
cylRow = gr.node('cylRow')
--cylRow:translate(0, 200, 0)
scene_root:add_child(cylRow)

cyl = gr.cylinder('cyl', {0,0,0}, 15, 200)
cyl:rotate('x', -90)
cyl:set_material(paleRedPlastic)
cylRow:add_child(cyl)

cyl = gr.cylinder('cyl', {0,0,0}, 15, 200)
cyl:rotate('x', -90)
cyl:translate(-80, 0, 0)
cyl:set_material(paleGreenPlastic)
cylRow:add_child(cyl)

cyl = gr.cylinder('cyl', {0,0,0}, 15, 200)
cyl:rotate('x', -90)
cyl:translate(-80*2^1.2, 0, 0)
cyl:set_material(paleRedPlastic)
cylRow:add_child(cyl)

cyl = gr.cylinder('cyl', {0,0,0}, 15, 200)
cyl:rotate('x', -90)
cyl:translate(-80*3^1.2, 0, 0)
cyl:set_material(paleGreenPlastic)
cylRow:add_child(cyl)

cyl = gr.cylinder('cyl', {0,0,0}, 15, 200)
cyl:rotate('x', -90)
cyl:translate(-80*4^1.2, 0, 0)
cyl:set_material(paleRedPlastic)
cylRow:add_child(cyl)

cyl = gr.cylinder('cyl', {0,0,0}, 15, 200)
cyl:rotate('x', -90)
cyl:translate(-80*5^1.2, 0, 0)
cyl:set_material(paleGreenPlastic)
cylRow:add_child(cyl)

cyl = gr.cylinder('cyl', {0,0,0}, 15, 200)
cyl:rotate('x', -90)
cyl:translate(-80*6^1.2, 0, 0)
cyl:set_material(paleRedPlastic)
cylRow:add_child(cyl)

cyl = gr.cylinder('cyl', {0,0,0}, 15, 200)
cyl:rotate('x', -90)
cyl:translate(-80*7^1.2, 0, 0)
cyl:set_material(paleGreenPlastic)
cylRow:add_child(cyl)

cyl = gr.cylinder('cyl', {0,0,0}, 15, 200)
cyl:rotate('x', -90)
cyl:translate(-80*8^1.2, 0, 0)
cyl:set_material(paleRedPlastic)
cylRow:add_child(cyl)

cyl = gr.cylinder('cyl', {0,0,0}, 15, 200)
cyl:rotate('x', -90)
cyl:translate(-80*9^1.2, 0, 0)
cyl:set_material(paleGreenPlastic)
cylRow:add_child(cyl)

cyl = gr.cylinder('cyl', {0,0,0}, 15, 200)
cyl:rotate('x', -90)
cyl:translate(-80*10^1.2, 0, 0)
cyl:set_material(paleRedPlastic)
cylRow:add_child(cyl)

cylRow:rotate('y', -60)
cylRow:translate(90, -100, 0)

mainLight = gr.light({0, 1000, 10000}, {0.7, 0.7, 0.7}, {1, 0, 0})
diffuseLight = gr.light({0, 1000, 1000}, {0.4, 0.3, 0.3}, {1, 0, 0})

gr.enableBoundsChecking()

gr.enableDepthOfField(20, 450)



gr.enableStratifiedSampling(1)
gr.render(scene_root, 'DepthOfField-500x500-s1-f_22.5.png', 500, 500,
	  {0, 0, 300}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(2)
gr.render(scene_root, 'DepthOfField-500x500-s2-f_22.5.png', 500, 500,
	  {0, 0, 300}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(3)
gr.render(scene_root, 'DepthOfField-500x500-s3-f_22.5.png', 500, 500,
	  {0, 0, 300}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(4)
gr.render(scene_root, 'DepthOfField-500x500-s4-f_22.5.png', 500, 500,
	  {0, 0, 300}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(5)
gr.render(scene_root, 'DepthOfField-500x500-s5-f_22.5.png', 500, 500,
	  {0, 0, 300}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
     

gr.enableJitteredSampling()
gr.enableStratifiedSampling(1)
gr.render(scene_root, 'DepthOfField-500x500-s1j-f_22.5.png', 500, 500,
	  {0, 0, 300}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(2)
gr.render(scene_root, 'DepthOfField-500x500-s2j-f_22.5.png', 500, 500,
	  {0, 0, 300}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(3)
gr.render(scene_root, 'DepthOfField-500x500-s3j-f_22.5.png', 500, 500,
	  {0, 0, 300}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(4)
gr.render(scene_root, 'DepthOfField-500x500-s4j-f_22.5.png', 500, 500,
	  {0, 0, 300}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(5)
gr.render(scene_root, 'DepthOfField-500x500-s5j-f_22.5.png', 500, 500,
	  {0, 0, 300}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(20)
gr.render(scene_root, 'DepthOfField-1000x1000-reference-f_22.5.png', 1000, 1000,
	  {0, 0, 300}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})