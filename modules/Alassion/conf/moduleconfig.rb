include RlScript

class Alassion < ContentModule
    def initialize()
        super("Alassion", "Alassion", false, 200603030);
    end

    def getDependencies()
        return ["common"];
    end

    def getTextureLocations()
	return ["model","tiled","alpha"];
    end

    def start()
        $CORE.loadMap("Octree", "alassion.scene", "Alassion", "Alassion.rb");
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(Alassion.new());