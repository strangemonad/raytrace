-- A simple scene with five spheres

mat1 = gr.material(gr.constantTexture({0.7, 1.0, 0.7}), {0.5, 0.7, 0.5}, 25)
mat2 = gr.material(gr.constantTexture({0.5, 0.5, 0.5}), {0.5, 0.7, 0.5}, 25)
mat3 = gr.material(gr.constantTexture({1.0, 0.6, 0.1}), {0.5, 0.7, 0.5}, 25)

scene_root = gr.node("root")

cylRow = gr.node('cylRow')
--cylRow:translate(0, 200, 0)
scene_root:add_child(cylRow)

-- Union
cyl1 = gr.cylinder('cyl', {0,0,0}, 100, 150)
--cyl1:rotate('y', -20)
cyl1:rotate('x', 30)
cyl1:translate(-30, 0, 0)
cyl1:set_material(mat1)


cyl2 = gr.cylinder('cyl', {0,0,0}, 100, 150)
cyl2:rotate('y', 30)
cyl2:set_material(mat3)
cylUnion = gr.union('cylUnion', cyl1, cyl2)
cylUnion:translate(-400, 0, 0)
cylUnion:rotate('x', 70)


c = gr.cone('c', {0,0,0}, 200, 300)
--c:rotate('y', -20)
c:rotate('x', 90)
c:set_material(mat1)

--scene_root:add_child(c)


--cylRow:add_child(cylUnion)
cylRow:add_child(cyl1)



white_light = gr.light({0.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})


gr.enableBoundsChecking()
gr.enableStratifiedSampling(2)

gr.render(scene_root, 'CSG-500x500.png', 500, 500,
	  {0, 0, 1500}, {0, 0, -100}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {white_light})
