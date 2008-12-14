include RlScript


class RuchinModule < ContentModule
    def initialize()
        super("ruchin", "Ruchin (WIP)", false, 200608012)
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
		require 'clothing.rb'
		
		SceneManager::getSingleton().loadScene("ruchin", false);
        load "maptest.rb"
        $World = $CORE.getWorld();
    end
end

CoreSubsystem.getSingleton().registerModule(RuchinModule.new());
