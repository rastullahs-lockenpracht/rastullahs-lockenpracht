class ModAthletik < Effect
  def initialize(modifier)
    super(1);
    @mModifier = modifier;
  end

  def isAlive()
	return getTimePassed <= 1000;
  end

  def apply()
	  getOwner().getTalentStateSet("Athletik").setModifier(@mModifier);
  end

  def remove()
	  getOwner().getTalentStateSet("Athletik").setModifier(0);
  end
end
