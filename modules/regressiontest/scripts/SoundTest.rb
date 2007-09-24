require 'util/vector_util.rb'
require 'testcase.rb'
require 'torch.rb'


class SoundFadeAction < Action
  def initialize
    super("SoundFadeAction", "Tests fading code.")
  end
  
  def canDo(go, user)
    true
  end
  
  def doAction(go, user, target)
    p "SoundFadeAction"
    sound = $SM.createSound("ruchin001.ogg")
    @sound = SoundObject.new(sound, "SoundFadeTest")
    @functor = LinearSoundFadeFunctor.new(120, false)
    @job = SoundFadeJob.new(@sound, @functor, true)
  end
  
end

class SoundTest < TestCase
  include Math
  
  @@torchcount = 10
  @@height = 0.30
  @@radius = 5
  
  def execute()
    min_base = [-@@radius, 0.05, -@@radius]
    max_base = [@@radius, @@height, @@radius]
    base = $AM.createBoxPrimitiveActor("SoundTestBase", min_base, max_base, "alpha_blue")
    base.placeIntoScene(getCenter())
    
    eh = $GOM.createGameObject("SoundFadeTest")
    eh.addAction(SoundFadeAction.new())
    eh.placeIntoScene()
    eh.setPosition(rel_pos([0.0, @@height, 0.0]))
    
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
    
    $SCRIPT.log("SoundTest initialisiert")
    
  end
  
end
