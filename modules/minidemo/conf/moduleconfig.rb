include RlScript

class MinidemoModule < ContentModule
    def initialize()
        super("minidemo", "Minidemo", false, 200603030)
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
        $CORE.loadMap("Octree", "minidemo.scene", "minidemo", "minidemo.rb");
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(MinidemoModule.new())