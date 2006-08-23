load "embed.rb"

class AstraleRegeneration < Effect
  def initialize(stufe)
    super(stufe);
    setName("Astrale Regeneration");
    setDescription("Verlorene AE wird schneller regeneriert.");
  end

  def apply()
	  getOwner().getWertStateSet(Creature::WERT_MOD_REGENERATION_AE).modifyModifier(+1);
	  getOwner().getWertStateSet(Creature::WERT_MOD_REGENERATION_AE).modifyProbenModifier(+1);
  end

  def remove()
	  getOwner().getWertStateSet(Creature::WERT_MOD_REGENERATION_AE).modifyModifier(-1);
	  getOwner().getWertStateSet(Creature::WERT_MOD_REGENERATION_AE).modifyProbenModifier(-1);
  end
end
