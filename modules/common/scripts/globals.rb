print( "\n- Registering Globals" );

load "embed.rb"

include RlScript

$CORE = CoreSubsystem.getSingleton();
$AF = ActorManager.getSingleton();
$PM = PhysicsManager.getSingleton();
$GL = GameLoopManager.getSingleton();
$MM = MusicManager.getSingleton();
$UI = UiSubsystem.getSingleton();
$SM = SoundManager.getSingleton();

$AnimMgr = AnimationManager.getSingleton();
