print( "\n- Registering Globals" );

load "embed.rb"

include RlCore;
include RlSound;

$Core = CoreSubsystem.getSingleton();
$World = $Core.getWorld();
$AF = ActorFactory.getSingleton();
$PM = PhysicsManager.getSingleton();
$GL = GameLoop.getSingleton();
$MM = MusicManager.getSingleton();