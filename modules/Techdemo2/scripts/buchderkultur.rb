class Lesen < Action
    def initialize()
       super("read", "Lesen")
    end

    def doAction( book, user, target ) 
        book.read
    end

    def canDo(book, user)    
        book.isReadAble( user )
    end
end

class BuckDerKultur < Item
    def initialize(name, beschreibung, model)
        super( name, beschreibung )
        # Buch laden
        setActor( $AM.createMeshActor( name, model ) )

        # Neue Unterhaltung vorladen
        @neueUnterhaltung = $AM.createSoundSampleActor( "unterhaltung2", "bauern_002.ogg" )
        @neueUnterhaltung.getControlledObject().set3d( true )
        @neueUnterhaltung.getControlledObject().setLooping( true )
        @neueUnterhaltung.getControlledObject().load()

        @mWasRead = false
        addAction( Lesen.new )
    end

    def isReadAble( user )
        !@mWasRead
    end

    def read
        # Alte Unterhaltung
        oldActor = $AM.getActor("unterhaltung1")
        oldActor.getControlledObject().stop()
        
        # Neue Unterhaltung starten             
        @neueUnterhaltung.placeIntoScene( oldActor.getPosition() )
        @neueUnterhaltung.getControlledObject().play()
        @neueUnterhaltung.getControlledObject().setVolume( 0.7 )        

        # Alte Unterhaltung löschen
        $AM.destroyActor( oldActor )

        # Buch wurde gelesen
        @mWasRead = true

        # Nachricht
        $WF.showMessageWindow( "Sowas, Bauern haben auch Kultur!" );
    end
end

buchDK = BuckDerKultur.new( "Buch der Kultur", "Ein magisches Buch mit Kultureinfluß", "ver_buch_02.mesh" );
buchDK.getActor( ).placeIntoScene( -35.1, -2.55, 17.04, 1.0, 0.0, 0.0, 0.0 );
