require "testcase.rb"

class CharacterSelectionTest < TestCase
    def execute()
        wolf = $GOM.createGameObject("Wolf");
        wolf.setPosition(getCenter());
        wolf.placeIntoScene();
        PartyManager.getSingleton().addCharacter(wolf);
    end
end
