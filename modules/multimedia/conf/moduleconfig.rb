include RlScript

class MultimediaModule < ContentModule
    def initialize()
        super("multimedia", "Multimedia", false, 200603030)
    end

    def getDependencies()
        return ["common"]
    end

    def getTextureLocations()
	    return []
    end

    def getSoundLocations()
        return []
    end

    def getModelLocations()
	    return []
    end

    def start()
        $CORE.loadMap("Octree", "minidemo.scene", "multimedia", "multimedia.rb");
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(MultimediaModule.new())
