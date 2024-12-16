//everything is left as I got it originally, exception is the client list fix and two comments. Everything else is original #Dev___1_
#include "stdafx.h"

MenuItems_s MenuItems;
int SelectedPlayer = -1;
int SelectedGamertagIndex = -1;

float flOldScrollPos;
DWORD dwScrollTick;
#define MOVE_TIME 100 // 0.1 second(s)

void Menu::ResetVars() {
	MenuItems.optionPress = false;
	MenuItems.optionPressX = false;
	MenuItems.leftPress = false;
	MenuItems.rightPress = false;
}

int Menu::GetOption() {
	if (MenuItems.optionPress)
		return MenuItems.currentOption;
	else
		return 0;
}

Menu::Menu(char* optionText, DWORD font, float fontScale) {
	this->option = optionText;
	this->font = font;
	this->fontScale = fontScale;
}

float flStaticBoxY = (Sunset::fMenuPosY + 57.0f);

void Menu::AddOption(char* optionName) {
	MenuItems.optionCount++;

	if (MenuItems.currentOption <= Sunset::dwDisplayOptions && MenuItems.optionCount <= Sunset::dwDisplayOptions) {
		if (MenuItems.currentOption == MenuItems.optionCount) {
			int MoveTick = (GetTickCount() - dwScrollTick);
			float yPos = (flStaticBoxY + (MenuItems.optionCount * 22.0f));

			if (MoveTick < MOVE_TIME)
			{
				float TickPercent = ((float)MoveTick / (float)MOVE_TIME);
				yPos = (((yPos - flOldScrollPos) * TickPercent) + flOldScrollPos);
			}

			Render::DrawShader(Sunset::fMenuPosX, yPos, Sunset::fMenuWidth, 22.0f, Sunset::dwWhite, MenuOptions::bSunsetMenuColorFade ? colorMainMenuFade : colorRed);
		}

		Render::DrawText(optionName, (Sunset::fMenuPosX + 10), ((Sunset::fMenuPosY + 78.0f) + (MenuItems.optionCount * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite);
	}
	else if ((MenuItems.optionCount > (MenuItems.currentOption - Sunset::dwDisplayOptions)) && MenuItems.optionCount <= MenuItems.currentOption) {
		if (MenuItems.currentOption == MenuItems.optionCount) {
			int MoveTick = (GetTickCount() - dwScrollTick);
			float yPos = (flStaticBoxY + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f));

			if (MoveTick < MOVE_TIME)
			{
				float TickPercent = ((float)MoveTick / (float)MOVE_TIME);
				yPos = (((yPos - flOldScrollPos) * TickPercent) + flOldScrollPos);
			}

			Render::DrawShader(Sunset::fMenuPosX, yPos, Sunset::fMenuWidth, 22.0f, Sunset::dwWhite, MenuOptions::bSunsetMenuColorFade ? colorMainMenuFade : colorRed);
		}

		Render::DrawText(optionName, (Sunset::fMenuPosX + 10), ((Sunset::fMenuPosY + 78.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite);
	}
}

inline Menu Menu::Option() {
	AddOption(this->option);

	return *this;
}

inline Menu Menu::AddInformation(char* information, int index) {
	/*AddOption(this->option);

	FLOAT currentArrayLength = (Render::GetTextWidth(information, Sunset::dwSmallFont) * 0.9f);

	if (MenuItems.currentOption <= Sunset::dwDisplayOptions && MenuItems.optionCount <= Sunset::dwDisplayOptions) {
		Render::DrawText(information, (((Sunset::fMenuPosX + Sunset::fMenuWidth) - 10.0f) - currentArrayLength), ((Sunset::fMenuPosY + 78.0f) + (MenuItems.optionCount * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite);
	}
	else if ((MenuItems.optionCount > (MenuItems.currentOption - Sunset::dwDisplayOptions)) && MenuItems.optionCount <= MenuItems.currentOption) {
		Render::DrawText(information, (((Sunset::fMenuPosX + Sunset::fMenuWidth) - 10.0f) - currentArrayLength), ((Sunset::fMenuPosY + 78.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite);
	}*/


	if (MenuItems.currentOption <= Sunset::dwDisplayOptions && MenuItems.optionCount <= Sunset::dwDisplayOptions) {
		Render::DrawText(this->option, (Sunset::fMenuPosX + 10), ((Sunset::fMenuPosY + 78.0f) + ((MenuItems.optionCount + index) * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite); //option

		//information
		Render::DrawString(information, ((Sunset::fMenuPosX + Sunset::fMenuWidth) - 10.0f), ((Sunset::fMenuPosY + 78.0f) + ((MenuItems.optionCount + index) * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite, JustifyRight);
	}
	else if ((MenuItems.optionCount > (MenuItems.currentOption - Sunset::dwDisplayOptions)) && MenuItems.optionCount <= MenuItems.currentOption) {
		Render::DrawText(this->option, (Sunset::fMenuPosX + 10), ((Sunset::fMenuPosY + 78.0f) + (((MenuItems.optionCount + index) - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite);  //option
	
				//information
		Render::DrawString(information, ((Sunset::fMenuPosX + Sunset::fMenuWidth) - 10.0f), ((Sunset::fMenuPosY + 78.0f) + (((MenuItems.optionCount + index) - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite, JustifyRight);
	}

	return *this;
}

inline Menu Menu::Toggle(BOOL* toChange) {
	AddOption(this->option);

	float size = 15.0f;

	if (MenuItems.currentOption <= Sunset::dwDisplayOptions && MenuItems.optionCount <= Sunset::dwDisplayOptions) {
		Render::DrawShader(((Sunset::fMenuPosX + Sunset::fMenuWidth) - 25.0f), (((Sunset::fMenuPosY + 57.0f) + (MenuItems.optionCount * 22.0f)) + 3.0f), size, 1, Sunset::dwWhite, colorWhite); //Top
		Render::DrawShader(((Sunset::fMenuPosX + Sunset::fMenuWidth) - 25.0f), (((Sunset::fMenuPosY + 57.0f) + (MenuItems.optionCount * 22.0f)) + 3.0f), 1, size, Sunset::dwWhite, colorWhite); //Left
		Render::DrawShader((((Sunset::fMenuPosX + Sunset::fMenuWidth) - 25.0f) + size), (((Sunset::fMenuPosY + 57.0f) + (MenuItems.optionCount * 22.0f)) + 3.0f), 1, size, Sunset::dwWhite, colorWhite); //Right
		Render::DrawShader(((Sunset::fMenuPosX + Sunset::fMenuWidth) - 25.0f), ((((Sunset::fMenuPosY + 57.0f) + (MenuItems.optionCount * 22.0f)) + size) + 3.0f), size, 1, Sunset::dwWhite, colorWhite); //Bottom

		if (*toChange) {
			Render::DrawShader(((Sunset::fMenuPosX + Sunset::fMenuWidth) - 25.0f), (((Sunset::fMenuPosY + 57.0f) + (MenuItems.optionCount * 22)) + 3.0f), size, size, Sunset::dwWhite, colorWhite); //Fill
		}	
	}
	else if ((MenuItems.optionCount > (MenuItems.currentOption - Sunset::dwDisplayOptions)) && MenuItems.optionCount <= MenuItems.currentOption) {
		Render::DrawShader(((Sunset::fMenuPosX + Sunset::fMenuWidth) - 25.0f), (((Sunset::fMenuPosY + 57.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)) + 3.0f), size, 1, Sunset::dwWhite, colorWhite); //Top
		Render::DrawShader(((Sunset::fMenuPosX + Sunset::fMenuWidth) - 25.0f), (((Sunset::fMenuPosY + 57.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)) + 3.0f), 1, size, Sunset::dwWhite, colorWhite); //Left
		Render::DrawShader((((Sunset::fMenuPosX + Sunset::fMenuWidth) - 25.0f) + size), (((Sunset::fMenuPosY + 57.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)) + 3.0f), 1, size, Sunset::dwWhite, colorWhite); //Right
		Render::DrawShader(((Sunset::fMenuPosX + Sunset::fMenuWidth) - 25.0f), ((((Sunset::fMenuPosY + 57.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)) + size) + 3.0f), size, 1, Sunset::dwWhite, colorWhite); //Bottom

		if (*toChange) {
			Render::DrawShader(((Sunset::fMenuPosX + Sunset::fMenuWidth) - 25.0f), (((Sunset::fMenuPosY + 57.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22)) + 3.0f), size, size, Sunset::dwWhite, colorWhite); //Fill
		}
	}

	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.optionPress) {
		*toChange = !*toChange;
	}

	return *this;
}

template<typename T>
inline Menu Menu::ArrayEditor(T arr, int size, int* identifier) {
	AddOption(this->option);

	sprintf(this->temp, "%s", arr);

	FLOAT currentArrayLength = (Game::GetTextWidth(this->temp, Sunset::dwSmallFont) * Sunset::fOptionScale);

	if (MenuItems.currentOption <= Sunset::dwDisplayOptions && MenuItems.optionCount <= Sunset::dwDisplayOptions) {
		Render::DrawText(this->temp, (((Sunset::fMenuPosX + Sunset::fMenuWidth) - 10.0f) - currentArrayLength), ((Sunset::fMenuPosY + 78.0f) + (MenuItems.optionCount * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite);
	}
	else if ((MenuItems.optionCount > (MenuItems.currentOption - Sunset::dwDisplayOptions)) && MenuItems.optionCount <= MenuItems.currentOption) {
		Render::DrawText(this->temp, (((Sunset::fMenuPosX + Sunset::fMenuWidth) - 10.0f) - currentArrayLength), ((Sunset::fMenuPosY + 78.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite);
	}

	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.leftPress) {
		*identifier -= 1;
		if (*identifier < 0)
			*identifier = (size - 1);
	}

	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.rightPress) {
		*identifier += 1;
		if (*identifier > (size - 1))
			*identifier = 0;
	}

	return *this;
}

template<typename T>
inline Menu Menu::Editor(T* toEdit, T increments, T min, T max) {
	AddOption(this->option);

	if (is_same<T, float>::value) {
		sprintf(this->temp, "%.0f", *toEdit);
	}
	if (is_same<T, unsigned char>::value) {
		sprintf(this->temp, "%d", *toEdit);
	}
	if (is_same<T, int>::value) {
		sprintf(this->temp, "%d", *toEdit);
	}

	float sliderWidth = 60.0f;
	float fBarWidth = sliderWidth * ((*toEdit - (min)) / ((max) - (min)));
	float currentBarTextLength = (Game::GetTextWidth(this->temp, Sunset::dwSmallFont) * 0.9f);

	if (MenuItems.currentOption <= Sunset::dwDisplayOptions && MenuItems.optionCount <= Sunset::dwDisplayOptions) {
		Render::DrawText(this->temp, ((((Sunset::fMenuPosX + Sunset::fMenuWidth) - 17.0f) - sliderWidth) - currentBarTextLength), ((Sunset::fMenuPosY + 78.0f) + (MenuItems.optionCount * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite);

		Render::DrawShader((((Sunset::fMenuPosX + Sunset::fMenuWidth) - 10.0f) - sliderWidth), (((Sunset::fMenuPosY + 57.0f) + (MenuItems.optionCount * 22.0f)) + 10.0f), sliderWidth, 2.0f, Sunset::dwWhite, colorWhite);
		Render::DrawShader(((((Sunset::fMenuPosX + Sunset::fMenuWidth) - 12.0f) - sliderWidth) + fBarWidth), (((Sunset::fMenuPosY + 57.0f) + (MenuItems.optionCount * 22.0f)) + 5.0f), 2.0f, 12.0f, Sunset::dwWhite, colorWhite);
	}
	else if ((MenuItems.optionCount > (MenuItems.currentOption - Sunset::dwDisplayOptions)) && MenuItems.optionCount <= MenuItems.currentOption) {
		Render::DrawText(this->temp, ((((Sunset::fMenuPosX + Sunset::fMenuWidth) - 17.0f) - sliderWidth) - currentBarTextLength), ((Sunset::fMenuPosY + 78.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite);

		Render::DrawShader((((Sunset::fMenuPosX + Sunset::fMenuWidth) - 10.0f) - sliderWidth), (((Sunset::fMenuPosY + 57.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)) + 10.0f), sliderWidth, 2.0f, Sunset::dwWhite, colorWhite);
		Render::DrawShader(((((Sunset::fMenuPosX + Sunset::fMenuWidth) - 12.0f) - sliderWidth) + fBarWidth), (((Sunset::fMenuPosY + 57.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)) + 5.0f), 2.0f, 12.0f, Sunset::dwWhite, colorWhite);
	}

	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.leftPress) {
		*toEdit -= increments;
		if (*toEdit < min)
			*toEdit = max;
	}

	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.rightPress) {
		*toEdit += increments;
		if (*toEdit > max)
			*toEdit = min;
	}

	return *this;
}

inline Menu Menu::Submenu(int submenu) {
	AddOption(this->option);

	if (MenuItems.submenu != MAIN) {
		sprintf(this->temp, ">>");

		FLOAT currentArrayLength = (Game::GetTextWidth(this->temp, Sunset::dwSmallFont) * Sunset::fOptionScale);

		if (MenuItems.currentOption <= Sunset::dwDisplayOptions && MenuItems.optionCount <= Sunset::dwDisplayOptions) {
			Render::DrawText(this->temp, (((Sunset::fMenuPosX + Sunset::fMenuWidth) - 10.0f) - currentArrayLength), ((Sunset::fMenuPosY + 78.0f) + (MenuItems.optionCount * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite);
		}
		else if ((MenuItems.optionCount > (MenuItems.currentOption - Sunset::dwDisplayOptions)) && MenuItems.optionCount <= MenuItems.currentOption) {
			Render::DrawText(this->temp, (((Sunset::fMenuPosX + Sunset::fMenuWidth) - 10.0f) - currentArrayLength), ((Sunset::fMenuPosY + 78.0f) + ((MenuItems.optionCount - (MenuItems.currentOption - Sunset::dwDisplayOptions)) * 22.0f)), Sunset::dwSmallFont, Sunset::fOptionScale, colorWhite);
		}
	}

	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.optionPress) {
		MenuItems.lastSubmenu[MenuItems.submenuLevel] = MenuItems.submenu;
		MenuItems.lastOption[MenuItems.submenuLevel] = MenuItems.currentOption;
		MenuItems.currentOption = 1;
		MenuItems.submenu = submenu;
		MenuItems.submenuLevel++;
	}
	return *this;
}

inline Menu Menu::PlayerSubmenu(int submenu) {
	AddOption(this->option);

	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.optionPress) {
		MenuItems.lastSubmenu[MenuItems.submenuLevel] = MenuItems.submenu;
		MenuItems.lastOption[MenuItems.submenuLevel] = MenuItems.currentOption;
		SelectedPlayer = (MenuItems.currentOption - 1);
		MenuItems.currentOption = 1;
		MenuItems.submenu = submenu;
		MenuItems.submenuLevel++;
	}
	return *this;
}

template<typename T>
inline Menu Menu::Callback(T function) {
	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.optionPress)
		function();
	return *this;
}

template<typename T, typename A>
inline Menu Menu::Callback(T function, A a) {
	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.optionPress)
		function(a);
	return *this;
}

template<typename T, typename A, typename B>
inline Menu Menu::Callback(T function, A a, B b) {
	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.optionPress)
		function(a, b);
	return *this;
}

template<typename T, typename A, typename B, typename C>
inline Menu Menu::Callback(T function, A a, B b, C c) {
	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.optionPress)
		function(a, b, c);
	return *this;
}

inline Menu Menu::ChangeBoolean(BOOL* toChange, BOOL value) {
	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.optionPress)
		*toChange = value;
	return *this;
}

template<typename T, typename A>
inline Menu Menu::GamertagCallback(T function, A a, int index) {
	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.optionPress) {
		function(a);
	}
	else if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.optionPressX) {
		SelectedGamertagIndex = index;
		if (SelectedGamertagIndex != -1) {
			MenuOptions::bEditCustomGT = TRUE;
		}
	}
	return *this;
}

template<typename T>
inline Menu Menu::SetGamertag(T function) {
	if (MenuItems.currentOption == MenuItems.optionCount && MenuItems.optionPress)
		function(this->option);
	return *this;
}

void HandleScroll(bool bUp) {
	int currentOption = MenuItems.currentOption;
	int maxOptions = MenuItems.optionCount;

jRestart:
	currentOption = currentOption + (bUp ? -1 : 1);

	if (currentOption == MenuItems.currentOption)
		return;

	if ((GetTickCount() - dwScrollTick) >= MOVE_TIME) {
		int Index = (MenuItems.currentOption <= Sunset::dwDisplayOptions) ? MenuItems.currentOption : (MenuItems.currentOption - (MenuItems.currentOption - Sunset::dwDisplayOptions));
		flOldScrollPos = flStaticBoxY + (Index * 22.0f);

		dwScrollTick = GetTickCount();
	}

	if (currentOption > maxOptions) {
		currentOption = 1;
	}
	else if (currentOption < 1) {
		currentOption = maxOptions;
	}

	//if (bIsInfoOnly[MenuItems.submenu][currentOption]) {
	//	goto jRestart;
	//}

	MenuItems.currentOption = currentOption;
}

bool buttonFinished;
void Menu::HandleInput() {
	XINPUT_STATE state;
	XInputGetState(0, &state);
	short button = state.Gamepad.wButtons;

	if (!buttonFinished)
	{
		if (button & XINPUT_GAMEPAD_DPAD_LEFT && state.Gamepad.bLeftTrigger >= 100) {
			if (Sunset::bMenuOpen) {
				Sunset::bSunsetSave = TRUE;
				//DbgPrint("Saving INI!");
			}
			Sunset::bMenuOpen = !Sunset::bMenuOpen;

			if (Sunset::bMenuOpen) {
				MenuItems.lastSubmenu[MenuItems.submenuLevel] = MenuItems.submenu;
				MenuItems.lastOption[MenuItems.submenuLevel] = MenuItems.currentOption;
				MenuItems.currentOption = 1;
				MenuItems.submenu = MAIN;
				MenuItems.submenuLevel++;
				MenuItems.submenuLevel = 0;
			}

			buttonFinished = true;
		}

		if (button != 0 && Sunset::bMenuOpen) {
			if (button & XINPUT_GAMEPAD_A) {
				MenuItems.optionPress = true;
			}
			else if (button & XINPUT_GAMEPAD_B) {
				if (MenuItems.submenu > MAIN) {
					MenuItems.submenu = MenuItems.lastSubmenu[MenuItems.submenuLevel - 1];
					MenuItems.currentOption = MenuItems.lastOption[MenuItems.submenuLevel - 1];
					MenuItems.submenuLevel--;
				}
			}
			else if (button & XINPUT_GAMEPAD_X) {
				MenuItems.optionPressX = true;
			}
			else if (button & XINPUT_GAMEPAD_DPAD_LEFT) {
				MenuItems.leftPress = true;
			}
			else if (button & XINPUT_GAMEPAD_DPAD_RIGHT) {
				MenuItems.rightPress = true;
			}

			if (button & XINPUT_GAMEPAD_DPAD_UP)
				HandleScroll(true);

			if (button & XINPUT_GAMEPAD_DPAD_DOWN)
				HandleScroll(false);

			buttonFinished = true;
		}
	}

	if (button == 0 && buttonFinished) {
		buttonFinished = false;
	}
}

void Menu::HandleMenu() {
	float height = (Sunset::dwDisplayOptions * 22.0f);

	if (MenuItems.submenu == MAIN)
		height = (6 * 22.0f);
	else if (MenuItems.submenu == AIMBOT_MENU)
		height = ((!CG::bIsZombies ? 7 : 6) * 22.0f);
	else if (MenuItems.submenu == GAMERTAG_EDITOR)
		height = (7 * 22.0f);
	else if (MenuItems.submenu == VISUAL_MENU)
		height = ((!CG::bIsZombies ? 10 : 9) * 22.0f);
	else if (MenuItems.submenu == SETTINGS)
		height = (10 * 22.0f);
	else if (MenuItems.submenu == CLIENT_OPTIONS) {
#if defined (BO2)
		height = (10 * 22.0f);
#elif defined (BO1)
		height = (9 * 22.0f);
#endif
	}
	else if (MenuItems.submenu == CLIENT_RME_OPTIONS) {
#if defined (BO2)
		height = ((!CG::bIsZombies ? 10 : 10) * 22.0f);
#elif defined (BO1)
		height = (10 * 22.0f);
#endif
	}
	else if (MenuItems.submenu == CLIENT_RME_EXTRAS) {
#if defined (BO2)
		height = (3 * 22.0f);
#elif defined (BO1)
		height = (2 * 22.0f);
#endif
	}
	else if (MenuItems.submenu == RME_MENU) {
#if defined (BO2)
		height = (10 * 22.0f);
#elif defined (BO1)
		height = (10 * 22.0f);
#endif
	}
	else if (MenuItems.submenu == RME_EXTRAS) {
#if defined (BO2)
		height = (3 * 22.0f);
#elif defined (BO1)
		height = (2 * 22.0f);
#endif
	}
	else if (MenuItems.submenu == RME_ALL_CLIENTS) {
#if defined (BO2)
		height = (7 * 22.0f);
#elif defined (BO1)
		height = (7 * 22.0f);
#endif
	}
	else if (MenuItems.submenu == RME_MENU_RENAME_PRESETS) {
#if defined (BO2)
		height = (5 * 22.0f);
#elif defined (BO1)
		height = (4 * 22.0f);
#endif
	}

	Sunset::fMenuPosX = ((Sunset::dwScreenCenterX - (Sunset::fMenuWidth / 2)));
	Sunset::fMenuPosY = (Sunset::dwScreenCenterY - ((height + 115.0f) / 2));

	//draw background
	Render::DrawShader(Sunset::fMenuPosX, Sunset::fMenuPosY, Sunset::fMenuWidth, (height + 115.0f), Sunset::dwWhite, colorBackground);

	//Header grey line
	Render::DrawShader((Sunset::fMenuPosX + 2.0f), (Sunset::fMenuPosY + 70.0f), (Sunset::fMenuWidth - 4.0f), 2.0f, Sunset::dwWhite, colorGrey);

	//Footer grey line
	Render::DrawShader((Sunset::fMenuPosX + 2.0f), ((Sunset::fMenuPosY + 85.0f) + height), (Sunset::fMenuWidth - 4.0f), 2.0f, Sunset::dwWhite, colorGrey);

	//Footer red line
	Render::DrawShader(Sunset::fMenuPosX, (Sunset::fMenuPosY + (height + 115.0f)), Sunset::fMenuWidth, 2.0f, Sunset::dwWhite, MenuOptions::bSunsetMenuColorFade ? colorMainMenuFade : colorRed);

	//Engine Name
	FLOAT textwidth = ((Game::GetTextWidth(ENGINE_NAME, Sunset::dwSmallFont) / 2) * Sunset::fMenuNameScale);
	Render::DrawText(ENGINE_NAME, (Sunset::fMenuPosX + (Sunset::fMenuWidth / 2) - textwidth), (Sunset::fMenuPosY + 35.0f), Sunset::dwSmallFont, Sunset::fMenuNameScale, MenuOptions::bSunsetMenuColorFade ? colorMainMenuFade : colorRed);

	//Current Menu
	PCHAR MenuTitles[15] = { "Main", "Main Menu", "Aimbot Menu", "Visual Menu", "Settings", "Client List", "RME Menu", "RME Extra Options", "RME All Clients", "RME Rename Presets", "Empty Slot", "Gamertag Editor", "Custom Gamertags", "RME Options", "RME Extra Options" };

	PCHAR MenuTitle = MenuTitles[MenuItems.submenu];

	if (MenuItems.submenu == CLIENT_OPTIONS) {
		if (SelectedPlayer != -1) {
#if !defined (BO1_ZM)
			if (!strcmp(partyMember[SelectedPlayer].clanAbbrev, ""))
				MenuTitle = partyMember[SelectedPlayer].gamertag;
			else
				MenuTitle = va("[%s] %s", partyMember[SelectedPlayer].clanAbbrev, partyMember[SelectedPlayer].gamertag);
#else
			MenuTitle = partyMember[SelectedPlayer].gamertag;
#endif
		}
		else
			MenuTitle = "Error";
	}

	if (MenuTitle != NULL) {
		FLOAT submenuTextLength = ((Game::GetTextWidth(MenuTitle, Sunset::dwSmallFont) / 2) * Sunset::fMenuTitleScale);
		Render::DrawText(MenuTitle, (Sunset::fMenuPosX + (Sunset::fMenuWidth / 2) - submenuTextLength), (Sunset::fMenuPosY + 63.0f), Sunset::dwSmallFont, Sunset::fMenuTitleScale, colorWhite);
	}

	//Footer informations
	char* currentOptText = va("%i / %i", MenuItems.currentOption, MenuItems.optionCount);
	FLOAT currentOptLength = (Game::GetTextWidth(currentOptText, Sunset::dwSmallFont) * Sunset::fMenuTitleScale);

	float infoTextY = (((Sunset::fMenuPosY + 83.0f) + height) + 29.0f);

	Render::DrawText(ENGINE_VERSION, (Sunset::fMenuPosX + 10), infoTextY, Sunset::dwSmallFont, Sunset::fMenuTitleScale, colorWhite);
	Render::DrawText(currentOptText, (((Sunset::fMenuPosX + Sunset::fMenuWidth) - 10.0f) - currentOptLength), infoTextY, Sunset::dwSmallFont, Sunset::fMenuTitleScale, colorWhite);


	MenuItems.optionCount = 0;
	flStaticBoxY = (Sunset::fMenuPosY + 57.0f);

	switch (MenuItems.submenu) {
	case MAIN:
		Menu("Main Menu").Submenu(MAIN_MENU);
		Menu("Aimbot Menu").Submenu(AIMBOT_MENU);
		Menu("Visual Menu").Submenu(VISUAL_MENU);
		Menu("Settings").Submenu(SETTINGS);
		Menu("Client List").Submenu(CLIENT_LIST);
		Menu("RME Menu").Submenu(RME_MENU);
		break;

	case MAIN_MENU:
		Menu("Gamertag Editor").Submenu(GAMERTAG_EDITOR);
		Menu("No Recoil").Toggle(&MenuOptions::bNoRecoil);
		Menu("No Sway").Toggle(&MenuOptions::bNoSway);
		Menu("No Flinch").Toggle(&MenuOptions::bNoFlinch);
		Menu("No Bob").Toggle(&MenuOptions::bNoBob);
		Menu("No Spread").Toggle(&MenuOptions::bNoSpread);
		Menu("No Bullet Impacts").Toggle(&MenuOptions::bNoBulletImpacts);
		Menu("Constant UAV").Toggle(&MenuOptions::bRadar);
#ifndef BO1
		Menu("Laser").Toggle(&MenuOptions::bLaser);
#endif
		Menu("Fast Reload").Toggle(&MenuOptions::bFastReload);
		Menu("Third Person").Toggle(&MenuOptions::bThirdPerson);
		Menu("Third Person Range").Editor<FLOAT>(&MenuOptions::fThirdPersonRange, 5.0f, 60.0f, 160.0f);
		Menu("Field Of View").Editor<FLOAT>(&MenuOptions::fFieldOfView, 5.0f, 65.0f, 160.0f);

		Menu("Disconnect").Option().Callback(Game::LeaveGame);
		Menu("End Game").Option().Callback(Game::EndGame);
#ifndef BO1
		Menu("Crash Game").Option().Callback(Game::SuperEndGame);
#endif

#if defined (BO2)
		if (CG::bIsZombies) {
			Menu("Set Zombies Max Rank").Option().Callback(Game::SetZombieMaxRank);
		}
#endif

#if defined (DEBUG_BUILD)
		Menu("Dump Buffer").Option().ChangeBoolean(&Sunset::DumpBuffer, TRUE);
		//Menu("Dump Buffer").Option().Callback(SendJo);
#endif

		break;

	case GAMERTAG_EDITOR:
		Menu("Set Gamertag").Option().ChangeBoolean(&MenuOptions::bLaunchCustomGT, TRUE);
		Menu("Rainbow Gamertag").Toggle(&MenuOptions::bRainbowGT);
		Menu("Rainbow Delay").Editor<FLOAT>(&MenuOptions::fRainbowDelay, 250.0f, 1000.0f, 3500.0f);
		Menu("Cycle Gamertags").Toggle(&MenuOptions::bCycleGT);
		Menu("Cycle Delay").Editor<FLOAT>(&MenuOptions::fCycleDelay, 250.0f, 1000.0f, 3500.0f);
		Menu("Custom Gamertags").Submenu(CUSTOM_GAMERTAGS);
		Menu("Set Default Gamertag").Option().Callback(Game::ResetGamertag);
		break;

	case CUSTOM_GAMERTAGS:
		for (int i = 0; i < 10; i++) {
			Menu(pModdedGamertags[i]).Option().GamertagCallback(Game::SetGamertag, pModdedGamertags[i], i);
		}
		break;

	case AIMBOT_MENU:
		Menu("Aimbot").Toggle(&MenuOptions::bAimbot);
		Menu("Aimbot Type").ArrayEditor(Array_AimbotType[MenuOptions::dwAimbotType], ARRAYSIZE(Array_AimbotType), (int*)&MenuOptions::dwAimbotType);
		Menu("Target Type").ArrayEditor(Array_TargetType[MenuOptions::dwTargetType], ARRAYSIZE(Array_TargetType), (int*)&MenuOptions::dwTargetType);
		Menu("Target Bone").ArrayEditor(Array_TargetBone[MenuOptions::dwTargetBone], ARRAYSIZE(Array_TargetBone), (int*)&MenuOptions::dwTargetBone);
		Menu("Auto Shoot").Toggle(&MenuOptions::bAutoShoot);
		Menu("Ping Prediction").Toggle(&MenuOptions::bPingPrediction);
		if (!CG::bIsZombies) {
			Menu("Anti Aim").Toggle(&MenuOptions::bAntiAim);
		}
		break;

	case VISUAL_MENU:

		Menu("Draw Allies").Toggle(&MenuOptions::bDrawAllies);
		Menu("Draw Axis").Toggle(&MenuOptions::bDrawAxis);
		Menu("Box Type").ArrayEditor(DrawingTypes[MenuOptions::dwBoxType], ARRAYSIZE(DrawingTypes), (int*)&MenuOptions::dwBoxType);
		Menu("Names").Toggle(&MenuOptions::bDrawNames);
		Menu("Distances").Toggle(&MenuOptions::bDrawDistances);
		if (!CG::bIsZombies) {
			Menu("Weapons").Toggle(&MenuOptions::bDrawWeapons);
		}
		Menu("Bones").Toggle(&MenuOptions::bDrawBones);
		Menu("Draw Snaplines").Toggle(&MenuOptions::bDrawSnaplines);
		Menu("Small Crosshair").Toggle(&MenuOptions::bSmallCrosshair);
		Menu("Health Bar").Toggle(&MenuOptions::bHealthBar);

		break;

	case SETTINGS:
		Menu("Menu Width").Editor<FLOAT>(&Sunset::fMenuWidth, 5.0f, 280.0f, 450.0f);
		Menu("Max Display Count").Editor<FLOAT>(&Sunset::dwDisplayOptions, 1.0f, 10.0f, 20.0f);
		Menu("Watermark").Toggle(&Sunset::bWatermark);
		Menu("Debug Info").Toggle(&MenuOptions::bDebugInfo);
		Menu("Main Menu Fade").Toggle(&MenuOptions::bMainMenuFade);
		Menu("Sunset Color Fade").Toggle(&MenuOptions::bSunsetMenuColorFade);
		Menu("Game Protections").Toggle(&MenuOptions::bGameProtections);
		Menu("Allow People Joining You").Toggle(&MenuOptions::bJoinInProgressAllowed);
		Menu("Load Rage Config").Option().Callback(Game::LoadConfig);
		Menu("Unload Menu").Option().ChangeBoolean(&Global.UnloadEngine, TRUE);
		break;

	case CLIENT_LIST:
	{
		for (int i = 0; i < 18; i++) {
			if (partyMember[i].XUID == 0 || partyMember[i].gamertag[0] == NULL) {
				Menu("Empty Slot").Option();
			}
			else {
#if !defined (BO1_ZM)

#if defined (DEBUG_BUILD)
				if (playerMenuDetection[i].bDetected) {
					Menu(va("[Sunset User] %s", partyMember[i].gamertag)).PlayerSubmenu(CLIENT_OPTIONS);
				}
				else {
					if (!strcmp(partyMember[i].clanAbbrev, ""))
						Menu(partyMember[i].gamertag).PlayerSubmenu(CLIENT_OPTIONS);
					else
						Menu(va("[%s] %s", partyMember[i].clanAbbrev, partyMember[i].gamertag)).PlayerSubmenu(CLIENT_OPTIONS);
			}
#else

				if (!strcmp(partyMember[i].clanAbbrev, ""))
					Menu(partyMember[i].gamertag).PlayerSubmenu(CLIENT_OPTIONS);
				else
					Menu(va("[%s] %s", partyMember[i].clanAbbrev, partyMember[i].gamertag)).PlayerSubmenu(CLIENT_OPTIONS);
#endif





#else
				//Menu(va("%s", partyMember[i].gamertag)).PlayerSubmenu(CLIENT_OPTIONS);
				PBYTE pXUID = (PBYTE)malloc(0x8);
				memcpy(pXUID, &partyMember[i].XUID, 0x8);

				if (pXUID[0] == 0x00 && pXUID[1] == 0x09) {
					Menu(va("%s", partyMember[i].gamertag)).PlayerSubmenu(CLIENT_OPTIONS);
				}
				else {
					Menu("Empty Slot").Option();
				}

				free(pXUID);
#endif
			}
		}
	}
	break;

	case CLIENT_OPTIONS:
	{
		IN_ADDR onlineIP = partyMember[SelectedPlayer].onlineIP;
		IN_ADDR localIP = partyMember[SelectedPlayer].internalIP;

		Menu("RME Options").Submenu(CLIENT_RME_OPTIONS);
		Menu("Blacklisted").Toggle(&pInfo[SelectedPlayer].Prioritized);
		Menu("Whitelisted").Toggle(&pInfo[SelectedPlayer].Whitelisted);
		Menu("Kick Player").Option().Callback(Game::OffhostKick, SelectedPlayer);
		Menu("Migrate Player").Option().Callback(Game::GiveMigrationScreen, SelectedPlayer);
		//Menu("Crash Player").Option().Callback(Game::SendPlayerCrash, SelectedPlayer);
#ifndef BO1
		Menu("Crash Player").Option().Callback(Game::SendPlayerCrash, SelectedPlayer);
		//Menu("Crash Player 2").Option().Callback(Game::SendPlayerCrash2, SelectedPlayer);
#endif
		//Menu("Send Host Error").Option().Callback(Game::SendHostError, SelectedPlayer);

		Menu("IP Address:").AddInformation(va("%i.%i.%i.%i", onlineIP.S_un.S_un_b.s_b1, onlineIP.S_un.S_un_b.s_b2, onlineIP.S_un.S_un_b.s_b3, onlineIP.S_un.S_un_b.s_b4), 2);
		Menu("Local IP:").AddInformation(va("%i.%i.%i.%i", localIP.S_un.S_un_b.s_b1, localIP.S_un.S_un_b.s_b2, localIP.S_un.S_un_b.s_b3, localIP.S_un.S_un_b.s_b4), 3);
		Menu("XUID:").AddInformation(va("000%llX", partyMember[SelectedPlayer].XUID), 4);
	}
	break;

	case CLIENT_RME_OPTIONS:
		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasExtras)) {
			Menu("Extra Options").Submenu(CLIENT_RME_EXTRAS);
		}

		Menu("God Mode").ArrayEditor(RME_Toggle[MenuOptions::dwRME_Client_GodMode], ARRAYSIZE(RME_Toggle), (int*)&MenuOptions::dwRME_Client_GodMode).Callback(ChangeGodMode, SelectedPlayer, (MenuOptions::dwRME_Client_GodMode == 0));
		//put the new option under this comment...

		Menu("Super Speed").ArrayEditor(RME_Toggle[MenuOptions::dwRME_Client_SuperSpeed], ARRAYSIZE(RME_Toggle), (int*)&MenuOptions::dwRME_Client_SuperSpeed).Callback(ChangeSuperSpeed, SelectedPlayer, (MenuOptions::dwRME_Client_SuperSpeed == 0));
		Menu("Invisibility").ArrayEditor(RME_Toggle[MenuOptions::dwRME_Client_Invisibility], ARRAYSIZE(RME_Toggle), (int*)&MenuOptions::dwRME_Client_Invisibility).Callback(ChangeInvisibility, SelectedPlayer, (MenuOptions::dwRME_Client_Invisibility == 0));
#ifndef BO1
		Menu("Infrared Vision").ArrayEditor(RME_Toggle[MenuOptions::dwRME_Client_InfraredVision], ARRAYSIZE(RME_Toggle), (int*)&MenuOptions::dwRME_Client_InfraredVision).Callback(ChangeInfraredVision, SelectedPlayer, (MenuOptions::dwRME_Client_InfraredVision == 0));
#endif
		Menu("Teleport Player To Me").Option().Callback(TeleportPlayerToMe, SelectedPlayer);
		Menu("Teleport Me To Him").Option().Callback(TeleportPlayerToHim, SelectedPlayer);
		Menu("Teleport Player Far Away").Option().Callback(TeleportPlayerFarAway, SelectedPlayer);

		Menu("Set Unlimited Ammo").Option().Callback(SetUnlimitedAmmo, SelectedPlayer);
		Menu("Take Ammo Away").Option().Callback(TakeAmmoAway, SelectedPlayer);
		
#ifndef BO2
		Menu("Set Mode").ArrayEditor(RME_SetModes[MenuOptions::dwRME_Client_SetMode], ARRAYSIZE(RME_SetModes), (int*)&MenuOptions::dwRME_Client_SetMode).Callback(ChangeMode, SelectedPlayer, MenuOptions::dwRME_Client_SetMode);
#endif
		Menu("Rename Player").Option().ChangeBoolean(&MenuOptions::bRenameSelectedClient, TRUE);

#if !defined (BO1)
		if (!CG::bIsZombies) {
			Menu("Give Weapon").ArrayEditor(RME_GiveWeapons[MenuOptions::dwRME_GiveWeaponIndex], ARRAYSIZE(RME_GiveWeapons), (int*)&MenuOptions::dwRME_GiveWeaponIndex).Callback(GiveWeapon, SelectedPlayer, RME_weaponIndexes[MenuOptions::dwRME_GiveWeaponIndex]);
		}
		else {
			Menu("Give Weapon").ArrayEditor(RME_GiveWeapons_ZM[MenuOptions::dwRME_GiveWeaponIndex], ARRAYSIZE(RME_GiveWeapons_ZM), (int*)&MenuOptions::dwRME_GiveWeaponIndex).Callback(GiveWeapon, SelectedPlayer, RME_weaponIndexes_ZM[MenuOptions::dwRME_GiveWeaponIndex]);
		}
#endif
		Menu("Take Weapon Away").Option().Callback(TakeWeapon, SelectedPlayer);
//#if !defined (BO1)
//		Menu("Take Weapon Away").Option().Callback(TakeWeapon, SelectedPlayer);
//#endif

#if defined (BO2)
		if (CG::bIsZombies) {
			Menu("Give Max Points").Option().Callback(GivePoints, SelectedPlayer);
		}
#endif

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasScreenOpts)) {
			Menu("Set Dead Screen").Option().Callback(SetSessionState, SelectedPlayer, SESS_STATE_DEAD);
			Menu("Set Spectator Mode").Option().Callback(SetSessionState, SelectedPlayer, SESS_STATE_SPECTATOR);
			Menu("Set Default State").Option().Callback(SetSessionState, SelectedPlayer, SESS_STATE_PLAYING);
		}

		//Menu("Set Model").Option().Callback(SetModel, SelectedPlayer);
		//Menu("Set Model").ArrayEditor(modelList_BO2[MenuOptions::dwRME_ModelIndex], ARRAYSIZE(modelList_BO2), (int*)&MenuOptions::dwRME_ModelIndex).Callback(SetModel, SelectedPlayer, modelListIndex_BO2[MenuOptions::dwRME_ModelIndex]);
		//Menu("Set Disconnected State").Option().Callback(SetDisconnectState, SelectedPlayer);
		//Menu("Set Client Rank").Option().Callback(SetClientRank, SelectedPlayer, 0);

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasMapOptions)) {
			Menu("Send Restart Map").Option().Callback(Send_LoadingNewMap, SelectedPlayer, false);
			Menu("Send Map Crash").Option().Callback(Send_LoadingNewMap, SelectedPlayer, true);
		}

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasKickBanOptions)) {
			Menu("Kick Player").Option().Callback(KickClient, SelectedPlayer);
			Menu("Ban Player From Lobby").Option().Callback(BanClient, SelectedPlayer);
		}

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasKillPlayer)) {
			Menu("Kill Player").Option().Callback(KillPlayer, SelectedPlayer);
		}

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasFreezeMovement)) {
			Menu("Freeze Movement ON").Option().Callback(FreezeMovement, SelectedPlayer, true);
			Menu("Freeze Movement OFF").Option().Callback(FreezeMovement, SelectedPlayer, false);
		}

#if defined (DEBUG_BUILD)
		//Menu("Kill Player").Option().Callback(KillPlayer, SelectedPlayer);
		Menu("Freeze Movement ON").Option().Callback(FreezeMovement, SelectedPlayer, true);
		Menu("Freeze Movement OFF").Option().Callback(FreezeMovement, SelectedPlayer, false);
		//Menu("Set Perks").Option().Callback(SetPerk, SelectedPlayer);
		//Menu("Unset Perks").Option().Callback(UnsetPerk, SelectedPlayer);
#endif


		//Menu("Kill Player").Option().Callback(KillPlayer, SelectedPlayer);
		//Menu("Kick Player").Option().Callback(KickClient, SelectedPlayer);
		//Menu("Ban Player From Lobby").Option().Callback(BanClient, SelectedPlayer);
		//Menu("Reset Host Stats").Option().Callback(ResetHostStats);
		//Menu("Restart Map").Option().Callback(MapRestart);

		//Menu("Send Disconnect Player").Option().Callback(Send_Disconnect, SelectedPlayer);

		break;

	case CLIENT_RME_EXTRAS:
		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasExtras)) {
			Menu("Set Disconnected State").Option().Callback(SetDisconnectState, SelectedPlayer);
#if defined (BO2)
			Menu("Fake Derank").Option().Callback(SetFakeDerank, SelectedPlayer, 0);
#endif
			Menu("Set Blackscreen").Option().Callback(SetBlackscreen, SelectedPlayer);
		}
		break;

	case RME_MENU:
		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasExtras)) {
			Menu("Extra Options").Submenu(RME_EXTRAS);
			Menu("All Clients").Submenu(RME_ALL_CLIENTS);
		}

		Menu("God Mode Loop").Toggle(&MenuOptions::bRME_GodMode);
		Menu("God Mode").ArrayEditor(RME_Toggle[MenuOptions::dwRME_GodMode], ARRAYSIZE(RME_Toggle), (int*)&MenuOptions::dwRME_GodMode).Callback(ChangeGodMode, -1, (MenuOptions::dwRME_GodMode == 0));
		Menu("Super Speed").ArrayEditor(RME_Toggle[MenuOptions::dwRME_SuperSpeed], ARRAYSIZE(RME_Toggle), (int*)&MenuOptions::dwRME_SuperSpeed).Callback(ChangeSuperSpeed, -1, (MenuOptions::dwRME_SuperSpeed == 0));
		Menu("Invisibility").ArrayEditor(RME_Toggle[MenuOptions::dwRME_Invisibility], ARRAYSIZE(RME_Toggle), (int*)&MenuOptions::dwRME_Invisibility).Callback(ChangeInvisibility, -1, (MenuOptions::dwRME_Invisibility == 0));
#ifndef BO1
		Menu("Infrared Vision").ArrayEditor(RME_Toggle[MenuOptions::dwRME_InfraredVision], ARRAYSIZE(RME_Toggle), (int*)&MenuOptions::dwRME_InfraredVision).Callback(ChangeInfraredVision, -1, (MenuOptions::dwRME_InfraredVision == 0));
#endif
		Menu("Teleport All To Me").Option().Callback(TeleportAllPlayers);
		Menu("Set Unlimited Ammo").Option().Callback(SetUnlimitedAmmo, -1);

#ifndef BO2
		Menu("Set Mode").ArrayEditor(RME_SetModes[MenuOptions::dwRME_SetMode], ARRAYSIZE(RME_SetModes), (int*)&MenuOptions::dwRME_SetMode).Callback(ChangeMode, -1, MenuOptions::dwRME_SetMode);
#endif
		Menu("Custom Rename All Clients").Option().ChangeBoolean(&MenuOptions::bCustomRenameAllClients, TRUE);

		Menu("Rename All Clients Presets").Submenu(RME_MENU_RENAME_PRESETS);

#ifndef BO1
		if (!CG::bIsZombies) {
			Menu("Give Weapon").ArrayEditor(RME_GiveWeapons[MenuOptions::dwGiveWeaponIndex], ARRAYSIZE(RME_GiveWeapons), (int*)&MenuOptions::dwGiveWeaponIndex).Callback(GiveWeapon, -1, RME_weaponIndexes[MenuOptions::dwGiveWeaponIndex]);
		}
		else {
			Menu("Give Weapon").ArrayEditor(RME_GiveWeapons_ZM[MenuOptions::dwGiveWeaponIndex], ARRAYSIZE(RME_GiveWeapons_ZM), (int*)&MenuOptions::dwGiveWeaponIndex).Callback(GiveWeapon, -1, RME_weaponIndexes_ZM[MenuOptions::dwGiveWeaponIndex]);
		}
#endif
		Menu("Make Everyone Throw Nades").Toggle(&MenuOptions::bMakeEveryoneThrowNades);

#if defined (BO2)
		if (CG::bIsZombies) {
			Menu("Give Max Points").Option().Callback(GivePoints, -1);
		}

		//if (CG::bAllowExtras) {
		//	Menu("Anti Quit").ArrayEditor(RME_AntiQuit[MenuOptions::dwAntiQuitIndex], ARRAYSIZE(RME_AntiQuit), (int*)&MenuOptions::dwAntiQuitIndex).Callback(AntiQuit, MenuOptions::dwAntiQuitIndex == 0 ? true : false);
		//}
#endif
		//Menu("Set Model").ArrayEditor(modelList_BO2[MenuOptions::dwRME_ModelIndex], ARRAYSIZE(modelList_BO2), (int*)&MenuOptions::dwRME_ModelIndex).Callback(SetModel, -1, modelListIndex_BO2[MenuOptions::dwRME_ModelIndex]);

		//Menu("Set Model").Option().Callback(SetModel, -1);

		//Menu("Move Everyone Around").Toggle(&MenuOptions::bMoveEveryoneAround);
		/*Menu("Set Disconnected State").Option().Callback(SetDisconnectState, -1);*/
		//Menu("Set Client Rank").Option().Callback(SetClientRank, -1, 0);

#if defined (DEBUG_BUILD)
		Menu("Change Map").ArrayEditor(mapNames[MenuOptions::dwRME_ChangeMapIndex].displayName, ARRAYSIZE(mapNames), (int*)&MenuOptions::dwRME_ChangeMapIndex).Callback(ChangeMap);
		//Menu("Change Game Type").ArrayEditor(gameTypes[MenuOptions::dwRME_ChangeGameTypeIndex].displayName, ARRAYSIZE(gameTypes), (int*)&MenuOptions::dwRME_ChangeGameTypeIndex).Callback(ChangeGametype);
		Menu("Reset Host Stats").Option().Callback(ResetHostStats);
		Menu("Restart Map").Option().Callback(MapRestart);
		Menu("Fast Restart").Option().Callback(FastRestart);
		Menu("Start Host Migration").Option().Callback(StartHostMigration);
		Menu("Send Host To Zombies").Option().Callback(SendHostToZombies);
		Menu("Send Host To Campagne").Option().Callback(SendHostToSingleplayer);
		Menu("Set Freeze Classes").Option().Callback(ExecuteTestPayload);
		Menu("Set Hear Everyone ON").Option().Callback(SetHearEveryone, true);
		Menu("Set Hear Everyone OFF").Option().Callback(SetHearEveryone, false);

		Menu("Walk On Countdown").Option().Callback(FreezeMovement, -1, false);
		//Menu("Walk On Countdown").Toggle(&MenuOptions::bWalkOnCountdown);

		Menu("Set Super Jump").Option().Callback(SuperJump);

#else

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasFastRestart)) {
			Menu("Fast Restart").Option().Callback(FastRestart);
		}

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasFreezeMigrate)) {
			//Menu("Fast Restart").Option().Callback(FastRestart);
			Menu("Start Host Migration").Option().Callback(StartHostMigration);
#if defined (BO2)
			Menu("Set Host Freeze Classes").Option().Callback(ExecuteTestPayload);
#endif
		}

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasChangeMap)) {
			Menu("Change Map").ArrayEditor(mapNames[MenuOptions::dwRME_ChangeMapIndex].displayName, ARRAYSIZE(mapNames), (int*)&MenuOptions::dwRME_ChangeMapIndex).Callback(ChangeMap);
		}

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasResetStats)) {
			Menu("Reset Host Stats").Option().Callback(ResetHostStats);
		}

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasHearEveryone)) {
#if defined (BO2)
			Menu("Set Hear Everyone ON").Option().Callback(SetHearEveryone, true);
			Menu("Set Hear Everyone OFF").Option().Callback(SetHearEveryone, false);
#endif
		}
#endif

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwWalkOnCountdown)) {
			Menu("Walk On Countdown").Option().Callback(FreezeMovement, -1, false);
		}

		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasUnbindControls)) {
			Menu("Unbind Host Controls").Option().Callback(UnbindHostControls);
		}

		break;

	case RME_EXTRAS:
		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasExtras)) {
			Menu("Anti Quit Auto").Toggle(&MenuOptions::bAntiQuitAuto);
			Menu("Anti Quit").ArrayEditor(RME_AntiQuit[MenuOptions::dwAntiQuitIndex], ARRAYSIZE(RME_AntiQuit), (int*)&MenuOptions::dwAntiQuitIndex).Callback(AntiQuit, MenuOptions::dwAntiQuitIndex == 0 ? true : false);
#if !defined (BO1)
			Menu("Fake Derank All").Option().Callback(SetFakeDerankAll);
#endif
		}
		break;

	case RME_ALL_CLIENTS:
		if (Utilities::ReadBoolFromDWORD(Global.sunsetInfo.dwHasExtras)) {
			//Menu("God Mode").ArrayEditor(RME_Toggle[MenuOptions::dwRME_AllClients_GodMode], ARRAYSIZE(RME_Toggle), (int*)&MenuOptions::dwRME_AllClients_GodMode).Callback(GodModeEveryone, (MenuOptions::dwRME_AllClients_GodMode == 0));
			Menu("God Mode ON").ArrayEditor(RME_Teams[MenuOptions::dwRME_AllClients_GodMode_ON], ARRAYSIZE(RME_Teams), (int*)&MenuOptions::dwRME_AllClients_GodMode_ON).Callback(GodModeEveryone, true, MenuOptions::dwRME_AllClients_GodMode_ON == 0 ? true : false);
			Menu("God Mode OFF").ArrayEditor(RME_Teams[MenuOptions::dwRME_AllClients_GodMode_OFF], ARRAYSIZE(RME_Teams), (int*)&MenuOptions::dwRME_AllClients_GodMode_OFF).Callback(GodModeEveryone, false, MenuOptions::dwRME_AllClients_GodMode_OFF == 0 ? true : false);
			
			Menu("Super Speed ON").ArrayEditor(RME_Teams[MenuOptions::dwRME_AllClients_SuperSpeed_ON], ARRAYSIZE(RME_Teams), (int*)&MenuOptions::dwRME_AllClients_SuperSpeed_ON).Callback(SuperSpeedEveryone, true, MenuOptions::dwRME_AllClients_SuperSpeed_ON == 0 ? true : false);
			Menu("Super Speed OFF").ArrayEditor(RME_Teams[MenuOptions::dwRME_AllClients_SuperSpeed_OFF], ARRAYSIZE(RME_Teams), (int*)&MenuOptions::dwRME_AllClients_SuperSpeed_OFF).Callback(SuperSpeedEveryone, false, MenuOptions::dwRME_AllClients_SuperSpeed_OFF == 0 ? true : false);
			
			Menu("Blackscreen").ArrayEditor(RME_Teams[MenuOptions::dwRME_AllClients_Blackscreen], ARRAYSIZE(RME_Teams), (int*)&MenuOptions::dwRME_AllClients_Blackscreen).Callback(BlackscreenEveryone, MenuOptions::dwRME_AllClients_Blackscreen == 0 ? true : false);
#if !defined (BO1)
			//Menu("Blackscreen").Option().Callback(BlackscreenEveryone);
			//Menu("Take Weapons Away").Option().Callback(TakeWeaponForEveryone);

			/*Menu("Blackscreen").ArrayEditor(RME_Teams[MenuOptions::dwRME_AllClients_Blackscreen], ARRAYSIZE(RME_Teams), (int*)&MenuOptions::dwRME_AllClients_Blackscreen).Callback(BlackscreenEveryone, MenuOptions::dwRME_AllClients_Blackscreen == 0 ? true : false);*/
#endif
			Menu("Take Weapons Away").ArrayEditor(RME_Teams[MenuOptions::dwRME_AllClients_TakeWeaponsAway], ARRAYSIZE(RME_Teams), (int*)&MenuOptions::dwRME_AllClients_TakeWeaponsAway).Callback(TakeWeaponForEveryone, MenuOptions::dwRME_AllClients_TakeWeaponsAway == 0 ? true : false);
			//Menu("Take Ammo Away").Option().Callback(TakeAmmoAwayAll);
			Menu("Take Ammo Away").ArrayEditor(RME_Teams[MenuOptions::dwRME_AllClients_TakeAmmoAway], ARRAYSIZE(RME_Teams), (int*)&MenuOptions::dwRME_AllClients_TakeAmmoAway).Callback(TakeAmmoAwayAll, MenuOptions::dwRME_AllClients_TakeAmmoAway == 0 ? true : false);
		}
		break;

	case RME_MENU_RENAME_PRESETS:
		Menu("How's that possible non host?").Option().Callback(RenameAll, va("^%iHow's that possible non host?", rand() % 7));
		Menu("Shield Website").Option().Callback(RenameAll, va("^%ishieldstealth.com", rand() % 7));
		Menu("Shield Discord Link").Option().Callback(RenameAll, va("^%ihttps://discord.gg/RuhuDKaH8E", rand() % 7));
		Menu("Fuck off retard").Option().Callback(RenameAll, va("^%iFuck off retard", rand() % 7));
#if !defined (BO1)
		Menu("Set Freezing Name").Option().Callback(RenameAll, "^H");
#endif
		break;
	}
	Menu(NULL).ResetVars();
}

void Menu::HandleOptions() {
	if (CG::bInGameCached && CG::bLobbyInitialized) {
		Game::ToggleNoRecoil(&MenuOptions::bNoRecoil);
		Game::ToggleNoSway(&MenuOptions::bNoSway);
		Game::ToggleNoFlinch(&MenuOptions::bNoFlinch);
#if defined (BO2)
		Game::ToggleLaser(&MenuOptions::bLaser);
		Game::SetNoBob(MenuOptions::bNoBob ? 0.0f : 0.024639944f);
#elif defined (BO1) || defined (BO1_ZM)
		Game::SetNoBob(MenuOptions::bNoBob ? 0.0f : 0.25f);
#endif
#ifndef BO1_ZM
		Game::ToggleVSAT(&MenuOptions::bRadar);
#endif
		Game::ToggleThirdPerson(&MenuOptions::bThirdPerson);
		Game::SetThirdPersonRange(MenuOptions::fThirdPersonRange);
		Game::SetFOV(MenuOptions::fFieldOfView);
		Game::SetOverheadNames(MenuOptions::bDrawNames);
#if defined (BO2)
		Game::SetNoBulletImpacts(&MenuOptions::bNoBulletImpacts);
#endif
	}
	Game::SetJoinInProgressAllowed(&MenuOptions::bJoinInProgressAllowed);
}

bool bEnteredIngame = false;
bool bEnteredClientList = false;

void Menu::Update() {
	Structs::UpdateStructures();

	if (!Game::CheckInGame()) {
		if (bEnteredIngame) {
			for (int i = 0; i < 1024; i++) {
				pInfo[i].Whitelisted = FALSE;
				pInfo[i].Prioritized = FALSE;
				pInfo[i].Wallbangable = FALSE;
				pInfo[i].Visible = FALSE;
				pInfo[i].Bone = nullptr;
			}

			bEnteredIngame = false;
		}
	}
	else {
		if (!bEnteredIngame)
			bEnteredIngame = true;
	}

	HandleInput();
	HandleOptions();

	//all of this commented out shit would normally break the client list > you crash if you try viewing it in a running game. This is the only change made by #Dev___1_
	//if (MenuItems.submenu == CLIENT_LIST) {
	//	if (!bEnteredClientList) {

		//	for (int i = 0; i < 18; i++) {
		//		playerMenuDetection[i].bDetected = false;
		//		if (partyMember[i].XUID != 0 && partyMember[i].gamertag[0] != NULL) {
			//		netAdr_t netAdr = netAdr_t();
				//	memset(&netAdr, 0x00, sizeof(netAdr_t));

					//Live_GetPlayerNetAddr(&netAdr, Live_GetCurrentSession(), i);

					//NET_OutOfBandPrint(NS_CLIENT1, netAdr, "Xbox360");
			//	}
		//	}
			//bEnteredClientList = true;
		//}
//	}
	//else {
		//if (bEnteredClientList)
			//bEnteredClientList = false;
	//}

	if (Sunset::bMenuOpen)
		HandleMenu();

	if (Sunset::bWatermark) {
		char pInstructionText[250];
		float yInstruction = 711.0f;
#if defined (BO2)
		RtlSnprintf(pInstructionText, 250, "Press ^BXENONButtontrigL^ & ^BXENONButtondpadL^ to %s ^1%s^7", !Sunset::bMenuOpen ? "Open" : "Close", ENGINE_NAME);
#elif defined (BO1) || defined (BO1_ZM)
		RtlSnprintf(pInstructionText, 250, "Press \x0016 & \x0012 to %s ^1%s^7", !Sunset::bMenuOpen ? "Open" : "Close", ENGINE_NAME);
#endif
		float fInstructionWidth = (Game::GetTextWidth(pInstructionText, Sunset::dwSmallFont) * Sunset::fInstructionScale);

		Render::DrawShader(15.0f, 680.0f, (fInstructionWidth + 10.0f), Game::GetFontHeight(Sunset::dwSmallFont, Sunset::fInstructionScale), Sunset::dwWhite, colorBackground);
		Render::DrawText(pInstructionText, 20.0f, yInstruction, Sunset::dwSmallFont, Sunset::fInstructionScale, colorWhite);
	}
}