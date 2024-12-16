#pragma once



enum Submenu_s {
	MAIN = 0,
	MAIN_MENU,
	AIMBOT_MENU,
	VISUAL_MENU,
	SETTINGS,
	CLIENT_LIST,
	RME_MENU,
	RME_EXTRAS,
	RME_ALL_CLIENTS,
	RME_MENU_RENAME_PRESETS,
	CLIENT_OPTIONS,
	GAMERTAG_EDITOR,
	CUSTOM_GAMERTAGS,
	CLIENT_RME_OPTIONS,
	CLIENT_RME_EXTRAS
};

struct MenuItems_s {
	int currentOption;
	int optionCount;
	int submenu;
	int submenuLevel;
	int lastSubmenu[20];
	int lastOption[20];
	bool optionPress;
	bool optionPressX;
	bool leftPress;
	bool rightPress;
};

class Menu {
private:
	char* option;
	DWORD font;
	float fontScale;

	char temp[50];
public:

	void ResetVars();
	int GetOption();
	Menu(char* optionText = NULL, DWORD font = Sunset::dwSmallFont, float fontScale = Sunset::fOptionScale);
	void AddOption(char* optionName);
	inline Menu Option();
	inline Menu AddInformation(char* information, int index);
	inline Menu Toggle(BOOL* toChange);
	template<typename T>
	inline Menu ArrayEditor(T arr, int size, int* identifier);
	template<typename T>
	inline Menu Editor(T* toEdit, T increments, T min, T max);
	inline Menu Submenu(int submenu);
	inline Menu PlayerSubmenu(int submenu);
	template<typename T>
	inline Menu Callback(T function);
	template<typename T, typename A>
	inline Menu Callback(T function, A a);
	template<typename T, typename A, typename B>
	inline Menu Callback(T function, A a, B b);
	template<typename T, typename A, typename B, typename C>
	inline Menu Callback(T function, A a, B b, C c);
	inline Menu ChangeBoolean(BOOL* toChange, BOOL value);
	template<typename T, typename A>
	inline Menu GamertagCallback(T function, A a, int index);
	template<typename T>
	inline Menu SetGamertag(T function);

	static void HandleInput();
	static int ReturnCorrectSubmenu();
	static void HandleMenu();
	static void HandleOptions();
	static void Update();
};

extern MenuItems_s MenuItems;
extern int SelectedPlayer;
extern int SelectedGamertagIndex;