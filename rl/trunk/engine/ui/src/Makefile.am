INCLUDES = $(RUBY_CFLAGS) $(OGRE_CFLAGS) $(CEGUI_CFLAGS) $(CEGUIOGRE_CFLAGS) \
	$(STLPORT_CFLAGS) -I../include -I../../common/include  -I../../core/include \
	-I../../rules/include -I../components/include -I../../dialog/include \
	-I../../startup/include -I../../multimedia/include -I../../uicomponents/include 

lib_LTLIBRARIES = libRlUi.la
libRlUi_la_LDFLAGS = $(RUBY_LIBS) $(LIBS) \
	$(CEGUI_LIBS) $(OGRE_LIBS) $(CEGUIOGRE_LIBS) -lRlUiComponents -L../../uicomponents/src 
libRlUi_la_SOURCES = \
	AboutWindow.cpp \
	ActionChoiceWindow.cpp \
	CeGuiWindow.cpp \
	CharacterController.cpp \
	CharacterSheetWindow.cpp \
	CharacterStateWindow.cpp \
	CloseConfirmationWindow.cpp \
	CombatWindow.cpp \
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
	InGameMenuWindow.cpp \
	InputManager.cpp \
	InventoryWindow.cpp \
	JournalWindow.cpp \
	LogWindow.cpp \
	MainMenuWindow.cpp \
	MainMenuEngineWindow.cpp \
	MessageWindow.cpp \
	MovementCharacterController.cpp \
	PlaylistWindow.cpp \
	RTCombatCharacterController.cpp \
	SoundConfig.cpp \
	TargetSelectionWindow.cpp \
	UiSubsystem.cpp \
	VideoWindow.cpp \
	WindowFactory.cpp \
	WindowManager.cpp 
