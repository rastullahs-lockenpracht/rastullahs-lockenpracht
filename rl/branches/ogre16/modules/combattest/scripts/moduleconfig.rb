include RlScript

class CombatTestModule < ContentModule
    def initialize()
        super("combattest", "combattest", false, 200108026)
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
        $CORE.loadMap("Octree", "combattest.scene", "combattest")
        $World = $CORE.getWorld();
        load "combattest.rb"
    end
end

CoreSubsystem.getSingleton().registerModule(CombatTestModule.new())
