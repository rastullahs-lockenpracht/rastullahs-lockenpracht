include RlScript

class InIngerimmsNamenModule < ContentModule
    def initialize()
        super("minidemo2", "Minidemo 2", false, 200603030);
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
        $CORE.loadMap("Octree", "testlevel2.scene", "minidemo2", "ruchin.rb");
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(InIngerimmsNamenModule.new());