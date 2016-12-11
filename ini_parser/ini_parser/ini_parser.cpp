#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <algorithm>
#include "ini_parser.h"

using std::pair;
using std::string;
using std::map;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::cerr;
using std::endl;

namespace ini {
	// Load a *.ini file
	int IniFile::load(const string &fname) {
		ifstream input(fname);
		filename = fname;

		if (input) {
			long line_counter = 0;
			string line;
			IniSection section;
			sections[""] = section;
			string curr_sect_name = "";
			string comment = "";

			while (getline(input, line)) {
				line_counter++;
				strip(line);
				if (line.length() <= 0) {
					continue;
				}

				if (line[0] == '#' || line[0] == ';') { // comment
					comment += (comment == "") ? line : NEW_LINE + line;
				}
				else if (line[0] == '[') { // section
					auto rb = line.find_first_of(']');
					if (rb == string::npos) {
						cerr << "Lack of ']' to match " << line_counter << endl;
						return -1;
					}
					if (rb == 1) {
						cerr << "Empty section " << line_counter << endl;
						return -1;
					}

					string content = line.substr(1, rb - 1); // rb - 1 is content length
					if (sections.find(content) != sections.end()) {
						cerr << "Section: " << line << " already exists " << line_counter << endl;
						return -1;
					}

					IniSection sect;
					sect.name = content;
					sect.comment = comment;
					curr_sect_name = sect.name;
					sections[curr_sect_name] = sect;
					comment = "";
				}
				else { // key-value
					string k, v, c;
					if (parser(line, k, v, c)) {
						IniItem item;
						item.key = k;
						item.value = v;
						item.comment = comment + c;
						comment = "";
						sections[curr_sect_name].items.push_back(item);
					}
					else {
						cerr << "Can not parser key-value " << line_counter << endl;
					}
				}
			}
			// if any comment left, add it to last section comment
			sections[curr_sect_name].comment += NEW_LINE + comment;
		}
		else {
			cerr << "No such file(" << fname << ")" << endl;
			return -1;
		}

		input.close();
		return 0;
	}

	// Save the content and change to this file
	int IniFile::save() {
		return saveas(filename);
	}
	// Save the content and change to another file
	int IniFile::saveas(const string &fname) {
		string data;

		for (auto &sect : sections) {
			if (sect.second.comment != "") {
				data += sect.second.comment + NEW_LINE;
			}

			if (sect.first != "") {
				data += string("[") + sect.first + string("]") + NEW_LINE;
			}

			for (auto &item : sect.second.items) {
				if (item.comment != "") {
					data += item.comment + NEW_LINE;
				}

				data += item.key + "=" + item.value + NEW_LINE;
			}
		}

		ofstream out(fname);
		out << data << endl;
		out.close();
		return 0;
	}

	// Get the first value of key in section, return string value
	string IniFile::getStringValue(const string &section, const string &key) {
		return getValue(section, key);
	}
	// Get the first value of key in section, return int value
	int IniFile::getIntValue(const string &section, const string &key) {
		return std::stoi(getValue(section, key));
	}
	// Get the first value of key in section, reutrn double value
	double IniFile::getDoubleValue(const string &section, const string &key) {
		return std::stod(getValue(section, key));
	}

	// Get the first value of key, return value
	string IniFile::getValue(const string &section, const string &key) {
		string value;
		if (sections.find(section) != sections.end()) {
			for (auto &item : sections[section].items) {
				if (item.key == key) {
					return item.value;
				}
			}
		}
		return value;
	}
	// Get the first value and comment of key, return pair<value, comment> 
	pair<string, string> 
	IniFile::getValueComment(const string &section, const string &key) {
		string value, comment;
		if (sections.find(section) != sections.end()) {
			for (auto &item : sections[section].items) {
				if (item.key == key) {
					return std::make_pair(item.value, item.comment);
				}
			}
		}
		return std::make_pair(value, comment);
	}


	// Get all values of key in section, return values in vector
	vector<string> 
	IniFile::getValues(const string &section, const string &key) {
		vector<string> values;
		if (sections.find(section) != sections.end()) {
			for (auto &item : sections[section].items) {
				if (item.key == key) {
					values.push_back(item.value);
				}
			}
		}
		return values;
	}
	// Get all values, comment of key in section, return pair<value, comment> in vector
	vector<pair<string, string>>
	IniFile::getValuesComments(const string &section, const string &key) {
		vector<pair<string, string>> va_co;
		if (sections.find(section) != sections.end()) {
			for (auto &item : sections[section].items) {
				if (item.key == key) {
					va_co.push_back(std::make_pair(item.value, item.comment));
				}
			}
		}
		return va_co;
	}
	// Get all sections name from *.ini file, return it in vector
	vector<string> IniFile::getSections() {
		vector<string> all_sect;
		for (auto &sect : sections) {
			if (sect.first != "") {
				all_sect.push_back(sect.first);
			}
		}
		return all_sect;
	}

	bool IniFile::hasSection(const string &section) {
		return sections.find(section) != sections.end();
	}
	bool IniFile::hasKey(const string &section, const string &key) {
		if (sections.find(section) != sections.end()) {
			for (auto &item : sections[section].items) {
				if (item.key == key) {
					return true;
				}
			}
		}
		return false;
	}

	// Set value(comment) of key in section
	int IniFile::setValue(const string &section, const string &key,
							const string &value, const string &comment) {
		string comt = comment;
		if (comt != "") {
			comt = "#" + comt;
		}

		if (sections.find(section) != sections.end()) {
			for (auto &item : sections[section].items) {
				if (item.key == key) {
					item.value = value;
					item.comment = comt;
					return 1;
				}
			}
			// Not found key
			IniItem item;
			item.key = key;
			item.value = value;
			item.comment = comt;
			sections[section].items.push_back(item);
		}
		else {
			// Not found section
			IniSection sect;
			sect.name = section;
			IniItem item;
			item.key = key;
			item.value = value;
			item.comment = comt;
			sect.items.push_back(item);
			sections[sect.name] = sect;
		}
		return 1;
	}

	// Delete the given section
	void IniFile::deleteSection(const string &section) {
		sections.erase(section);
	}
	// Delete the given key in section
	void IniFile::deleteKey(const string &section, const string &key) {
		if (sections.find(section) != sections.end()) {
			IniSection sect;
			sect.name = section;
			sect.comment = sections[section].comment;
			
			for (auto it = sections[section].begin();
						it != sections[section].end(); ++it) {
				if (it->key != key) {
					sect.items.push_back(*it);
				}
			}
			deleteSection(section);
			sections[sect.name] = sect;
		}
	}

	void IniFile::strip(string& str) {
		str.erase(0, str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" ") + 1);
	}

	bool IniFile::parser(string &line, string &key, string &value, string &comment) {
		auto index_c = line.find_first_of("#;");
		if (index_c == string::npos) {
			comment = "";
		}
		else {
			comment = line.substr(index_c);
			line = line.substr(0, index_c);
		}

		auto index_kv = line.find_first_of("=:");
		if (index_kv == string::npos) {
			return false;
		}
		else {
			key = line.substr(0, index_kv);
			strip(key);
			value = line.substr(index_kv + 1);
			strip(value);
		}
		return true;
	}
}