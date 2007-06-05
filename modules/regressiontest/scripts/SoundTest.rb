require 'util/vector_util.rb'
require 'testcase.rb'
require 'torch.rb'

class SoundTest < TestCase
    include Math

    @@torchcount = 10
    @@height = 0.50
    @@radius = 5
    
    def initialize(center)
        super(center)
  
        min_base = [-@@radius, 0.05, -@@radius]
        max_base = [@@radius, 0.3, @@radius]
        base = $AM.createBoxPrimitiveActor("SoundTestBase",
                                           min_base, max_base, "alpha_blue")
        base.placeIntoScene(center);

        step = 2 * PI / @@torchcount
        0.step(2 * PI, step) do |radian|
            x = cos(radian) * @@radius
            y = sin(radian) * @@radius
            fackel = $GOM.createGameObject("torch")
            fackel.setDescription("Fackel" + radian.to_s)
            fackel.setPosition(rel_pos([x, @@height, y]))
            fackel.setLit(true)
            fackel.placeIntoScene()
        end
        
        $SCRIPT.log("SoundTest initialisiert");
        
    end
  
end
