# Nur eingebettet starten
load( "embed.rb" );

# print wird auf die Konsole umgeleitet
print( "Teste Animationen" );

# Definition des AnimationsListeners
class UmdrehAbspielListener < AnimationListener
	def animationFinished(anEvent)
		print( "Sowas, schon vorbei - drehen wir doch mal um" );
		anim = anEvent.getAnimation();
		anim.reverseAnimation();
		anim.resetTimesPlayed();
	end
	def animationPaused(anEvent)
	end
	def animationUnpaused(anEvent)
	end
end
# Definition des AnimationsFrameListeners
class MachWasBeiFrameXListener < AnimationFrameListener
	def animationFrameReached(anEvent)
		print( "Sowas, schon wieder die Mitte erreicht" );
	end
end


# Erstellen eines laufenden Roboters
actBotter = $AM.createMeshActor("Botter","robot.mesh");
actBotter.placeIntoScene( 160.0, 24.0, 160.0, 1.0, 0.0, 0.0, 0.0 );
boBotter = actBotter.getControlledObject();
# Animation "Walk" starten
boBotter.startAnimation( "Walk" );

# Die Laufende Animation "Walk" holen 
anim = boBotter.getAnimation( "Walk" );
# Geschwindigkeit setzen
anim.setSpeed( 4.0 );


# Erzeugen einer Fackel
fackel = $AM.createMeshActor("Fackel","fackel.mesh");
fackel.placeIntoScene( 100.0, 16.0, 100.0, 1.0, 0.0, 0.0, 0.0 );


# Erstellen eines einfachen Tracks  
#	- für die fackel 
#	- mit dem _EINZIGARTIGEN_ Namen "testTrackAnimation"
#	- Und der Länge von 4 Sekunden
trackAnim = $AnimMgr.createTrackAnimation( fackel, "testTrackAnimation", 4.0 );
# Setzen der ersten Keyframe
trackAnim.addKeyFrame( 0.0 );
trackAnim.setKeyFrameRotation( 0.0, 0.0, 0.0, 0.0, 0.0 );
trackAnim.setKeyFrameTranslation( 0.0, 0.0, 0.0, 0.0 );
# Setzen der zweiten Keyframe
trackAnim.addKeyFrame( 2.0 );
trackAnim.setKeyFrameRotation( 2.0, 1.0, 0.0, 0.0, 90.0 );
trackAnim.setKeyFrameTranslation( 2.0, 100.0, 0.0, 0.0 );
# Setzen der dritten Keyframe
trackAnim.addKeyFrame( 4.0 );
trackAnim.setKeyFrameRotation( 4.0, 0.0, 0.0, 0.0, 0.0 );
trackAnim.setKeyFrameTranslation( 4.0, 0.0, 0.0, 0.0 );

# AnimationFrameListener hinzufügen
animFrameListener = MachWasBeiFrameXListener.new();
trackAnim.addAnimationFrameListener( animFrameListener, 2.0 );
# Und los gehts
trackAnim.setPaused( false );


# Noch einfacherer ;) Track, aber mit nem Listener verknüpft
tischlein = $AM.createMeshActor("TavernenTisch","tisch_taverne.mesh");
tischlein.placeIntoScene( 160.0, 24.0, 160.0, 1.0, 0.0, 0.0, 0.0 );

listenedTrackAnim = $AnimMgr.createTrackAnimation( tischlein, "testListenerTrackAnimation", 6.0 );
listenedTrackAnim.addKeyFrame( 0.0 );
listenedTrackAnim.setKeyFrameTranslation( 0.0, 0.0, 0.0, 0.0 );
listenedTrackAnim.addKeyFrame( 6.0 );
listenedTrackAnim.setKeyFrameTranslation( 1.0, 0.0, 100.0, 0.0 );
# Begrenzte Abspielanzahl setzen
listenedTrackAnim.setTimesToPlay( 1 );
listenedTrackAnim.setSpeed( 2.0 );
# AnimationsListener erzeugen
animListener = UmdrehAbspielListener.new();
# AnimationsListener hinzufuegen
listenedTrackAnim.addAnimationListener( animListener );
# Und los gehts
listenedTrackAnim.setPaused( false );