--
-- CS488 -- Introduction to Computer Graphics
-- 
-- test.lua
-- smorel - 20090740
-- 
-- Puppet scene descirption

red = gr.material(gr.constantTexture({1.0, 0.0, 0.0}), {0.1, 0.1, 0.1}, 10)
blue = gr.material(gr.constantTexture({0.0, 0.0, 1.0}), {0.1, 0.1, 0.1}, 10)
green = gr.material(gr.constantTexture({0.0, 1.0, 0.0}), {0.1, 0.1, 0.1}, 10)
white = gr.rtMaterial(gr.constantTexture({1.0, 1.0, 1.0}), {0.1, 0.1, 0.1}, 2,
                      0.1, 1.0, 1.0)
cloud = gr.material(gr.constantTexture({0.9, 0.9, 0.9}), {0.1, 0.1, 0.1}, 10)
gold = gr.material(gr.constantTexture({0.9, 0.8, 0.4}), {0.8, 0.8, 0.4}, 25)


rootnode = gr.node('root')
rootnode:translate(0.0, 0.0, -30)

-- Floor

floor = gr.cube('floor')
floor:scale(10.0, 1.0, 100.0)
floor:translate(-5.0, -7.0, -20.0)
floor:set_material(white)
rootnode:add_child(floor)

sun = gr.cube('sun')
sun:scale(2, 2, 2)
sun:translate(-9, 7, -15)
sun:set_material(gold)
rootnode:add_child(sun)


-- Cloud
c1 = gr.cube('c1')
c1:scale(1, 1, 1)
c1:translate(-1, 6, -10)
c1:set_material(cloud)
rootnode:add_child(c1)

c2 = gr.cube('c2')
c2:scale(1, 1, 1)
c2:translate(-5, 7, -9)
c2:set_material(cloud)
rootnode:add_child(c2)

c3 = gr.cube('c3')
c3:scale(2, 2, 2)
c3:translate(-6, 5, -9)
c3:set_material(white)
rootnode:add_child(c3)

c4 = gr.cube('c4')
c4:scale(2, 2, 2)
c4:translate(-4, 6, -8.5)
c4:set_material(cloud)
rootnode:add_child(c4)

c5 = gr.cube('c5')
c5:scale(1, 1, 1)
c5:translate(-4, 4, -7)
c5:set_material(cloud)
rootnode:add_child(c5)

-- Torso
t = gr.node('t')
rootnode:add_child(t)

torso = gr.sphere('torso')
torso:scale(1.0, 2.2, 0.4)
torso:set_material(red)
t:add_child(torso)

-- Shoulders
s = gr.node('s')
s:translate(0.0, 2.2, 0.0)
t:add_child(s)

shoulders = gr.sphere('shoulders')
shoulders:scale(1.6, 0.4, 0.4)
shoulders:set_material(blue)
s:add_child(shoulders)

-- Left Arm
la = gr.node('la')
la:translate(1.4, -0.2, 0.0)
la:rotate('z', 20.0)
s:add_child(la)

lBicep = gr.sphere('lBicep')
lBicep:scale(0.3, 1.5, 0.3)
lBicep:translate(0.0, -1.5, 0.0)
lBicep:set_material(green)
la:add_child(lBicep)

le = gr.node('le')
le:translate(0.0, -3.0, 0.0)
la:add_child(le)

lForearm = gr.sphere('lForearm')
lForearm:scale(0.2, 1.2, 0.2)
lForearm:translate(0.0, -0.8, 0.0)
lForearm:set_material(red)
le:add_child(lForearm)

lh = gr.node('lh')
lh:translate(0.0, -1.6, 0.0)
le:add_child(lh)

lHand = gr.sphere('lHand')
lHand:scale(0.2, 0.2, 0.2)
lHand:translate(0.0, -0.2, 0.0)
lHand:set_material(blue)
lh:add_child(lHand)


-- Right Arm
ra = gr.node('ra')
ra:translate(-1.4, -0.2, 0.0)
ra:rotate('z', -20.0)
s:add_child(ra)

rBicep = gr.sphere('rBicep')
rBicep:scale(0.3, 1.5, 0.3)
rBicep:translate(0.0, -1.5, 0.0)
rBicep:set_material(green)
ra:add_child(rBicep)

re = gr.node('re')
re:translate(0.0, -3.0, 0.0)
ra:add_child(re)

rForearm = gr.sphere('rForearm')
rForearm:scale(0.2, 1.2, 0.2)
rForearm:translate(0.0, -0.8, 0.0)
rForearm:set_material(red)
re:add_child(rForearm)

rh = gr.node('rh')
rh:translate(0.0, -1.6, 0.0)
re:add_child(rh)

rHand = gr.sphere('rHand')
rHand:scale(0.2, 0.2, 0.2)
rHand:translate(0.0, -0.2, 0.0)
rHand:set_material(blue)
rh:add_child(rHand)



-- Hips
h = gr.node('h')
h:translate(0.0, -2.2, 0.0)
t:add_child(h)

hips = gr.sphere('hips')
hips:scale(1.3, 0.4, 0.4)
hips:set_material(blue)
h:add_child(hips)

-- Left leg
ll = gr.node('ll')
ll:translate(1.1, 0.0, 0.0)
h:add_child(ll)

lThigh = gr.sphere('lThigh')
lThigh:scale(0.6, 1.4, 0.6)
lThigh:translate(0.0, -1.4, 0.0)
lThigh:set_material(green)
ll:add_child(lThigh)

lk = gr.node('lk')
lk:translate(0.0, -2.8, 0.0)
ll:add_child(lk)

lShin = gr.sphere('lShin')
lShin:scale(0.3, 1.3, 0.4)
lShin:translate(0.0, -1.1, 0.0)
lShin:set_material(red)
lk:add_child(lShin)

lf = gr.node('lf')
lf:translate(0.0, -2.2, 0.0)
lk:add_child(lf)

lFoot = gr.sphere('lFoot')
lFoot:scale(0.3, 0.2, 0.4)
lFoot:translate(0.0, -0.2, 0.4)
lFoot:set_material(blue)
lf:add_child(lFoot)



-- Right leg
rl = gr.node('rl')
rl:translate(-1.1, 0.0, 0.0)
h:add_child(rl)

rThigh = gr.sphere('rThigh')
rThigh:scale(0.6, 1.4, 0.6)
rThigh:translate(0.0, -1.4, 0.0)
rThigh:set_material(green)
rl:add_child(rThigh)

rk = gr.node('rk')
rk:translate(0.0, -2.8, 0.0)
rl:add_child(rk)

rShin = gr.sphere('rShin')
rShin:scale(0.3, 1.3, 0.4)
rShin:translate(0.0, -1.1, 0.0)
rShin:set_material(red)
rk:add_child(rShin)

rf = gr.node('rf')
rf:translate(0.0, -2.2, 0.0)
rk:add_child(rf)

rFoot = gr.sphere('rFoot')
rFoot:scale(0.3, 0.2, 0.4)
rFoot:translate(0.0, -0.2, 0.4)
rFoot:set_material(blue)
rf:add_child(rFoot)

white_light = gr.light({-40.0, 50.0, 40.0}, {0.9, 0.9, 0.9}, {1, 0, 0})
white_light2 = gr.light({0.0, 80.0, -300}, {0.3, 0.3, 0.3}, {1, 0, 0})
white_light3 = gr.light({-9, 8.0, -12}, {0.3, 0.3, 0.3}, {1, 0, 0})
         
gr.enableBoundsChecking()
gr.enableStratifiedSampling(5)
gr.enableJitteredSampling()
gr.render(rootnode, 'sample.png', 1024, 1024,
         {0, 0, 10}, {0, 0, -20}, {0, 1, 0}, 40,
         {0.0, 0.0, 0.0}, {white_light, white_light2, white_light3})