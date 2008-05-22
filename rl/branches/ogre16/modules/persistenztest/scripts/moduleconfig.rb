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
		$MAPLOADER = MapLoader.new("persistenztest")
		$MAPLOADER.setDefaultMaps(["persistenztest01.rlmap.xml","hero.rlmap.xml"])
		registerContentLoader($MAPLOADER)
		loadContent();
		load "persistenztest.rb"
    end
end

CoreSubsystem.getSingleton().registerModule(PersistenzTestModule.new())
