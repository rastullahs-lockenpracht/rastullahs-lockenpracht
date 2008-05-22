require 'testcase.rb'

class WallSet3 < TestCase
  def initialize(center,height,size,wallset)
    super(center)
    
    bname = "WallSet3" + center[0].to_s + center[1].to_s + \
      center[2].to_s + size.to_s
    if wallset[0] == 1
      min_base = [-size, 0.05, -size]
      max_base = [size, height, -(size + 0.3)]
      base = $AM.createBoxPrimitiveActor(bname + "_1",
                                           min_base, max_base, "white")
      base.placeIntoScene(center);
    end

    if wallset[1] == 1
      min_base = [- size, 0.05, size]
      max_base = [size, height, size + 0.3]
      base = $AM.createBoxPrimitiveActor(bname + "_2",
                                           min_base, max_base, "white")
      base.placeIntoScene(center);
    end

    if wallset[2] == 1
      min_base = [- size, 0.05, - size]
      max_base = [- (size +0.3), height, size]
      base = $AM.createBoxPrimitiveActor(bname +"_3",
                                           min_base, max_base, "white")
      base.placeIntoScene(center);
    end

    if wallset[3] == 1
      min_base = [size, 0.05, -size]
      max_base = [size +0.3, height, size]
      base = $AM.createBoxPrimitiveActor(bname + "_4",
                                         min_base, max_base, "white")
      base.placeIntoScene(center);
    end
  end
end
