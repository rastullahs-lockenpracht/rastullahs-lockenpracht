print( "\n- Registering Globals" );

load "embed.rb"

include RlScript

$CORE = CoreSubsystem.getSingleton();
$AF = ActorManager.getSingleton();
$AM = ActorManager.getSingleton();
$PM = PhysicsManager.getSingleton();
$GL = GameLoopManager.getSingleton();
$MM = MusicManager.getSingleton();
$UI = UiSubsystem.getSingleton();
$SM = SoundManager.getSingleton();

$GamEveMgr = GameManager.getSingleton();
$AnimMgr = AnimationManager.getSingleton();




class HeroPosWriter
  def initialize()
	
  end
  def writePos()
	actor = $AM.getActor("Held");
	$CORE.log("[ "+actor.getWorldPosition()[0].to_s+", "+actor.getWorldPosition()[1].to_s+", "+actor.getWorldPosition()[2].to_s+"]" );  
	$CORE.log("[ "+actor.getWorldOrientation()[0].to_s+", "+actor.getWorldOrientation()[1].to_s+", "+actor.getWorldOrientation()[2].to_s+", "+actor.getWorldOrientation()[3].to_s+"]") ; 
  end
end

$hpw = HeroPosWriter.new()


$CORE.log("Aktuelle Spielerposition in Log schreiben mit '$hpw.writePos'");