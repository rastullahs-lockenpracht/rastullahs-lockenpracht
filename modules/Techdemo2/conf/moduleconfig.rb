include RlScript

class Techdemo2 < ContentModule
    def initialize()
        super("Techdemo2", "Techdemo 2", false, 200603030);
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
	return ["sound","ost"]
    end

    def start()
       $CORE.loadMap("Octree", "techdemo2map1_02.scene", "Techdemo2", "Techdemo2map1.rb");
#	$CORE.loadMap("Octree", "techdemo2map2_01.scene", "Techdemo2", "Techdemo2map2.rb");
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(Techdemo2.new());

