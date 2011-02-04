-- Common materials used in many scenes.

paleGreenPlastic = gr.material(gr.constantTexture({0.7, 1.0, 0.7}),
                               {0.7, 0.7, 0.7}, 50)
paleRedPlastic = gr.material(gr.constantTexture({1.0, 0.7, 0.7}),
                             {0.7, 0.7, 0.7}, 50)
                             
plainUV = gr.material(gr.uvTexture(), {0.7, 0.7, 0.7}, 50)

bwCheckers4 = gr.material(gr.checkerTexture(gr.constantTexture({1, 1, 1}),
                                           gr.constantTexture({0, 0, 0}), 4),
                         {0.7, 0.7, 0.7}, 50)

bwCheckers10 = gr.material(gr.checkerTexture(gr.constantTexture({1, 1, 1}),
                                             gr.constantTexture({0, 0, 0}), 10),
                         {0.7, 0.7, 0.7}, 50)
                         
bwCheckers30 = gr.material(gr.checkerTexture(gr.constantTexture({1, 1, 1}),
                                             gr.constantTexture({0, 0, 0}), 30),
                         {0.7, 0.7, 0.7}, 50)       
                                           
bwCheckers100 = gr.material(gr.checkerTexture(gr.constantTexture({1, 1, 1}),
                                              gr.constantTexture({0, 0, 0}), 100),
                         {0.7, 0.7, 0.7}, 50)
                         
dof = gr.material(gr.imageTexture('DepthOfField-1000x1000-reference-f_22.5.png'),
                  {0.7, 0.7, 0.7}, 50)
                  
sandStoneText = gr.imageTexture('textures/sandStone.png')
sandStoneSingle = gr.material(sandStoneText, {0.0, 0.0, 0.0}, 50)
                  
targetTile = gr.material(gr.checkerTexture(gr.constantTexture({1, 1, 1}),
                                           gr.imageTexture('textures/target.png'), 4),
                         {0.7, 0.7, 0.7}, 50)
                         
targetAppleTile = gr.material(gr.checkerTexture(gr.imageTexture('textures/apple.png'),
                                                gr.imageTexture('textures/target.png'), 4),
                         {0.7, 0.7, 0.7}, 50)
                         
sandStoneTiled = gr.material(gr.checkerTexture(gr.imageTexture('textures/sandStone.png'),
                                               gr.imageTexture('textures/sandStone.png'), 4),
                         {0.0, 0.0, 0.0}, 50)
                         
gridMap = gr.material(gr.constantTexture({0.738, 0.934, 0.195}),
                               {0.1, 0.1, 0.1}, 50)
gr.setBumpMap(gridMap, gr.imageTexture('textures/gridMap.png'))

gridMapTiled = gr.material(gr.constantTexture({0.738, 0.934, 0.195}),
                               {0.1, 0.1, 0.1}, 50)
gr.setBumpMap(gridMapTiled, gr.checkerTexture(gr.imageTexture('textures/gridMap.png'),
                                              gr.imageTexture('textures/gridMap.png'), 2))
                                              
                                              
checkEmbossedGreenPlastic = gr.material(gr.constantTexture({0.7, 1.0, 0.7}),
                                        {0.1, 0.1, 0.1}, 2)
gr.setBumpMap(checkEmbossedGreenPlastic, 
              gr.checkerTexture(gr.constantTexture({1, 1, 1}), gr.constantTexture({0, 0, 0}), 4))
              
appleTileText = gr.checkerTexture(gr.imageTexture('textures/apple.png'),
                                                gr.constantTexture({1, 1, 1}), 4)
appleEmbossGreenTile = gr.material(gr.constantTexture({0.7, 1.0, 0.7}),
                                    {0.1, 0.1, 0.1}, 2)
gr.setBumpMap(appleEmbossGreenTile, appleTileText)
                                                 
appleEmbossAppleTile = gr.material(appleTileText, {0.1, 0.1, 0.1}, 2)
gr.setBumpMap(appleEmbossAppleTile, gr.checkerTexture(gr.imageTexture('textures/apple.png'),
                                                 gr.constantTexture({1, 1, 1}), 4))


sandStoneBumpped = gr.material(sandStoneText, {0.0, 0.0, 0.0}, 50)
gr.setBumpMap(sandStoneBumpped, gr.imageTexture('textures/sandStoneMap.png'))

sandStoneBumppedTiled = gr.material(gr.checkerTexture(sandStoneText, sandStoneText, 4),
                                    {0.0, 0.0, 0.0}, 50)
gr.setBumpMap(sandStoneBumppedTiled, gr.checkerTexture(gr.imageTexture('textures/sandStoneMap.png'),
                                                       gr.imageTexture('textures/sandStoneMap.png'), 4))
