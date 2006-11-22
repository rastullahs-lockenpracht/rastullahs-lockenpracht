include RlScript

class CommonModule < ContentModule
    def initialize()
        super("common", "Common", true, 200603030)
    end

    def getDependencies()
        return []
    end

    def getTextureLocations()
	return ["alpha", "env", "model", "programs", "templates", "tiled", "particle"]
    end

    def getSoundLocations()
        return ["mensch", "metall", "natur", "ost", "sonst", "wesen"]
    end

    def getModelLocations()
	return []
    end

    def start()
	UiSubsystem.getSingleton().initializeSubsystem()

	load "globals.rb"
	load "startup-global.rb"	
    end
end

CoreSubsystem.getSingleton().registerModule(CommonModule.new())