include RlScript

class InIngerimmsNamenModule < ContentModule
    def initialize()
        super("InIngerimmsNamen", "In Ingerimms Namen", false, 200603030);
    end

    def getDependencies()
        return ["common"];
    end

    def getTextureLocations()
	return ["textures"];
    end

    def getModelLocations()
	return []
    end

    def getSoundLocations()
	return []
    end

    def start()
        $CORE.loadMap("Octree", "testlevel2.scene", "InIngerimmsNamen", "ruchin.rb");
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(InIngerimmsNamenModule.new());