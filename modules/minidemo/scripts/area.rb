# Nur eingebettet starten
load( "embed.rb" );
require( "areahelper.rb" )

print( "Area-Tests wird geladen" );

$AM.getActor( "Held" ).setQueryMask( RlScript::QUERYFLAG_PLAYER )

kugelDings = $AM.createSoundSampleActor( "Kugel-Zentrum", "lachen.ogg" )
kugelDings.placeIntoScene( [4.150, 0.5, -3.00] )

# Will be executed only once
AreaHelper::createLimitedTimesSphereArea( kugelDings, 0.5, RlScript::QUERYFLAG_PLAYER, 
    Proc.new { |e| e.getSource().getActor().getControlledObject().play(); $WF.showMessageWindow( "Das ist die dunkle Ecke" ); } )

print( "Area-Tests fertig geladen" );