--[[ 
A simple scene demonstrating both reflection and transmission. Each successive 
render increases the maximum recursion number. After a certain number of 
recursions the difference in the image is not noticeable. Of course a recursion 
of 0 will produce an image with no reflection or transmission.

For reflective surfaces any recursion higher than 1 will only affect scenes that 
have more than 1 reflective solid both in line of sight of each other. For
for semi-opague materials a recursion depth of at least 2 is needed for the ray
to exit the solid and then test at least 1 object on the other side.
]]

mat1 = gr.material(gr.constantTexture({0.7, 1.0, 0.7}), {0.5, 0.7, 0.5}, 25)
mat2 = gr.material(gr.constantTexture({0.5, 0.5, 0.5}), {0.5, 0.7, 0.5}, 25)
mat3 = gr.material(gr.constantTexture({1.0, 0.6, 0.1}), {0.5, 0.7, 0.5}, 25)


transmissiveMatSilver = gr.rtMaterial(gr.constantTexture({1.0, 1.0, 1.0}), 
                                    {0.9, 0.9, 0.9}, 
                                    5,
                                    0.0,
                                    0.01,
                                    1.0)
                                    
reflectiveMatSilver = gr.rtMaterial(gr.constantTexture({0.9, 0.9, 0.9}), 
                                    {0.9, 0.9, 0.9}, 
                                    5,
                                    0.9,
                                    1.0,
                                    1.0)

reflectiveMatBlue = gr.rtMaterial(gr.constantTexture({0.47, 0.8, 1.0}), 
                                  {0.9, 0.9, 0.9}, 
                                  5,
                                  0.9,
                                  1.0,
                                  1.0)
scene_root = gr.node('root')

s1 = gr.nh_sphere('s1', {0, 0, -400}, 100)
scene_root:add_child(s1)
s1:set_material(reflectiveMatBlue)

s2 = gr.nh_sphere('s2', {100, 50, -100}, 150)
scene_root:add_child(s2)
s2:set_material(transmissiveMatSilver)

s3 = gr.nh_sphere('s3', {0, -1200, -500}, 1000)
scene_root:add_child(s3)
s3:set_material(reflectiveMatSilver)

b1 = gr.nh_box('b1', {-200, -125, 0}, 100)
scene_root:add_child(b1)
b1:set_material(mat3)

s4 = gr.nh_sphere('s4', {-100, 25, -300}, 50)
scene_root:add_child(s4)
s4:set_material(mat3)

s5 = gr.nh_sphere('s5', {0, 100, -250}, 25)
scene_root:add_child(s5)
s5:set_material(mat1)


white_light = gr.light({-100.0, 150.0, 400.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
     
gr.enableBoundsChecking()
gr.enableStratifiedSampling(5)
gr.enableJitteredSampling()

gr.enableRayRecursions(0)
gr.render(scene_root, 'Transmission-500x500-r0.png', 500, 500,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {white_light})
     
gr.enableRayRecursions(1)
gr.render(scene_root, 'Transmission-500x500-r1.png', 500, 500,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {white_light})

gr.enableRayRecursions(2)
gr.render(scene_root, 'Transmission-500x500-r2.png', 500, 500,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {white_light})
     
gr.enableRayRecursions(3)
gr.render(scene_root, 'Transmission-500x500-r3.png', 500, 500,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {white_light})
     
gr.enableRayRecursions(10)
gr.render(scene_root, 'Transmission-500x500-r10.png', 500, 500,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {white_light})
     
gr.enableRayRecursions(10)
gr.render(scene_root, 'Transmission-1000x1000-reference.png', 1000, 1000,
	  {0, 0, 800}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.0, 0.0, 0.0}, {white_light})
     