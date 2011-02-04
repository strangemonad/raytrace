--[[ 
A simple scene demonstrating constructive solid geometry.

The scene will be rendered several times to demonstrate a few of the levels of
sampling.
]]

require('common/materials')

scene_root = gr.node('root')

-- Spheres
sphereRow = gr.node('sphereRow')
sphereRow:translate(0, 500, 0)
scene_root:add_child(sphereRow)

-- Union
s1 = gr.nh_sphere('s1', {-50, 0, 0}, 100)
s1:set_material(paleRedPlastic)
s2 = gr.nh_sphere('s2', {50, 0, 0}, 100)
s2:set_material(paleGreenPlastic)
sphereUnion = gr.union('sphereUnion', s1, s2)
sphereUnion:translate(-400, 0, 0)
sphereRow:add_child(sphereUnion)

-- Intersection
s3 = gr.nh_sphere('s3', {-50, 0, 0}, 100)
s3:set_material(paleRedPlastic)
s4 = gr.nh_sphere('s4', {50, 0, 0}, 100)
s4:set_material(paleGreenPlastic)
sphereIntersection = gr.intersection('sphereIntersection', s3, s4)
sphereIntersection:rotate('x', -50)
--sphereIntersection:rotate('y', -70)
sphereIntersection:rotate('z', 20)
sphereRow:add_child(sphereIntersection)

-- Difference
s5 = gr.nh_sphere('s5', {-50, 0, 0}, 100)
s5:set_material(paleRedPlastic)
s6 = gr.nh_sphere('s6', {50, 0, 0}, 100)
s6:set_material(paleGreenPlastic)
sphereDifference = gr.difference('sphereDifference', s5, s6)
sphereDifference:rotate('y', -60)
sphereDifference:rotate('x', -40)
sphereDifference:translate(430, 40, 0)
sphereRow:add_child(sphereDifference)


-- Cylinders
cylRow = gr.node('cylRow')
--cylRow:translate(0, 200, 0)
scene_root:add_child(cylRow)

-- Union
cyl1 = gr.cylinder('cyl', {0,0,0}, 100, 150)
cyl1:rotate('y', 0)
cyl1:rotate('x', -90)
cyl1:translate(-30, 0, 0)
cyl1:set_material(paleRedPlastic)
cyl2 = gr.cylinder('cyl', {0,0,0}, 100, 150)
cyl2:rotate('y', 30)
cyl2:set_material(paleGreenPlastic)
cylUnion = gr.union('cylUnion', cyl1, cyl2)
cylUnion:translate(-400, 0, 0)
cylUnion:rotate('x', 125)
cylRow:add_child(cylUnion)

-- Intersection
cyl3 = gr.cylinder('cyl', {-50,0,0}, 100, 150)
cyl3:set_material(paleRedPlastic)
cyl4 = gr.cylinder('cyl', {50,0,0}, 100, 150)
cyl4:set_material(paleGreenPlastic)
cylInt = gr.intersection('cylInt', cyl3, cyl4)
cylInt:rotate('x', -70)
--cylInt:rotate('z', 20)
--cylRow:add_child(cylInt)

-- Difference
cyl5 = gr.cylinder('cyl', {-50,0,0}, 100, 150)
cyl5:set_material(paleRedPlastic)
cyl6 = gr.cylinder('cyl', {50,0,0}, 100, 150)
cyl6:set_material(paleGreenPlastic)
cylDiff = gr.difference('cylDiff', cyl5, cyl6)
cylDiff:rotate('x', -30)
cylDiff:rotate('z', 40)
cylDiff:translate(470, 0, 0)
--cylRow:add_child(cylDiff)


-- Cones
coRow = gr.node('coRow')
coRow:translate(0, 200, 0)
--scene_root:add_child(coRow)

-- Union
co1 = gr.cone('co', {0,0,0}, 100, 150)
co1:rotate('y', -30)
co1:rotate('x', -90)
co1:translate(-30, 0, 0)
co1:set_material(paleRedPlastic)
co2 = gr.cone('co', {0,0,0}, 100, 150)
co2:rotate('y', 30)
co2:set_material(paleGreenPlastic)
coUnion = gr.union('coUnion', co1, co2)
coUnion:translate(-400, -200, 0)
coRow:add_child(coUnion)

-- Intersection
co3 = gr.cone('co', {-50,0,0}, 100, 150)
co3:set_material(paleRedPlastic)
co4 = gr.cone('co', {50,0,0}, 100, 150)
co4:set_material(paleGreenPlastic)
coInt = gr.intersection('coInt', co3, co4)
--coInt:rotate('x', 90)
coInt:rotate('z', 20)
--coRow:add_child(coInt)

-- Difference
co5 = gr.cone('co', {-50,0,0}, 100, 150)
co5:set_material(paleRedPlastic)
co6 = gr.cone('co', {50,0,0}, 100, 150)
co6:set_material(paleGreenPlastic)
coDiff = gr.difference('coDiff', co5, co6)
coDiff:rotate('x', -30)
coDiff:rotate('z', 40)
coDiff:translate(400, 0, 0)
--coRow:add_child(coDiff)


mainLight = gr.light({0, 1000, 10000}, {0.7, 0.7, 0.7}, {1, 0, 0})
diffuseLight = gr.light({0, 1000, 1000}, {0.4, 0.3, 0.3}, {1, 0, 0})

gr.enableBoundsChecking()

gr.enableStratifiedSampling(1)
gr.render(scene_root, 'CSG-500x500-s1.png', 500, 500,
	  {0, 0, 1500}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})

gr.enableStratifiedSampling(2)
gr.render(scene_root, 'CSG-500x500-s2.png', 500, 500,
	  {0, 0, 1500}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(3)
gr.render(scene_root, 'CSG-500x500-s3.png', 500, 500,
	  {0, 0, 1500}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})

gr.enableStratifiedSampling(4)
gr.render(scene_root, 'CSG-500x500-s4.png', 500, 500,
	  {0, 0, 1500}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
     
gr.enableJitteredSampling()
gr.enableStratifiedSampling(1)
gr.render(scene_root, 'CSG-500x500-s1j.png', 500, 500,
	  {0, 0, 1500}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})

gr.enableStratifiedSampling(2)
gr.render(scene_root, 'CSG-500x500-s2j.png', 500, 500,
	  {0, 0, 1500}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(3)
gr.render(scene_root, 'CSG-500x500-s3j.png', 500, 500,
	  {0, 0, 1500}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})

gr.enableStratifiedSampling(4)
gr.render(scene_root, 'CSG-500x500-s4j.png', 500, 500,
	  {0, 0, 1500}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
     
gr.enableStratifiedSampling(10)
gr.render(scene_root, 'CSG-1000x1000-reference.png', 1000, 1000,
	  {0, 0, 1500}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {mainLight, diffuseLight})
