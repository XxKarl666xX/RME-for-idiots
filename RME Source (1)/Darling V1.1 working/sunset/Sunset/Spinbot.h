#pragma once

extern char* SpinbotTypesX[3];
extern char* SpinbotTypesY[4];

extern vec2 spinAngles;

class Spinbot {
public:
	static int FindBestAntiAimTarget();
	static void HandleSpinbot(usercmd_s* currentCmd);
};