noinst_HEADERS = \
    AboutWindow.h \
    ActionChoiceWindow.h \
    AbstractWindow.h \
    CeGuiHelper.h \
    CharacterSheetWindow.h \
    CharacterStateWindow.h \
    CloseConfirmationWindow.h \
    CombatControlState.h \
    CombatGui.h \
	CombatWindow.h \
    CommandMapper.h \
    ConfigComponent.h \
    Console.h \
    ContainerContentWindow.h \
    ControlState.h \
    CutsceneControlState.h \
    DebugWindow.h \
    DialogWindow.h \
    DialogControlState.h \
    FreeflightControlState.h \
    GameLoggerWindow.h \
    GameObjectInfoWindow.h \
	GameOverWindow.h \
	GameSaveLoadWindow.h \
    GameSettings.h \
    InfoPopup.h \
    InGameMenuWindow.h \
    InputManager.h \
    InventoryWindow.h \
    ItemDragContainer.h \
    ItemDescriptionDragContainer.h \
    ItemIconDragContainer.h \
    JournalWindow.h \
    ListboxWrappedTextItem.h \
    LogWindow.h \
    MainMenuWindow.h \
    MainMenuEngineWindow.h \
	MainMenuLoadWindow.h \
    MessageWindow.h \
    MovementControlState.h \
    ObjectDescriptionWindow.h \
    PlaylistWindow.h \
    ProgressWindow.h \
    PropertiesWindow.h \
    RenderSystemConfigComponent.h \
    SoundDriverConfigComponent.h \
    SubtitleWindow.h \
    UiPrerequisites.h \
    UiSubsystem.h \
    WindowFactory.h \
    WindowFadeJob.h \
    WindowManager.h

CLEANFILES = stdinc.h.gch
all-local: stdinc.h.gch
stdinc.h.gch: $(noinst_HEADERS) stdinc.h
	@echo Creating GCH file
	g++ -I. stdinc.h
