class ResistentGegenKrankheiten < Effect
  def initialize()
    super(0);
    setName("Resistent gegen Krankheiten");
    setDescription("Der Character ist sehr Widerstandskräftig gegen Krankheiten.");
  end

  def getMod(target, type, tag)
    if ((target == "KO") && (type == MODTYPE_WERTMOD) && (tag == MODTAG_KRANKHEIT))
      return 3;
    else
      return 0;
    end
  end

  def check()
    p "Resistent gegen Krankheiten check called";
    return PERMANENT;
  end
end
