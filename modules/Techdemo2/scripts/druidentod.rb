#<!-- Script um Druiden mit Animationen sterben zu lassen, Physikschubs, als Dialogpartner deaktivieren. druidentod.rb-->


#Geräusch
	@mTodSound = $AM.createSoundSampleActor("TodSound","altermann005.ogg");
	@mTodSound.getControlledObject().set3d(false);
	@mTodSound.getControlledObject().load();
	@mTodSound.getControlledObject().play();

#Masse aktivieren
$AM.getActor("Alter Mann").getPhysicalThing().setMass(900.0)



#Druideanimation
$AM.getActor("Alter Mann").getControlledObject().startAnimation("kampf_schwerter_getroffen", 0.8, 1); 
$AM.getActor("Alter Mann").getControlledObject().startAnimation("reden", 3.0, 7); 

	#Physikstoß an den Druiden
	$AM.getActor("Alter Mann").getPhysicalThing().unfreeze();
	$AM.getActor("Alter Mann").getPhysicalThing().addForce([30000.0, 5000.0, 65000.0]);

	startani = $AM.getActor("Alter Mann").getControlledObject().getAnimation("kampf_sturz")
	startani.resetTimesPlayed( )
	startani.setPaused( false )
	startani.setTimesToPlay( 1 )
	endani = $hero.getActor().getControlledObject().getAnimation("idle")
	 
	fade = $AnimMgr.fadeAnimation( startani, endani, 1.0 )
	fade.setDeleteOnFinish( true )




#Schwert wegstecken
	$AM.getActor("Held").getGameObject().detachWeapon();  

#Tagebucheinträge bei Tötung einstellen
	questBook = RulesSubsystem.getSingleton().getQuestBook()
			questBook.addJournalEntry("Der alte Mann aus dem Wald II",
			"Geschafft! Als ich auf ihn traf setzte er sich zur Wehr und ich mußte in kampfunfähig machen. So wird er zumindest niemanden mehr belästigen. Ich denke, nun wird sich der Wald, und damit auch die Wölfe, wieder beruhigen.");

#Endsequenz
	load("sonja2.rb");