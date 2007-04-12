require 'testcase.rb'
load "effects/paralues.rb"
load "effects/resistentgegenkrankheiten.rb"
load "effects/wunde.rb"
load "effects/astraleregeneration.rb"


class ParaluesAction < Action
  def initialize
    super("paralues", "Spontan versteinern.");
  end
  
  def canDo(go, user)
    true
  end
  
  def doAction(go, user, target)
     p "Du wirst versteinert.";
     $hero.getGameObject().addEffectWithCheckTime($paralueseffect, 1 * Date::ONE_SPIELRUNDE);
     p "Du solltest dich jetzt eine Weile nicht mehr bewegen können.";
  end
end

class ResistenzAction < Action
  def initialize
    super("resistenz", "Resistenz gegen Krankheiten erweben.");
  end
  
    def canDo(go, user)
    true
    end
  
  def doAction(go, user, target)
     p "KO normal: " 
     p $hero.getGameObject().getEigenschaft("KO");
     p "Du wirst resistent gegen Krankheiten.";
     $hero.getGameObject().addEffect($resistenzeffect);
     p "Effekt angewendet.";
     p "KO gegen Krankheiten: " 
     p $hero.getGameObject().getEigenschaft("KO", Effect::MODTAG_KRANKHEIT);
  end
end


# Test case for weffects. 
class EffectTest < TestCase
    def initialize(center)
        super(center)
	# Define a pointer to the hero
	$h = $UI.getActiveCharacter()
        # Define a base box to place everything else on.
        height = 0.1
        min_base = [-2.0, 0.05, -2.0]
        max_base = [2.0, height, 2.0]
        base = $AM.createBoxPrimitiveActor("EffectTestBase", min_base, max_base,
                                          "alpha_yellow")
        base.placeIntoScene(center);

        bottich = $GOM.createGameObjectProxy("EffectTest").getGameObject();
        bottich.addAction(ParaluesAction.new());
        bottich.addAction(ResistenzAction.new());
        bottich.placeIntoScene();
        bottich.setPosition(rel_pos([0.0, height, 0.0]));


        $paralueseffect = Paralues.new();
        $resistenzeffect = ResistentGegenKrankheiten.new();


        $SCRIPT.log("EffectTest initialisiert.");
    end
end

