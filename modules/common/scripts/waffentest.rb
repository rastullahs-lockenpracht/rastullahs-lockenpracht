load "hand.rb"

$hand = Hand.new("Hand","Alriks rechte Hand", $hero, "Bone15");
name = "Knüppel";
$waffe = Weapon.new(name, "Ein einfacher Holzknüppel");
waffenActor = $AM.createMeshActor( name, "ins_fackel_01.mesh" );
$waffe.setActor(waffenActor);
$waffe.setKampftechnik( "Hiebwaffen" );

$hand.take( $waffe );
