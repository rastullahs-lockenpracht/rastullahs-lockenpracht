class Rock < GameObject
  def initialize(name, model, position, orientation, spawned)
    super(name, "Ein Fels")
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
      rockActor = $AM.createMeshActor( getName(), @mModel, PhysicsManager::GT_ELLIPSOID, 3000.0)
      rockActor.getPhysicalThing().setGravityOverride(true)
      setActor(rockActor)

      rockActor.placeIntoScene(@mPosition, @mOrientation)

      @mSpawned = true
      setHighlightingEnabled(false)
    end
  end

  def setGravityState(active)
    if (active)
      getActor().getPhysicalThing().setGravityOverride(true, 0.0, -98.800, 0.0)
    else
      getActor().getPhysicalThing().setGravityOverride(true, 0.0, 0.0, 0.0)
    end
    getActor().getPhysicalThing().unfreeze();
  end

  def startFalling()
	setGravityState(true)
  end
end

class ZusammenfallListener < AnimationListener
	def initialize(rockPile,dustCloud)
		super()
		@mDustCloud = dustCloud
		@mRockPile = rockPile
	end

	def animationFinished(anEvent)
		@mDustCloud.getControlledObject().setActive(false)
		$AM.destroyActor( @mDustCloud );
		@mRockPile.getPhysicalThing().updateCollisionHull();
	end
	
	def animationPaused(anEvent)

	end
	
	def animationUnpaused(anEvent)
	end
end


class RockPile < GameObject
  def initialize(positionPile, orientation, positionParticles)
    super("Steinhaufen", "Ein großer Steinhaufen")

    @mRockPile = $AM.createMeshActor("Steinhaufen", "Steinhaufen.mesh", PhysicsManager::GT_CONVEXHULL, 0.0)
    @mRockPile.getPhysicalThing().setGravityOverride(true, 0.0, 0.0, 0.0)
    @mRockPile.placeIntoScene(positionPile, orientation)

    @mPositionPart = positionParticles;
    @mSteinSchlagSound = $AM.createSoundSampleActor("SteinSchlagSound","steinschlag_wenig_zu_vielen.ogg");
    @mSteinSchlagSound.getControlledObject().load();
    @mSteinSchlagSound.placeIntoScene( 	positionParticles[0], positionParticles[1], positionParticles[2], 1.0, 0.0, 0.0, 0.0 );

    setActor(@mRockPile)
    setHighlightingEnabled(false)
  end

  def collapse()
    @mDustCloud = $AM.createParticleSystemActor("Steinstaubwolke", "Techdemo/Steinstaubwolke")
    @mSteinSchlagSound.getControlledObject().play();
    @mDustCloud.placeIntoScene(@mPositionPart);
    @mDustCloud.getControlledObject().setActive(true)

    fallAnim = getActor().getControlledObject().startAnimation("Zusammenfallen", 0.5, 1);  
    fallAnim.addAnimationListener( ZusammenfallListener.new(@mRockPile,@mDustCloud) );

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
	def initialize(rockPile, rock1, rock2, rock3)
		super()
		@mRockActor1 = rock1.getActor()
		@mRockActor2 = rock2.getActor()
		@mRockActor3 = rock3.getActor()
		@mRockPile = rockPile
	end

	def contactOccured(actor1, actor2)
		if (actor2 == @mRockActor1 || actor2 == @mRockActor2 || actor2 == @mRockActor3 )
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
	def initialize(rock1, rock2, rock3)
		super()
		@mRock1 = rock1
		@mRock1.spawn()
		@mRock1.getActor().setVisible(false);
		@mRock2 = rock2
		@mRock2.spawn()
		@mRock2.getActor().setVisible(false);
		@mRock3 = rock3
		@mRock3.spawn()
		@mRock3.getActor().setVisible(false);
	end

	def areaLeft(anEvent)
	end

	def areaEntered(anEvent)
		if (RulesSubsystem.getSingleton().getQuestBook().getQuest("spinne").getState() == Quest::CLOSED) 
			@mRock1.getActor().setVisible(true);
			@mRock2.getActor().setVisible(true);
			@mRock3.getActor().setVisible(true);
			@mRock1.startFalling()
			@mRock2.startFalling()		
			@mRock3.startFalling()	
			$GameEveMgr.removeAreaListener(self)
		end		
	end
end

