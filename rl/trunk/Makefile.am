SUBDIRS =  engine modules tools docs

install:
	cd docs; ${MAKE} install
	cd engine; ${MAKE} install
	cd modules; ${MAKE} install
	cd tools; ${MAKE} install

doc:
	cd docs; ${MAKE} doc
	
tests:
	cd engine; ${MAKE} tests

install-data-local:
	install -d /etc/rl
	install -m 644 rl.conf /etc/rl
	
nobase_dist_pkgdata_DATA = \
	modules/modules.cfg \
	modules/common/OgreCore.zip \
	modules/common/conf/moduleconfig.cfg \
	modules/common/conf/plugins-linux.cfg \
	modules/common/dsa/basis.xdi \
	modules/common/gui/CEGUIConfig.xsd \
	modules/common/gui/cegui.config \
	modules/common/gui/fonts/Font.xsd \
	modules/common/gui/fonts/tahoma-10.font \
	modules/common/gui/fonts/tahoma-12.font \
	modules/common/gui/fonts/tahoma-14.font \
	modules/common/gui/fonts/tahoma-8.font \
	modules/common/gui/fonts/tahoma.ttf \
	modules/common/gui/imagesets/Imageset.xsd \
	modules/common/gui/imagesets/Rastullah.imageset \
	modules/common/gui/imagesets/TaharezLook.imageset \
	modules/common/gui/imagesets/TaharezLook.tga \
	modules/common/gui/schemes/GUIScheme.xsd \
	modules/common/gui/schemes/Rastullah.scheme \
	modules/common/gui/windows/GUILayout.xsd \
	modules/common/gui/windows/actionchoicewindow.xml \
	modules/common/gui/windows/charactersheet.xml \
	modules/common/gui/windows/commandmapper.xml \
	modules/common/gui/windows/console.xml \
	modules/common/gui/windows/debugwindow.xml \
	modules/common/gui/windows/dialogwindow.xml \
	modules/common/gui/windows/messagewindow.xml \
	modules/common/materials/dsa07_bk.jpg \
	modules/common/materials/dsa07_dn.jpg \
	modules/common/materials/dsa07_fr.jpg \
	modules/common/materials/dsa07_lf.jpg \
	modules/common/materials/dsa07_rt.jpg \
	modules/common/materials/dsa07_up.jpg \
	modules/common/materials/skies.material \
	modules/common/models/held.material \
	modules/common/models/held.mesh \
	modules/common/models/held.png \
	modules/common/models/held.skeleton \
	modules/common/scripts/classes.rb \
	modules/common/scripts/embed.rb \
	modules/common/scripts/globals.rb \
	modules/minidemo/conf/media.cfg \
	modules/minidemo/conf/moduleconfig.cfg \
	modules/minidemo/conf/resources.cfg \
	modules/minidemo/dialogs/aiml_test.aiml \
	modules/minidemo/dialogs/startup.xml \
	modules/minidemo/dsa/kalchas.xml \
	modules/minidemo/maps/2049x2049_map.png \
	modules/minidemo/maps/rastullah-testworld.cfg \
	modules/minidemo/maps/testraum.bsp \
	modules/minidemo/maps/testraum.zip \
	modules/minidemo/maps/testraum_alt.bsp \
	modules/minidemo/materials/dsa.zip \
	modules/minidemo/materials/terrain_detail.jpg \
	modules/minidemo/materials/terrain_texture.jpg \
	modules/minidemo/models/cube.mesh \
	modules/minidemo/models/flare.png \
	modules/minidemo/models/flaretrail.png \
	modules/minidemo/models/r2skin.jpg \
	modules/minidemo/models/robot.material \
	modules/minidemo/models/robot.mesh \
	modules/minidemo/models/robot.skeleton \
	modules/minidemo/models/sphere.mesh \
	modules/minidemo/models/test.particle \
	modules/minidemo/models/testraum.material \
	modules/minidemo/models/tisch_taverne.material \
	modules/minidemo/models/tisch_taverne.mesh \
	modules/minidemo/models/tisch_taverne.png \
	modules/minidemo/models/zumpi.particle \
	modules/minidemo/scripts/dialog.rb \
	modules/minidemo/scripts/physik.rb \
	modules/minidemo/scripts/simple.rb \
	modules/minidemo/scripts/sound.rb \
	modules/minidemo/scripts/startup.rb \
	modules/minidemo/scripts/test.rb \
	modules/minidemo/scripts/test_rules.rb \
	modules/minidemo/scripts/test_rules_actions.rb \
	modules/minidemo/scripts/zumpi.rb \
	modules/minidemo/scripts/maps/testraum.rb \
	modules/minidemo/sound/boom.ogg \
	modules/minidemo/sound/testsound.wav
	