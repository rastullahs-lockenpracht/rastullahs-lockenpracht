print( "\n- Registering Globals" );

load "embed.rb"

include RlScript

$CORE = CoreSubsystem.getSingleton();
$AM = ActorManager.getSingleton();
$PM = PhysicsManager.getSingleton();
$GL = GameLoopManager.getSingleton();
$UI = UiSubsystem.getSingleton();
$DM = DsaManager.getSingleton();
$DS = DialogSubsystem.getSingleton();
$SCRIPT = ScriptSubsystem.getSingleton();

$GameEveMgr = GameEventManager.getSingleton();
$AnimMgr = AnimationManager.getSingleton();

$VideoDisplay = VideoWindow.new();


class HeroPosWriter
  def initialize()
	
  end
  def writePos()
	actor = $AM.getActor("Held");
	$SCRIPT.log("[ "+actor.getWorldPosition()[0].to_s+", "+actor.getWorldPosition()[1].to_s+", "+actor.getWorldPosition()[2].to_s+"]" );  
	$SCRIPT.log("[ "+actor.getWorldOrientation()[0].to_s+", "+actor.getWorldOrientation()[1].to_s+", "+actor.getWorldOrientation()[2].to_s+", "+actor.getWorldOrientation()[3].to_s+"]") ; 
  end
end

$hpw = HeroPosWriter.new()


$SCRIPT.log("Aktuelle Spielerposition in Log schreiben mit '$hpw.writePos'");
