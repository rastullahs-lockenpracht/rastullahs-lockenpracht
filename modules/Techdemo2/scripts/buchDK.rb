#buchDK = $AM.createMeshActor("Buch der Kultur", "ver_buch_02.mesh");
#buchDK = Creature.new("Buch der Kultur", "Ein magisches Buch mit Kultureinfluß");
#buchDK.setActor( bowerActor );
#buchDK.placeIntoScene(-35.1, -2.55, 17.04, 1.0, 0.0, 0.0, 0.0);




class Lesen < Action
    def initialize()
       super()
    end

    def getTargetClass
       return TC_SELF;
    end

    def doAction()
	#Bauern2
	#$WF.showMessageWindow( "Auch Bauern können zivilisiert sprechen." );
	$AM.getActor("unterhaltung1").getControlledObject().stop()
	$SCRIPT.log("Unterhaltung2 laden");
	unterhaltung2 = $AM.createSoundSampleActor( "unterhaltung2", "bauern_002.ogg" ); 
	$SCRIPT.log(" Volumen");
	unterhaltung2.getControlledObject().setVolume( 0.5 )
	unterhaltung2.getControlledObject().set3d(true);
	$SCRIPT.log(" Loopen");
	unterhaltung2.getControlledObject().setLooping( true );
	$SCRIPT.log(" Setzen");
	unterhaltung2.placeIntoScene([-27.19, -6.0, -0.89]);
	$SCRIPT.log(" Abspielen");
	unterhaltung2.getControlledObject().play();
	$SCRIPT.log("Unterhaltung2 fertig");
    end
end

class buchDK < Item
  def initialize(name, beschreibung, model)
   super()
   itemActor = $AM.createMeshActor( name, model);
   itemActor = Creature.new("name", "beschreibung");
   setActor(itemActor)

    #addAction(Lesen.new("Lesen", "Lesen"))
  end
end

#buchDK1 = buchDK.new( "Buch der Kultur","Ein magisches Buch mit Kultureinfluß","ver_buch_02.mesh");
#buchDK1.placeIntoScene(-35.1, -2.55, 17.04, 1.0, 0.0, 0.0, 0.0);