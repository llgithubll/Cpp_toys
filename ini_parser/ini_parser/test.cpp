#include <iostream>
#include <fstream>
#include <cassert>
#include "ini_parser.h"

using namespace std;
using namespace ini;

int main()
{
	IniFile ini;
	if (ini.load("test_ansic.ini") != -1) {
		for (auto &e : ini.getSections()) {
			cout << e << endl;
		}

		string sect = "Translation";
		assert(ini.getValue(sect, "FILEVER") == "3160");
		assert(ini.getIntValue(sect, "FILEVER") == 3160);
		assert(ini.getDoubleValue(sect, "FILEVER") == 3160.0);
		assert(ini.hasSection(sect) == true);

		for (auto &e : ini.getValuesComments(sect, "FILEVER")) {
			cout << e.first << e.second << endl;
		}

		ini.setValue(sect, "DD", "12.5", "DoubleValue");
		assert(ini.hasKey(sect, "DD") == true);
		assert(ini.getDoubleValue(sect, "DD") == 12.5);
		ini.deleteKey(sect, "DD");
		assert(ini.hasKey(sect, "DD") == false);
	}
	ini.saveas("test_result.ini");
	system("pause");
	return 0;
}