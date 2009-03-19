include RlScript

class CommonModule < ContentModule
    def initialize()
        super("common", "Common", true, 200603030)
    end

    def getDependencies()
        return []
    end

    def getTextureLocations()
	return ["alpha", "env", "model", "programs", "programs/Skydome", "programs/Normalmapping", "templates", "tiled", "particle"]
    end

    def getSoundLocations()
        return ["mensch", "metall", "natur", "ost", "sonst", "wesen"]
    end

    def getModelLocations()
	return []
    end

    def start()
    # Der GC reisst oft Obejekte ab...
    GC.disable
	UiSubsystem.getSingleton().initializeSubsystem()
    CoreSubsystem.getSingleton() # with these strange things here, globals.rb works

	require "globals.rb"
	require "startup-global.rb"	
    end
end

CoreSubsystem.getSingleton().registerModule(CommonModule.new())
