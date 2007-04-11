class Paralues < Effect
  def initialize()
    super(0);
    setName("Versteinert");
    setDescription("Die Haut sieht aus wie aus feinem Marmor.");
  end

  def getStatus()
    rval = (STATUS_PETRIFIED | STATUS_INVULNERABLE | STATUS_DEAF);
    return rval;
  end
    
  def check()
    p "Paralues check called";
    return REMOVE;
  end
end
