include RlScript

class Techdemo2 < ContentModule
    def initialize()
        super("techdemo2", "Techdemo 2", false, 200603030);
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
	return ["ost"]
    end

    def start()
       $CORE.loadMap("Octree", "techdemo2map1_02.scene", "techdemo2");
       load "techdemo2map1.rb"
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(Techdemo2.new());

