namespace rl {

%feature("director") UiSubsystem;   
class UiSubsystem {
private:
	UiSubsystem();
public:
    static UiSubsystem& getSingleton(void);
    
    void showActionChoice(rl::GameObject* obj);
    Person* getActiveCharacter();
};

}
