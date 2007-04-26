require 'testcase.rb'
#require 'math.rb'
require 'torch.rb'

class SoundTest < TestCase
    include Math

    @@torchcount = 10
    @@height = 0.50
    @@radius = 5
    
    def initialize(center)
        super(center)
  
        step = 2 * PI / @@torchcount
        0.step(2 * PI, step) do |radian|
            x = cos(radian) * @@radius
            y = sin(radian) * @@radius
            fackel = $GOM.createGameObject("torch")
            fackel.setDescription("Fackel" + radian.to_s)
            fackel.setPosition(rel_pos([x, @@height, y]))
            fackel.placeIntoScene()
        end
        
        $SCRIPT.log("SoundTest initialisiert");
        
    end
  
end
