include RlScript

class TechdemoModule < ContentModule
    def initialize()
        super("techdemo", "Techdemo", false, 200603030);
    end

    def getDependencies()
        return ["common"];
    end

    def getTextureLocations()
	return ["alpha", "env", "model", "tiled"];
    end

    def getSoundLocations()
	return ["ost"];
    end

    def getModelLocations()
	return []
    end

    def start()
        $CORE.loadMap("Octree", "techdemo_terrain.scene", "techdemo");
        load "techdemo.rb"
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(TechdemoModule.new())