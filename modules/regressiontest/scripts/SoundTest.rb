require 'jobs/jobsequence.rb'
require 'util/vector_util.rb'
require 'testcase.rb'
require 'torch.rb'
require 'switch.rb'

class SwitchChanger < ObjectStateChangeListener

  def initialize(switch)
    super()
    @switch = switch
    @switch.addObjectStateChangeListener(self)
  end

  def objectStateChanged(event)
    state = @switch.getState();
    doFadeTest() if state == Switch3Way::STATE_OBEN
    doStitchingTest() if state == Switch3Way::STATE_UNTEN
  end
  
  def doFadeTest
    p "SoundFadeAction"
    sound = $SM.createSound("ruchin001.ogg")
    soundobject = SoundObject.new(sound, "SoundFadeTest")
    soundobject.set3d(false);
    soundobject.play();
    functor = LinearSoundFadeFunctor.new(5, false)
    job = SoundFadeJob.new(soundobject, functor, true)
    jobSequence([5, job])
  end
  
  def doStitchingTest
    p "SoundStitchingAction"
    stitching = Stitching
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

    eh = $GOM.createGameObject("switch3way")
    eh.placeIntoScene()
    eh.setPosition(rel_pos([0.0, @@height, 0.0]))
    SwitchChanger.new(eh)
    
    step = 2 * PI / @@torchcount
    0.step(2 * PI, step) do |radian|
      x = cos(radian) * @@radius
      y = sin(radian) * @@radius
      fackel = $GOM.createGameObject("torch")
      fackel.setDescription("Fackel" + radian.to_s)
      fackel.setPosition(rel_pos([x, @@height, y]))
      fackel.setLit(false)
      fackel.placeIntoScene()
    end
    
    $SCRIPT.log("SoundTest initialisiert")
    
  end
  
end
