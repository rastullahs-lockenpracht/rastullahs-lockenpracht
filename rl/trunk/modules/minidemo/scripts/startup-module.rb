$Core.loadMap("Octree", "b_1.mesh", "testraum.rb");

$song = $SM.getByName("Ruchin-Thema.ogg");
if $song != nil 
	$song.play();
end;
$World = $Core.getWorld();

$SM.printfme();