namespace rl {

class UiSubsystem 
{
public:
    static UiSubsystem& getSingleton(void);
    
	void showCharacterActionChoice();
	void showActionChoice(rl::GameObject* obj);
	void showMessageWindow(const CeGuiString& message);
	Person* getActiveCharacter();
	void setActiveCharacter(Person* person);
};

class CommandMapper
{
public:
	static CommandMapper& getSingleton();
	//void loadCommandMap(const char* mapfile);

};

}