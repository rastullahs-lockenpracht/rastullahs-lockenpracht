class Paralues < Effect
  def initialize(stufe)
    super(0);
    setName("Versteinert");
    setDescription("Die Haut sieht aus wie aus feinem Marmor.");
  end

  def getLifeState()
    rval = (LS_PETRIFIED | LS_INVULNERABLE | LS_DEAF);
    return rval;
  end
    
  def timeCheck()
    p "Paralues check called";
    return REMOVE;
  end
end
