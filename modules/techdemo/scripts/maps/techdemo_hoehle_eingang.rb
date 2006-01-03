class Rock < GameObject
  def initialize(model, position, orientation, spawned)
    super("Fels", "Ein Fels")
    @mPosition = position
    @mOrientation = orientation
    @mModel = model
    @mSpawned = false

    if (spawned)
      spawn()
    end
  end

  def spawn
    if ( ! @mSpawned )
      rockActor = $AM.createMeshActor(getName(), @mModel, PhysicsManager::GT_ELLIPSOID, 2500.0)
      rockActor.getPhysicalThing().setGravityOverride(true)
      setActor(rockActor)

      rockActor.placeIntoScene( 
	@mPosition[0],
	@mPosition[1],
	@mPosition[2],
	@mOrientation[0],
	@mOrientation[1],
	@mOrientation[2],
	@mOrientation[3])

      @mSpawned = true
    end
  end

  def setGravityState(active)
    if (active)
      getActor().getPhysicalThing().setGravityOverride(true, 0.0, -9880.0, 0.0)
    else
      getActor().getPhysicalThing().setGravityOverride(true, 0.0, 0.0, 0.0)
    end
    getActor().getPhysicalThing().unfreeze();
  end
end

class RockManager
  def initialize
    @mRocks = Array.new()
  end

  def add(rock)
    @mRocks.push(rock)
  end

  def startFalling
    @mRocks.each { |rock| rock.setGravityState(true) }
  end

  def spawn
    @mRocks.each { |rock| rock.spawn() }
  end
end


# Zone für Questveränderung und später Steinschlag erstellen. Steinschlagauslösung ist abhängig davon ob das Quest "spinne" schon auf COMPLETED steht

print( "GameEvent-Tests wird geladen" );

print( "Definiere SteinschlagzoneListener" );
# Definition des GameAreaListeners
class SteinschlagzoneListener < GameAreaListener
	def areaLeft(anEvent)
		print( "Raus - " +  anEvent.getProvokingActor().getName() );
		areaListener = $GameEveMgr.removeAreaListener(@self)
	end
	def areaEntered(anEvent)
		print( "Rein - " + anEvent.getProvokingActor().getName() );
		RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleEingang").setState(Quest::COMPLETED)
		RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleZeug").setState(Quest::OPEN)
	end
end

held = $AM.getActor( "Held" );
held.setQueryMask( Actor::QGF_PLAYER );


$SCRIPT.log("Kugel-Zentrum Actor erstellen");
kugelDings = $AM.createEmptyActor( "Kugel-Zentrum" );
$SCRIPT.log("Kugel-Zentrum Actor in die Szene einfügen");
kugelDings.placeIntoScene( 7817.69, 1013.17, 5093.91, 1.0, 0.0, 0.0, 0.0);

$SCRIPT.log("SteinschlagzoneListener erstellen");
areaListener = SteinschlagzoneListener.new();

$SCRIPT.log("SteinschlagzoneListener hinzufügen");
$GameEveMgr.addSphereAreaListener( kugelDings, 50.0, areaListener, Actor::QGF_PLAYER );