#pragma once

struct FastReload_s {
	BOOL WasReloading;
	BOOL YButtonPressed;
	int yyCount;
};

class Hooking {
public:
	static void LoadHooks();
	static void UnloadHooks();
};

extern FastReload_s FastReload;