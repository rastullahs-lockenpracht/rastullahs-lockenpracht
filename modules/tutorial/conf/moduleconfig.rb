include RlScript

class MinidemoModule < ContentModule
    def initialize()
        super("tutorial", "tutorial", false, 200608012)
    end

    def getDependencies()
        return ["common"]
    end

    def getTextureLocations()
	return []
    end

    def getModelLocations()
	return []
    end

    def getSoundLocations()
	return []
    end

    def start()
        $CORE.loadMap("Octree", "DialogLevel.scene", "tutorial", "tutorial.rb");
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(MinidemoModule.new())