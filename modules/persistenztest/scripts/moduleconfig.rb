include RlScript

class PersistenzTestModule < ContentModule
    def initialize()
        super("persistenztest", "Persistenztest", false, 200603080)
    end

    def getDependencies()
        return ["common"]
    end

    def getTextureLocations()
        return ["textures"]
    end

    def getModelLocations()
        return []
    end

    def getSoundLocations()
        return []
    end

    def start()
        require "mckhero.rb"
        require "hero.rb"
		require "MapChange.rb"
		require "clothing.rb"
		SceneManager::getSingleton().loadScene("scene01", false);
		$PM.setEnabled(true)
    end
end

CoreSubsystem.getSingleton().registerModule(PersistenzTestModule.new())
