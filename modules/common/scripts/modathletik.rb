class ModAthletik < Effect
  def initialize(modifier)
    super();
    @mModifier = modifier;
  end

  def isAlive()
	if (getTimePassed > 1000) return false;
	return true;
  end

  def apply()
	  getOwner().getTalentStateSet("Athletik").setModifier(@mModifier);
  end

  def remove()
	  getOwner().getTalentStateSet("Athletik").setModifier(0);
  end
end
