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

    def start()
    end
end

CoreSubsystem.getSingleton().registerModule(CommonModule.new())