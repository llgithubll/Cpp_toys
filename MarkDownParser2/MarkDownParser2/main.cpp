// 来自 shiyanlou ，自个学习用
#include <fstream>
#include <string>
#include <iostream>
#include <cctype>
#include "mdstransform.h"

int main()
{
	std::cout << "An Markdown to HTML converter written in C++, and it will generate targets for headers." << std::endl;
	std::cout << "input the full filename in current folder you want to transform,eg:\"test.md\" (ctrl+z to end)" << std::endl;
	std::string filename;

	while (std::cin >> filename) {
		char m = tolower(filename[filename.size() - 2]);
		char d = tolower(filename[filename.size() - 1]);
		if (filename[filename.size() - 3] == '.' && m == 'm' && d == 'd') {
			MarkdownTransform transformer(filename);

			std::string head = "<!DOCTYPE html><html><head>\
								<meta charset=\"utf-8\">\
								<title>Markdown</title>\
								<link rel=\"stylesheet\" href=\"github-markdown.css\">\
								</head><body><article class=\"markdown-body\">";
			std::string end = "</article></body></html>";

			string outFilename = filename.substr(0, filename.size() - 3) + ".html";
			ofstream out;
			out.open(outFilename);
			out << head + transformer.getTableOfContents() + transformer.getContents() + end;
			out.close();
			std::cout << "transformer success" << std::endl;
		}
		else {
			std::cout << "wrong filename" << std::endl;
		}

	}

	return 0;
}