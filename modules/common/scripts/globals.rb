load "embed.rb"

print( "\n- Registering Globals" );

include RlScript

# Der GC reisst oft die Objekte ab
GC.disable

$CORE = CoreSubsystem.getSingleton();
$AM = ActorManager.getSingleton();
$PM = PhysicsManager.getSingleton();
$GL = GameLoopManager.getSingleton();
$UI = UiSubsystem.getSingleton();
$DM = DsaManager.getSingleton();
$DS = DialogSubsystem.getSingleton();
$WM = WindowManager.getSingleton();
$SCRIPT = ScriptSubsystem.getSingleton();

$GameEveMgr = GameEventManager.getSingleton();
$AnimMgr = AnimationManager.getSingleton();

if ($VideoDisplay == nil)
    $VideoDisplay = VideoWindow.new();
end


class HeroPosWriter
  def initialize()
	
  end
  def writePos()
	actor = $UI.getActiveCharacter().getActor();
	$SCRIPT.log("[ "+actor.getWorldPosition()[0].to_s+", "+actor.getWorldPosition()[1].to_s+", "+actor.getWorldPosition()[2].to_s+"]" );  
	$SCRIPT.log("[ "+actor.getWorldOrientation()[0].to_s+", "+actor.getWorldOrientation()[1].to_s+", "+actor.getWorldOrientation()[2].to_s+", "+actor.getWorldOrientation()[3].to_s+"]") ; 
  end
end

$hpw = HeroPosWriter.new()


p "Aktuelle Spielerposition in Log schreiben mit '$hpw.writePos'"

require( "questsound.rb" ); 
