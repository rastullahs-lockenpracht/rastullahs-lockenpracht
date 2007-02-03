INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(CEGUIOGRE_CFLAGS) \
	$(STLPORT_CFLAGS) $(OIS_CFLAGS) -I../include -I../../common/include  -I../../core/include \
	-I../../rules/include -I../components/include -I../../dialog/include \
	-I../../startup/include -I../../uicomponents/include 

lib_LTLIBRARIES = libRlUi.la
libRlUi_la_LIBADD = -lRlUiComponents $(RUBY_LIBS) $(LIBS) \
	$(CEGUI_LIBS) $(OGRE_LIBS) $(CEGUIOGRE_LIBS) $(OIS_LIBS)
libRlUi_la_LDFLAGS = -L../../uicomponents/src
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
	Selector.cpp \
	SoundConfig.cpp \
	SoundDriverConfigComponent.cpp \
	SubtitleWindow.cpp \
	UiSubsystem.cpp \
	VanityModeCharacterController.cpp \
	WindowFactory.cpp \
	WindowFadeJob.cpp \
	WindowManager.cpp
