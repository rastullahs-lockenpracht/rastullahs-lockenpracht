#<!-- Script f�rs Schwert mit Ger�usch und Musikwechsel auf Kampf! kampf.rb-->

#Musik zum Kampf wechseln

	$AM.getActor("techdemoDruidenWald").getControlledObject().stop()
	$AM.getActor("kampfMusik").getControlledObject().play()

#Ger�usch
	@mSchwertziehenSound = $AM.createSoundSampleActor("Schwertziehen","schwert.ogg");
	@mSchwertziehenSound.getControlledObject().set3d(false);
	@mSchwertziehenSound.getControlledObject().load();
	@mSchwertziehenSound.getControlledObject().play();

#schwert ziehen
	class Kurzschwert < Weapon 
	 def initialize() 
	   super("Kurzschwert", "Ein kurzes, gerades Schwert"); 

	   setTp(1, 0, 2); 
	   setTpKk(11, 4); 
	   setBf(3); 
	   setIni(-1); 
	   setDk(Weapon::DK_N); 
	   setKampftechnik("Schwerter"); 

	   setActor($AM.createMeshActor("kurzschwert", "waf_kurzschwert_01.mesh")); 
	 end 
	end

	held = $AM.getActor("Held");
	schwertH = Kurzschwert.new()
	held.addWeapon(schwertH)