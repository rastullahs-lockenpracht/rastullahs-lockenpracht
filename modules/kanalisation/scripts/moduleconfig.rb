include RlScript

class KanalisationModule < ContentModule
    def initialize()
        super("kanalisation", "Kanalisation", false, 200608012)
    end

    def getDependencies()
        return ["common"];
    end

    def getTextureLocations()
	return ["textures"];
    end

    def getModelLocations()
	return ["Model"]
    end

    def getSoundLocations()
	return ["Sound"]
    end

    def start()
        $CORE.loadMap("Octree", "kanalisation.scene", "kanalisation");
        load "kanalisation.rb"
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(KanalisationModule.new());
