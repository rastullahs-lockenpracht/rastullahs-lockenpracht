print( "\n- Registering Globals" );

load "embed.rb"

include RlScript

$Core = CoreSubsystem.getSingleton();
$AF = ActorManager.getSingleton();
$PM = PhysicsManager.getSingleton();
$GL = GameLoop.getSingleton();
$MM = MusicManager.getSingleton();
$SM = SoundManager.getSingleton();
$Ui = UiSubsystem.getSingleton();

