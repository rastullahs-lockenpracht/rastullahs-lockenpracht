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

  def startFalling()
	setGravityState(true)
  end
end

class RockPile < GameObject
  def initialize(positionPile, orientation, positionParticles)
    super("Steinhaufen", "Ein großer Steinhaufen")
    @mRockPile = $AM.createMeshActor("Steinhaufen", "Steinhaufen.mesh", PhysicsManager::GT_CONVEXHULL, 0.0)
    @mRockPile.getPhysicalThing().setGravityOverride(true, 0.0, 0.0, 0.0)
    setActor(@mRockPile)
    @mRockPile.placeIntoScene( 
	positionPile[0],
	positionPile[1],
	positionPile[2],
	orientation[0],
	orientation[1],
	orientation[2],
	orientation[3])
    @mPositionPart = positionParticles;
    @mSteinSchlagActor = $AM.createSoundSampleActor("SteinSchlagSound","steinschlag_wenig_zu_vielen.ogg");
    @mSteinSchlagActor.placeIntoScene( 
	positionParticles[0], 
	positionParticles[1], 
	positionParticles[2], 
	1.0, 0.0, 0.0, 0.0 );
  end

  def collapse()
    getActor().getControlledObject().startAnimation("Zusammenfallen", 1.0, 1)
    @mDustCloud = $AM.createParticleSystemActor("Steinstaubwolke", "RL/Staubwolke")
    @mSteinSchlagActor.getControlledObject().play();
    @mDustCloud.placeIntoScene(
	@mPositionPart[0], 
	@mPositionPart[1],
	@mPositionPart[2],
	1.0, 
	0.0, 
	0.0, 
	0.0);
    @mDustCloud.getControlledObject().setActive(true)

    RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleEingang").setState(Quest::COMPLETED)
    RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleZeug").setState(Quest::OPEN)
    RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleZeug").setState(Quest::OPEN)
    RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleZeugFackel").setState(Quest::OPEN)
    RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleZeugHebel").setState(Quest::OPEN)
    RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleZeugTrank").setState(Quest::OPEN)
  end
end

# Stein-Steinhaufen-Kontakt
class RockpileContactListener < PhysicsContactListener
	def initialize(rockPile, rock1, rock2)
		super()
		@mRock1 = rock1
		@mRock2 = rock2
		@mRockPile = rockPile
	end

	def contactOccured(actor1, actor2)
		if (actor2 == @mRock1.getActor() || actor2 == @mRock2.getActor())
			@mRockPile.collapse()
			@mRockPile.getActor().getPhysicalThing().setContactListener(nil)
		end
	end
end

# Zone für Questveränderung und später Steinschlag erstellen. Steinschlagauslösung ist abhängig davon ob das Quest "spinne" schon auf CLOSED steht
print( "GameEvent-Tests wird geladen" );

print( "Definiere SteinschlagzoneListener" );
# Definition des GameAreaListeners
class SteinschlagzoneListener < GameAreaListener
	def initialize(rock1, rock2)
		super()
		@mRock1 = rock1
		@mRock2 = rock2
	end

	def areaLeft(anEvent)
	end

	def areaEntered(anEvent)
		if (RulesSubsystem.getSingleton().getQuestBook().getQuest("spinne").getState() == Quest::CLOSED) 
			
			@mRock1.spawn()
			@mRock1.startFalling()
			@mRock2.spawn()
			@mRock2.startFalling()

			$GameEveMgr.removeAreaListener(self)
		end		
	end
end

