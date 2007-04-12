load "embed.rb"

class SchnelleHeilung < Effect
  def initialize(stufe)
    super(stufe);
    setName("Schnelle Heilung");
    setDescription("Verlorene LE wird schneller regeneriert.");
  end

  def getMod(target, type, tag)
	  if ((target == Creature::WERT_REGENERATION) &&
	      (type == Effect::MODTYPE_SUM) &&
	      (tag == Effect::MODTAG_REGENERATION_LE))
		  return stufe;
	  end
	  return 0;
  end
end
