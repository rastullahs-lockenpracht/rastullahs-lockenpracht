

fontfiles = fonts/maiden.ttf fonts/console.fontdef fonts/fonts.zip fonts/sample.fontdef fonts/solo5.ttf fonts/trebucbd.ttf
guifiles = gui/console.overlay gui/console_2.png gui/ogreborder.png gui/ogrebordercenter.png gui/console.material \
    gui/console.material gui/console_1.png gui/cursor.png gui/ogreborderUp.png
mapsfiles = maps/.keepme
modelsfiles = models/.keepme
scriptfiles = scripts/.keepme
soundfiles = sound/.keepme
conffiles = conf/plugins.cfg

install-data-local:
	install -d ${pkgdatadir}/media 
	install -m 644 OgreCore.zip ${pkgdatadir}/media/OgreCore.zip
	install -m 644 ${fontfiles} ${pkgdatadir}/media
	install -m 644 ${guifiles} ${pkgdatadir}/media
	install -m 644 ${mapsfiles} ${pkgdatadir}/media
	install -m 644 ${modelsfiles} ${pkgdatadir}/media
	install -m 644 ${scriptfiles} ${pkgdatadir}/media
	install -m 644 ${soundfiles} ${pkgdatadir}/media
	install -d /etc/rl
	install -m 644 ${conffiles} /etc/rl
	
