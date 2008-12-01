include RlScript

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
		SceneManager::getSingleton().loadScene("intro", false);
        #$CORE.loadMap("Octree", "intro.scene", "General");
        load "intro.rb"
        $SCRIPT.log("done")
        

        $SCRIPT.log("showMainMenu()..")
        $WF.showMainMenu()
        $SCRIPT.log("done.")
    end
end

CoreSubsystem.getSingleton().registerModule(IntroModule.new())
