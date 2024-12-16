#pragma once

class Visuals {
public:
	static void DrawESPBox(color color, int Index, int currentType);
	static void DrawBones(int clientNum, color color);
	static void DrawESP(int clientNum, color colorESP);
	static void HandleDebugInfo();
	static void HandleVisuals();
};