load "embed.rb"

class AstraleRegeneration < Effect
  def initialize(stufe)
    super(stufe);
    setName("Astrale Regeneration");
    setDescription("Verlorene AE wird schneller regeneriert.");
  end

  def getMod(target, type, tag)
	if ((target == Creature::WERT_REGENERATION) && 
	    (type == MODTYPE_SUM) &&
	    (tag == MODTAG_REGENERATION_AE))
		return stufe;
	else 
		return 0;
	end

  end

  def timeCheck()
	  return PERMANENT;
  end
end
