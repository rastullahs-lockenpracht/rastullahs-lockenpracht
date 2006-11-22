class IntroModule < ContentModule
    def initialize()
        super("intro", "Intro", false, 200603030)
    end

    def getDependencies()
        return ["common"]
    end

    def getTextureLocations()
	return []
    end

    def getModelLocations()
	return []
    end

    def getSoundLocations()
	return []
    end

    def start()
        $SCRIPT.log("Loading menu background..")
        $CORE.loadMap("Octree", "intro.scene", "General", "intro.rb");
        $SCRIPT.log("done")

        $SCRIPT.log("showMainMenu()..")
        $WF.showMainMenu()
        $SCRIPT.log("done.")
    end
end

CoreSubsystem.getSingleton().registerModule(IntroModule.new())