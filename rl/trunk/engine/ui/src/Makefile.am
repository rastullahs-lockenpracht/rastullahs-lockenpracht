INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(CEGUIOGRE_CFLAGS) \
	$(STLPORT_CFLAGS) -I../include -I../../common/include  -I../../core/include \
	-I../../rules/include -I../components/include -I../../dialog/include \
	-I../../startup/include -I../../multimedia/include -I../../uicomponents/include \
	-I../../multimedia/include -I../../multimedia/include/fmod3 \
	-I../../multimedia/include/null -I../../multimedia/include/openal 

lib_LTLIBRARIES = libRlUi.la
libRlUi_la_LIBADD = -lRlUiComponents $(RUBY_LIBS) $(LIBS) \
	$(CEGUI_LIBS) $(OGRE_LIBS) $(CEGUIOGRE_LIBS) -lRlMultimedia
libRlUi_la_LDFLAGS = -L../../uicomponents/src  -L../../multimedia/src
libRlUi_la_SOURCES = \
	AboutWindow.cpp \
	ActionChoiceWindow.cpp \
	CeGuiWindow.cpp \
	CharacterController.cpp \
	CharacterSheetWindow.cpp \
	CharacterStateWindow.cpp \
	CloseConfirmationWindow.cpp \
	CommandExecutor.cpp \
	CommandMapper.cpp \
	CommandMapperWindow.cpp \
	Console.cpp \
	ContainerContentWindow.cpp \
	CutsceneCharacterController.cpp \
	DataLoadingProgressWindow.cpp \
	DebugWindow.cpp \
	DialogCharacterController.cpp \
	DialogWindow.cpp \
	FreeFlightCharacterController.cpp \
	GameLoggerWindow.cpp \
	InfoPopup.cpp \
	InGameMenuWindow.cpp \
	InputManager.cpp \
	InventoryWindow.cpp \
	JournalWindow.cpp \
	LogWindow.cpp \
	MainMenuWindow.cpp \
	MainMenuEngineWindow.cpp \
	MessageWindow.cpp \
	MovementCharacterController.cpp \
	ObjectDescriptionWindow.cpp \
	PlaylistWindow.cpp \
	SoundConfig.cpp \
	SoundDriverConfigWindow.cpp \
	SubtitleWindow.cpp \
	TargetSelectionWindow.cpp \
	UiSubsystem.cpp \
	VideoWindow.cpp \
	WindowFactory.cpp \
	WindowManager.cpp \
	WindowUpdater.cpp
