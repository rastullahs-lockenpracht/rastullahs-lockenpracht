class Hero < Person
    def initialize
        super(10000, "Alrik", "Der Testheld");
        setActor($AF.createMeshActor("Held","held.mesh", 2, 2.0));
    end
end

