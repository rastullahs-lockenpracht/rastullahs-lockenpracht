load "embed.rb"

class SchnelleHeilung < Effect
  def initialize(stufe)
    super(stufe);
    setName("Schnelle Heilung");
    setDescription("Verlorene LE wird schneller regeneriert.");
  end

  def apply()
	  getOwner().getWertStateSet(Creature::WERT_MOD_REGENERATION_LE).modifyModifier(+1);
	  getOwner().getWertStateSet(Creature::WERT_MOD_REGENERATION_LE).modifyProbenModifier(+1);
  end

  def remove()
	  getOwner().getWertStateSet(Creature::WERT_MOD_REGENERATION_LE).modifyModifier(-1);
	  getOwner().getWertStateSet(Creature::WERT_MOD_REGENERATION_LE).modifyProbenModifier(-1);
  end
end
