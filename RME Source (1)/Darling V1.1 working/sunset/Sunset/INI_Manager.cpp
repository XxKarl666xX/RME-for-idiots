#include "stdafx.h"
#include <fstream>
#include <iomanip>

#define INI_PATH "HDD:\\Sunset.ini"

unsigned char EngineAscii[201] = {
	0x5B, 0x43, 0x75, 0x73, 0x74, 0x6F, 0x6D, 0x20, 0x47, 0x61, 0x6D, 0x65,
	0x72, 0x74, 0x61, 0x67, 0x73, 0x5D, 0x0D, 0x0A, 0x53, 0x6C, 0x6F, 0x74,
	0x20, 0x31, 0x20, 0x3D, 0x20, 0x22, 0x45, 0x6D, 0x70, 0x74, 0x79, 0x22,
	0x0D, 0x0A, 0x53, 0x6C, 0x6F, 0x74, 0x20, 0x32, 0x20, 0x3D, 0x20, 0x22,
	0x45, 0x6D, 0x70, 0x74, 0x79, 0x22, 0x0D, 0x0A, 0x53, 0x6C, 0x6F, 0x74,
	0x20, 0x33, 0x20, 0x3D, 0x20, 0x22, 0x45, 0x6D, 0x70, 0x74, 0x79, 0x22,
	0x0D, 0x0A, 0x53, 0x6C, 0x6F, 0x74, 0x20, 0x34, 0x20, 0x3D, 0x20, 0x22,
	0x45, 0x6D, 0x70, 0x74, 0x79, 0x22, 0x0D, 0x0A, 0x53, 0x6C, 0x6F, 0x74,
	0x20, 0x35, 0x20, 0x3D, 0x20, 0x22, 0x45, 0x6D, 0x70, 0x74, 0x79, 0x22,
	0x0D, 0x0A, 0x53, 0x6C, 0x6F, 0x74, 0x20, 0x36, 0x20, 0x3D, 0x20, 0x22,
	0x45, 0x6D, 0x70, 0x74, 0x79, 0x22, 0x0D, 0x0A, 0x53, 0x6C, 0x6F, 0x74,
	0x20, 0x37, 0x20, 0x3D, 0x20, 0x22, 0x45, 0x6D, 0x70, 0x74, 0x79, 0x22,
	0x0D, 0x0A, 0x53, 0x6C, 0x6F, 0x74, 0x20, 0x38, 0x20, 0x3D, 0x20, 0x22,
	0x45, 0x6D, 0x70, 0x74, 0x79, 0x22, 0x0D, 0x0A, 0x53, 0x6C, 0x6F, 0x74,
	0x20, 0x39, 0x20, 0x3D, 0x20, 0x22, 0x45, 0x6D, 0x70, 0x74, 0x79, 0x22,
	0x0D, 0x0A, 0x53, 0x6C, 0x6F, 0x74, 0x20, 0x31, 0x30, 0x20, 0x3D, 0x20,
	0x22, 0x45, 0x6D, 0x70, 0x74, 0x79, 0x22, 0x0D, 0x0A
};

string RemoveChars(const string& source, const string& chars, BOOL ignoreBetweenQuotes) {
	string result = "";
	BOOL betweenQuotes = FALSE;
	for (unsigned int i = 0; i < source.length(); i++) {

		if (ignoreBetweenQuotes) {
			if (source[i] == '"' && !betweenQuotes) {
				betweenQuotes = TRUE;
			}
			else if (source[i] == '"' && betweenQuotes) {
				if (i > 0 && source[i - 1] == '\\') {
					// Skip this quote... it's been escaped so it must be within the string we're checking, not the end of it.
				}
				else {
					betweenQuotes = FALSE;
				}
			}
		}

		BOOL foundany = FALSE;
		for (unsigned int j = 0; j < chars.length() && !foundany; j++) {
			foundany = (source[i] == chars[j]) && !betweenQuotes;
		}
		if (!foundany) {
			result += source[i];
		}
	}
	return result;
}

BOOL ReadArray(string optionValue, DWORD dwSize, DWORD currentIndex) {
	string origValue = optionValue;
	BOOL success = FALSE;

	DWORD dwMaxLength = (DWORD)33;
	if (optionValue.size() > 0) {
		// Make sure the option is wrapped in quotes, otherwise dont even try to read it
		if (optionValue[0] == '\"' && optionValue[optionValue.size() - 1] == '\"') {
			if (optionValue.size() - 2 > dwMaxLength) {
				//DbgPrint("Custom string %s was larger than expected size %i! Truncating %i chars!", optionName.c_str(), dwMaxLength, (optionValue.size() - 2) - dwMaxLength);
			}
			else {
				optionValue[optionValue.size() - 1] = 0;
			}
			memcpy(pModdedGamertags[currentIndex], optionValue.c_str() + 1, 33);
			DbgPrint("Read option %s at %i", optionValue.c_str(), currentIndex);
		}
	}

	return success;
}

void INI_Manager::Load_INI() {
	std::filebuf fb;
	if (fb.open("HDD:\\Sunset.ini", std::ios::in)) {
		istream is(&fb);
		string opt;
		string currentSection;

		DWORD currentIndex = 0;

		while (getline(is, opt)) {
			// Remove all spaces from our config line
			opt = RemoveChars(opt, " ", TRUE);

			if (opt.length() > 0 && opt.length() < 100) {
				// If we're at the start of a section
				if (opt.c_str()[0] == '[') {
					opt = RemoveChars(opt, "[]", FALSE);
					//DbgPrint("Parsing section %s", opt.c_str());
					currentSection = opt;
					if (currentSection != "")
						DbgPrint("Reading section %s!", currentSection.c_str());
				}
				else {
					size_t pos = opt.find('=');
					if (pos != string::npos) {
						string name = opt.substr(0, pos);
						string value = opt.substr(pos + 1);
						opt.clear();

						//DbgPrint("name %s value %s", name.c_str(), value.c_str());

						ReadArray(value, 1, currentIndex);
						currentIndex++;
					}
				}
			}
		}


		fb.close();
	}
}

void SaveArray(ofstream* sEngineIni, DWORD currentIndex) {
	*sEngineIni << pModdedGTs[currentIndex] << " = ";

	PCHAR pText = pModdedGamertags[currentIndex];
	if (!Utilities::Compare(pText, "")) {
		*sEngineIni << "\"" << pText << "\"" << endl;
		//DbgPrint("Option %s[%i] saved as %s", option->pSaveName, i, pText);
		DbgPrint("Gamertag %s saved as %s", pText, pModdedGTs[currentIndex]);
	}
	else {
		DbgPrint("String was blank! Skipping!");
	}
}

void INI_Manager::Save_INI() {
	ofstream sEngineIni;
	sEngineIni.open("HDD:\\Sunset.ini.tmp", ios::app);
	if (sEngineIni.is_open()) {
		//// Write our engine ascii to the ini file
		//sEngineIni << EngineAscii << "\r\n";

		//// Then close it to disable binary mode
		//sEngineIni.flush();
		//sEngineIni.close();

		//// Reopen the file again as a text file in append mode
		//sEngineIni.open("HDD:\\Sunset.ini.tmp", ios::app);

		if (sEngineIni.is_open()) {
			DWORD dwNumOptionsSaved = 0;
			sEngineIni.setf(std::ios::fixed);

			sEngineIni << endl << "[" << "Custom Gamertags" << "]" << endl;

			for (int i = 0; i < 10; i++) {
				SaveArray(&sEngineIni, i);
			}

			sEngineIni.close();

			DeleteFile("HDD:\\Sunset.ini");
			MoveFile("HDD:\\Sunset.ini.tmp", "HDD:\\Sunset.ini");

			DbgPrint("Successfully saved config!");
		}
	}
}