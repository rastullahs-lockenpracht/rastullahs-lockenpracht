print( "\n- Registering Globals" );

load "embed.rb"

include RlScript

$Core = CoreSubsystem.getSingleton();
$World = $Core.getWorld();
$AF = ActorManager.getSingleton();
$PM = PhysicsManager.getSingleton();
$GL = GameLoop.getSingleton();
$MM = MusicManager.getSingleton();
$Ui = UiSubsystem.getSingleton();

load "playercommands.rb"
