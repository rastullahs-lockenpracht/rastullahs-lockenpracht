class Hand < Container
  def initialize(name, desc, owner, boneslot )
    super(name, desc);
    @mOwner = owner
    @mBoneslot = boneslot
  end
  
  def take( item )
    addItem( item );
    if ( item.getItemType() == 0 )
      @mOwner.addWeapon( item );
    end        
    @mOwner.getActor().attachToSlotAxisRot( item.getActor(), @mBoneslot, "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 1.0, 0.0, 0.0 ], 90.0 );
  end  
  
  def drop( item )
    @mOwner.getActor().detach( item.getActor() );
    @mOwner.removeWeapon( item.getId() );
    removeItem( item.getId() );    
  end
  
  def close ( )
  end
    
end
