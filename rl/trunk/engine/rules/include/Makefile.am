noinst_HEADERS = \
	Action.h \
	ActionManager.h \
	Armor.h \
	Combat.h \
	Combatant.h \
	CombatManager.h \
	Container.h \
	Creature.h \
	CreatureController.h \
	CreatureControllerManager.h \
	Date.h \
	DsaDataLoader.h \
	DsaManager.h \
	Effect.h \
	EffectFactory.h \
	EffectManagementTask.h \
	EffectManager.h \
	Eigenschaft.h \
	EigenschaftenStateSet.h \
	GameEventLog.h \
	GameObject.h \
	GameObjectStateListener.h \
	GameTimeSource.h \
	GlobalProperties.h \
	GoToJob.h \
	Item.h \
	Inventory.h \
	JournalEntry.h \
	Kampftechnik.h \
	ObjectStateChangeEvent.h \
	ObjectStateChangeEventSource.h \
	ObjectStateChangeListener.h \
	Person.h \
	Quest.h \
	QuestBook.h \
	QuestEvent.h \
	QuestListener.h \
	RulesPrerequisites.h \
	RulesSubsystem.h \
	SelectionHelper.h \
	Selector.h \
	Slot.h \
	StateSet.h \
	SonderfertigkeitenStateSet.h \
	Talent.h \
	Talentgruppe.h \
	TalentInputIterator.h \
	TalentStateSet.h \
	WaitJob.h \
	Weapon.h \
	ZauberStateSet.h

CLEANFILES = stdinc.h.gch
all-local: stdinc.h.gch
stdinc.h.gch: $(noinst_HEADERS) stdinc.h
	@echo Creating GCH file
	g++ -I. stdinc.h
