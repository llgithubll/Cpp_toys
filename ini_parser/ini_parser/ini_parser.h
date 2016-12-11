#pragma once

#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;

namespace ini {
	const char NEW_LINE = '\n';

	struct IniItem {
		string comment;
		string key;
		string value;
	};
	struct IniSection {
		typedef vector<IniItem>::iterator iterator;
		iterator begin() {
			return items.begin();
		}
		iterator end() {
			return items.end();
		}
		string comment;
		string name;
		vector<IniItem> items;
	};
	class IniFile {
	public:
		IniFile() = default;
	public:
		// Load a *.ini file
		int load(const string &fname);
		// Save the content and change to this file
		int save();
		// Save the content and change to another file
		int saveas(const string &fname);

		// Get the first value of key in section, return string value
		string getStringValue(const string &section, const string &key);
		// Get the first value of key in section, return int value
		int getIntValue(const string &section, const string &key);
		// Get the first value of key in section, reutrn double value
		double getDoubleValue(const string &section, const string &key);

		// Get the first value of key, return value
		string getValue(const string &section, const string &key);
		// Get the first value and comment of key, return pair<value, comment> 
		std::pair<string, string> getValueComment(const string &section, const string &key);

		// Get all values of key in section, return values in vector
		vector<string> getValues(const string &section, const string &key);
		// Get all values, comment of key in section, return pair<value, comment> in vector
		vector<std::pair<string, string>> getValuesComments(const string &section, const string &key);
		// Get all sections name from *.ini file, return it in vector
		vector<string> getSections();

		bool hasSection(const string &section);
		bool hasKey(const string &section, const string &key);

		// Set value(comment) of key in section
		int setValue(const string &section, const string &key,
			const string &value, const string &comment = "");
		// Delete the given section
		void deleteSection(const string &section);
		// Delete the given key in section
		void deleteKey(const string &section, const string &key);

	public:
		typedef map<string, IniSection>::iterator iterator;
		iterator begin() {
			return sections.begin();
		}
		iterator end() {
			return sections.end();
		}
	private:
		void strip(string &str);
		bool parser(string &line, string &key, string &value, string &comment);
	private:
		string filename;
		map<string, IniSection> sections;
	};
}