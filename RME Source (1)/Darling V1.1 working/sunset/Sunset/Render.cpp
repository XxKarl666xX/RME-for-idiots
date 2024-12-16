#include "stdafx.h"

void Render::Initialize() {
	Sunset::dwWhite = Material_RegisterHandle("white", 7);
#if defined (BO2)
	Sunset::dwSmallFont = CL_RegisterFont("fonts/720/smallFont", 3);
#elif defined (BO1) || defined (BO1_ZM)
	Sunset::dwSmallFont = CL_RegisterFont("fonts/smallFont", 3);
#endif

	int aspect;
	((VOID(*)(...))decryptDWORD(Security->addrs.CL_GetScreenDimensions))(&Sunset::dwScreenMaxX, &Sunset::dwScreenMaxY, &aspect);

	Sunset::dwScreenCenterX = Sunset::dwScreenMaxX / 2;
	Sunset::dwScreenCenterY = Sunset::dwScreenMaxY / 2;

	Global.MenuReady = TRUE;
}

void Render::DrawText(char* text, float x, float y, DWORD font, float fontScale, color color) {
	R_AddCmdDrawText(text, 0x7FFFFFFF, font, x, y, fontScale, fontScale, 0, color, 0);
}

void Render::DrawShader(float x, float y, float width, float height, DWORD materials, color color) {
	R_AddCmdDrawStretchPic(x, y, width, height, 1.0f, 1.0f, 1.0f, 1.0f, color, materials);
}

void Render::DrawShaderWithOutline(float x, float y, float w, float h, color backgroundColor, color outlineColor, float lineWidth) {
	DrawShader(x, y, lineWidth, h, Sunset::dwWhite, outlineColor);
	DrawShader(x + lineWidth, y, w - lineWidth, lineWidth, Sunset::dwWhite, outlineColor);
	DrawShader(x + lineWidth, y + h - lineWidth, w - (lineWidth * 2), lineWidth, Sunset::dwWhite, outlineColor);
	DrawShader(x + w - lineWidth, y + lineWidth, lineWidth, h - lineWidth, Sunset::dwWhite, outlineColor);

	DrawShader(x + lineWidth, y + lineWidth, w - (lineWidth * 2), h - (lineWidth * 2), Sunset::dwWhite, backgroundColor);
}

void Render::DrawShaderOnlyRight(float x, float y, float w, float h, color backgroundColor, color lineColor, float lineWidth) {
	DrawShader(x + w - lineWidth, y + lineWidth, lineWidth, h - lineWidth, Sunset::dwWhite, lineColor);

	DrawShader(x + lineWidth, y + lineWidth, w - (lineWidth * 2), h - (lineWidth * 2), Sunset::dwWhite, backgroundColor);
}

void Render::DrawCheckBox(float centerX, float centerY, color checkColor, color outlineBoxColor, bool isToggled) {
	DrawShader(centerX, centerY, 12, 1, Sunset::dwWhite, outlineBoxColor);
	DrawShader(centerX, centerY + 12, 12, 1, Sunset::dwWhite, outlineBoxColor);
	DrawShader(centerX, centerY, 1, 12, Sunset::dwWhite, outlineBoxColor);
	DrawShader(centerX + 12, centerY, 1, 13, Sunset::dwWhite, outlineBoxColor);

	if (isToggled)
	{
		DrawShader(centerX + 1, centerY + 1, 11, 11, Sunset::dwWhite, checkColor);
	}
}

void Render::DrawColorPreview(float centerX, float centerY, color Color) {
	DrawShader(centerX, centerY, 12, 1, Sunset::dwWhite, Color);
	DrawShader(centerX, centerY + 12, 12, 1, Sunset::dwWhite, Color);
	DrawShader(centerX, centerY, 1, 12, Sunset::dwWhite, Color);
	DrawShader(centerX + 12, centerY, 1, 13, Sunset::dwWhite, Color);

	DrawShader(centerX + 1, centerY + 1, 11, 11, Sunset::dwWhite, Color);
}

void Render::DrawLine(float X1, float Y1, float X2, float Y2, float width, color color, DWORD Shader) {
	float X, Y, Angle, L1, L2, H1;
	H1 = Y2 - Y1;
	L1 = X2 - X1;
	L2 = sqrt(L1 * L1 + H1 * H1);
	X = X1 + ((L1 - L2) / 2);
	Y = Y1 + (H1 / 2);
	Angle = (float)atan(H1 / L1) * (180.f / 3.14159265358979323846f);
	CG_DrawRotatedPicPhysical(decryptDWORD(Security->addrs.ScreenPlacement), X, Y, L2, width, Angle, color, Shader);
}

void Render::DrawCenteredText(char* text, float x, float y, float width, DWORD font, float fontScale, color color) {
	float textLength = ((Game::GetTextWidth(text, font) / 2) * fontScale);

	R_AddCmdDrawText(text, 0x7FFFFFFF, font, (x + (width / 2) - textLength), y, fontScale, fontScale, 0, color, 0);
}

float Render::CalculateXValueWithAlign(float x, char* text, DWORD font, float scale, eJustify justify) {
	if (justify == JustifyLeft)
		return x;
	if (justify == JustifyCenter)
		return x - ((Game::GetTextWidth(text, font) / 2) * scale);
	if (justify == JustifyRight)
		return x - (Game::GetTextWidth(text, font) * scale);

	return 0;
}

void Render::DrawString(char* text, float x, float y, DWORD font, float scale, color color, eJustify justify) {
	float defaultX = CalculateXValueWithAlign(x, text, font, scale, justify);

	DrawText(text, defaultX, y, font, scale, color);
}

void Render::MakeLines(vec3 Origin, FLOAT X1, FLOAT Y1, FLOAT Z1, FLOAT X2, FLOAT Y2, FLOAT Z2, color Color, int Size) {
	vec2 Screen1, Screen2;
	vec3 Origin1(Origin.x + X1, Origin.y + Y1, (Origin.z + Z1) - 4);
	vec3 Origin2(Origin.x + X2, Origin.y + Y2, (Origin.z + Z2) - 4);
	if (Game::WorldToScreen(Origin1, Screen1) && Game::WorldToScreen(Origin2, Screen2)) {
		Render::DrawLine(Screen1.x, Screen1.y, Screen2.x, Screen2.y, Size, Color, Sunset::dwWhite);
	}
}

void Render::DrawESPLine(vec3 center, float x1, float y1, float z1, float x2, float y2, float z2, color color) {
	vec3 pointPos1 = vec3(center.x + x1, center.y + y1, center.z + z1);
	vec3 pointPos2 = vec3(center.x + x2, center.y + y2, center.z + z2);
	vec2 xy1, xy2;

	if (Game::WorldToScreen(pointPos1, xy1) && Game::WorldToScreen(pointPos2, xy2)) {
		Render::DrawLine(xy1.x, xy1.y, xy2.x, xy2.y, 1, color, Sunset::dwWhite);
	}
}

void Render::DrawBox(FLOAT X, FLOAT Y, FLOAT W, FLOAT H, color Color) {
	R_AddCmdDrawStretchPic(X, Y, W, H, 4, 4, 1, 1, Color, Sunset::dwWhite);
}

void Render::DrawTextWithBackground(char *text, float x, float y, DWORD font, float fontScale, color color) {
	DrawShader(((x - 2) - (Game::GetTextWidth(text, font) / 2) * fontScale), (y - Game::GetFontHeight(font, fontScale)) + 1, ((Game::GetTextWidth(text, font) * fontScale) + 4), Game::GetFontHeight(font, fontScale), Sunset::dwWhite, colorBackground);
	DrawText(text, (x - (Game::GetTextWidth(text, font) / 2) * fontScale), y, font, fontScale, color);
}

void Render::DrawTopRightText(char* text, float x, float y, DWORD font, float fontScale, color color, int Index) {
	float textWidth = (Game::GetTextWidth(text, font) * fontScale);

	DrawShader((x - textWidth) - 5, (y + (Index * 26.0f)), (textWidth + 10), 24.0f, Sunset::dwWhite, colorBackground);

	DrawText(text, (x - textWidth), ((y + (Index * 26.0f)) + 22), font, fontScale, color);
}