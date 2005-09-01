require 'RlRules'
include RlRules

$dsa = DsaManager.new;

class HealAction < Action
   def initialize(name, description, dice)
      super(name, description)
      @dice = dice
   end

   def getTargetClass()
      return 0
   end

   def doAction(object, actor, target)
      lep = 0
      puts lep
      @dice.times {
         lep += $dsa.rollD6
	 puts lep
      }
      target.modifyLe(lep)
   end
end

$action = HealAction.new('Trinken', 'Gibt LeP zurueck.', 2)

$trank = Item.new(1, 'Heiltrank', 'Trinken heilt halt.')
$trank.addAction($action) 

$kraut = Item.new(2, 'Wirselkraut', 'Essen heilt halt.')
$kraut.addAction($action)

$hero = Creature.new(3, 'Alrik', 'Alrik aus Beilunk')
puts $hero.getLe
$trank.doAction('Trinken', $hero, $hero)
puts $hero.getLe

