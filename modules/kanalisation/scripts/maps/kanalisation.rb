load "embed.rb"
$World = $CORE.getWorld()
require 'player.rb'
require 'mckhero.rb'
require 'torch.rb'


# Physik aktivieren
$PM.setEnabled(true);

#$SCRIPT.log("Tageslicht erstellen..");
#sunlight = $AM.createLightActor("sunlight", LightObject::LT_DIRECTIONAL);
#sunlight.getControlledObject().setCastShadows(true);
#sunlight.getControlledObject().setSpecularColour(1.0,1.0,1.0);
#sunlight.getControlledObject().setDiffuseColour(0.1,0.1,0.2);
#$SCRIPT.log("Tageslicht erstellt.");

#backlight = $AM.createLightActor("backlight", LightObject::LT_DIRECTIONAL);
#backlight.getControlledObject().setDirection(0.17101, 0.873647, -0.969846);
#backlight.getControlledObject().setCastShadows(false);
#backlight.getControlledObject().setDiffuseColour(-0.3,-0.3,-0.3);

$World.setFog( World::FOG_EXP, [0.658,0.7568,0.9137,1.0], 0.00049, 0.8, 1.0);
$World.setAmbientLight( 0.08, 0.06, 0.05  );

$SCRIPT.log("Sound laden");
kanalLied = $AM.createSoundStreamActor( "KanalLied", "kanal.ogg" ); 
kanalLied.getControlledObject().set3d(false);
$SCRIPT.log(" Loopen");
kanalLied.getControlledObject().setLooping( true );
$SCRIPT.log(" Abspielen");
kanalLied.getControlledObject().play();
$SCRIPT.log("Sound fertig");

#$SCRIPT.log("Initialize hero");
#$hero = $GOM.createGameObject("mckhero", "hero");
#$hero.placeIntoScene();
#$hero.setPosition([ -35.15, 2.69, 26.09]);
#$hero.setOrientation([ [ -4.43614335381426e-005, -0.999999940395355, -3.53998184436932e-005 ] , 67.3891525268555]);
#PlayerSettings.preparePlayer($hero);
#PartyManager.getSingleton().setActiveCharacter($hero)

#$SCRIPT.log("Hero ready")

#$SCRIPT.log("Fackellicht erstellen..");
#fackellicht = $AM.createLightActor("Das Licht der Fackel", LightObject::LT_POINT );
#fackellicht.getControlledObject().setCastShadows(true);
#fackellicht.getControlledObject().setDiffuseColour(0.3,0.2,0.1);
#fackellicht.getControlledObject().setAttenuation(2.0, 0.79,  -0.21, 0.268 );
#$hero.getActor().attachToSlot( fackellicht, "Bip01 R SlotHand" );
#$SCRIPT.log("Fackellicht erstellt.");

load "particle.rb"

