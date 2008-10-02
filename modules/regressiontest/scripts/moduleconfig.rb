include RlScript

class RegressionTestModule < ContentModule
    def initialize()
        super("regressiontest", "regressiontest", false, 200608026)
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
        require "truhe.rb"
        MapLoader.new("regressiontest").loadMap("regressiontest.scene")
        load "regressiontest.rb"
    end
end

CoreSubsystem.getSingleton().registerModule(RegressionTestModule.new())
