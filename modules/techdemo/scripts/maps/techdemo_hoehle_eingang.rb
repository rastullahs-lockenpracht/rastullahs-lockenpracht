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
