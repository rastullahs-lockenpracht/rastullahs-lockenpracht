
conffiles = conf/adventure.xml conf/media.cfg conf/resources.cfg conf/testmap.xml
fontfiles = fonts/.keepme
guifiles = gui/.keepme
mapsfiles = maps/testraum.bsp maps/testraum.zip
modelsfiles = models/cube.mesh models/flare.png models/flaretrail.png models/r2skin.jpg models/robot.material \
    models/robot.mesh models/robot.skeleton models/sphere.mesh models/test.particle models/testraum.material \
    models/tisch_taverne.material models/tisch_taverne.mesh models/tisch_taverne.png
scriptfiles = scripts/physik.rb scripts/simple.rb scripts/sound.rb scripts/test.rb
soundfiles = sound/testsound.wav

install-data-local:
	install -d ${pkgdatadir}/media 
#	install -m 644 ${fontfiles} ${pkgdatadir}/media
#	install -m 644 ${guifiles} ${pkgdatadir}/media
	install -m 644 ${mapsfiles} ${pkgdatadir}/media
	install -m 644 ${modelsfiles} ${pkgdatadir}/media
	install -m 644 ${scriptfiles} ${pkgdatadir}/media
	install -m 644 ${soundfiles} ${pkgdatadir}/media
	install -d /etc/rl
	install -m 644 ${conffiles} /etc/rl
	
