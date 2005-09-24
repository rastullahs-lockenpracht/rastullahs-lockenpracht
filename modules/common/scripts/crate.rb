require 'globals.rb'

class Crate < GameObject
  def initialize(name, description)
    super(name, description);

    crateActor = $AM.createMeshActor( "crate", "kiste.mesh",  PhysicsManager::GT_BOX, 120.0 );
    $CORE.log("crate.rb - Aktor erstellt.");
    setActor(crateActor);
    $CORE.log("crate.rb - Aktor gesetzt");
  end
end

