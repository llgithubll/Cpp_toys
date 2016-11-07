#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>

#include <cstdio>

using namespace std;

#define maxLength 10000

// �ʷ��ؼ���ö��
enum {
	nul			= 0,
	paragraph	= 1,
	href		= 2,
	ul			= 3,
	ol			= 4,
	li			= 5,
	em			= 6,
	strong		= 7,
	hr			= 8,
	br			= 9,
	image		= 10,
	quote		= 11,
	h1			= 12,
	h2			= 13,
	h3			= 14,
	h4			= 15,
	h5			= 16,
	h6			= 17,
	blockcode	= 18,
	code		= 19
};
// HTML ǰ�ñ�ǩ
const string frontTag[] = {
	"",
	"<p>",
	"",
	"<ul>",
	"<ol>",
	"<li>",
	"<em>",
	"<strong>",
	"<hr color=#CCCCCC size=1 />",
	"<br />",
	"",
	"<blockquote>",
	"<h1 ",
	"<h2 ",
	"<h3 ",
	"<h4 ",
	"<h5 ",
	"<h6 ", // �ұߵļ�����Ԥ������������ı�ǩ����
	"<pre><code>",
	"<code>"
};
// HTML ���ñ�ǩ
const string backTag[] = {
	"",
	"</p>",
	"",
	"</ul>",
	"</ol>",
	"</li>",
	"</em>",
	"</strong>",
	"",
	"",
	"",
	"</blockquote>",
	"</h1>",
	"</h2>",
	"</h3>",
	"</h4>",
	"</h5>",
	"</h6>",
	"</code></pre>",
	"</code>"
};

// AST
struct node {
	friend class MarkdownTransform;
	int type;
	vector <node *> ch;
	string elem[3];			// elem[0]:���� elem[1]:���� elem[2]:title

	node(int _type)
		:type(_type)
	{
	}
};

// mark
struct Cnode {
	friend class MarkdownTransform;
	vector<Cnode *> ch;
	string heading;
	string tag;

	Cnode(const string &hd)
		:heading(hd)
	{
	}
};

class MarkdownTransform {
private:
	string content, TOC;
	int cntTag = 0;
	char s[maxLength];
	node *root, *now;
	Cnode *Croot;

	bool isHeading(node *v) {
		return (h1 <= v->type && v->type <= h6);
	}

	bool isHref(node *v) {
		return (v->type == href);
	}

	bool isImage(node *v) {
		return (v->type == image);
	}

	template <typename T>
	void destroy(T *v) {
		for (int i = 0; i < (int)v->ch.size(); i++) {
			destroy(v->ch[i]);
		}
		delete v;
	}

	void Cdfs(Cnode *v, string index) {
		TOC += "<li>\n";
		TOC += "<a href=\"#" + v->tag + "\">" + index + " " + v->heading + "</a>\n";
		int n = (int)v->ch.size();
		if (n) {
			TOC += "<ul>\n";
			for (int i = 0; i < n; i++) {
				Cdfs(v->ch[i], index + to_string(i + 1) + ".");
			}
			TOC += "</ul>\n";
		}
		TOC += "</li>\n";
	}

	void Cins(Cnode *v, int x, const string &hd, int tag) {
		int n = (int)v->ch.size();
		if (x == 1) {
			v->ch.push_back(new Cnode(hd));
			v->ch.back()->tag = "tag" + to_string(tag);
			return;
		}

		if (!n || v->ch.back()->heading.empty()) {
			v->ch.push_back(new Cnode(""));
		}
		Cins(v->ch.back(), x - 1, hd, tag);
	}

	// ȥ��ÿ�п�ʼ�Ŀո�� Tab 
	// ����ֵ ��һ�� int Ϊ�ո����������tab ��4���ո�ת����һ�� tab ����tab ������
	// ���ص� �ڶ���ֵ Ϊָ�����ݿ�ʼ��ָ��
	pair<int, char *> caluBeginSpace(char *src) {
		if ((int)strlen(src) == 0) {
			return make_pair(0, nullptr);
		}

		int cntSpace = 0;
		int cntTab = 0;

		for (int i = 0; src[i] != '\0'; i++) {
			if (src[i] == ' ') {
				cntSpace++;
			}
			else if (src[i] == '\t') {
				cntTab++;
			}
			else {
				return make_pair(cntTab + cntSpace / 4, src + i);
			}
		}
		return make_pair(0, nullptr);
	}

	// ���ɶ���
	void mkpara(node *v) {
		if (v->ch.size() == 1U && v->ch.back()->type == paragraph) {
			return;
		}
		if (v->type == paragraph) {
			return;
		}
		if (v->type == nul) {
			v->type = paragraph;
			return;
		}
		node *x = new node(paragraph);
		x->ch = v->ch;
		v->ch.clear();
		v->ch.push_back(x);
	}

	// �ж�������
	// ���� pair �У���һ�� int ֵΪ���ͣ��ڶ��� char * Ϊ�����͵��ı�
	pair<int, char *> judgeType(char *src) {
		if (src == nullptr) {
			return make_pair(nul, nullptr);
		}

		char *ptr = src;
		
		// ###
		while (*ptr == '#') {
			ptr++;
		}

		if (ptr - src > 0 && *ptr == ' ') {
			return make_pair(ptr - src + h1 - 1, ptr + 1);
		}

		ptr = src;
		
		// ```
		if (strncmp(ptr, "```", 3) == 0) {
			return make_pair(blockcode, ptr + 3);
		}

		// * or + or -, ����һ���ַ�Ϊ�ո���Ϊ�б�
		if (strncmp(ptr, "- ", 2) == 0 || strncmp(ptr, "* ", 2) == 0 || strncmp(ptr, "+ ", 2) == 0) {
			return make_pair(ul, ptr + 1);
		}

		// ������� > ����һ���ַ�Ϊ�ո���Ϊ����
		if (*ptr == '>' && (ptr[1] == ' ')) {
			return make_pair(quote, ptr + 1);
		}

		// ����������֣�����һ���ַ�Ϊ . ��Ϊ�����б�
		char *ptr1 = ptr;
		while (*ptr1 && (isdigit(*ptr1))) {
			ptr1++;
		}
		if (ptr1 != ptr && *ptr1 == '.' && ptr1[1] == ' ') {
			return make_pair(ol, ptr1 + 1);
		}

		// ���򣬾�����ͨ����
		return make_pair(paragraph, ptr);
	}

	// �����������Ѱ�ҽڵ�
	node *findnode(int depth) {
		node *ptr = root;
		while (!ptr->ch.empty() && depth != 0) {
			ptr = ptr->ch.back();
			if (ptr->type == li) {
				depth--;
			}
		}
		return ptr;
	}

	// ��ָ��������Ҫ����Ĵ�
	void insert(node *v, const string &src) {
		int n = (int)src.size();
		bool incode = false,
			inem = false,
			instrong = false,
			inautolink = false;
		v->ch.push_back(new node(nul));
		
		for (int i = 0; i < n; i++) {
			char ch = src[i];

			if (ch == '\\') {
				ch = src[++i];
				v->ch.back()->elem[0] += string(1, ch);
				continue;
			}
			
			// ���ڴ���
			if (ch == '`' && !inautolink) {
				incode ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(code));
				incode = !incode;
				continue;
			}

			// ǿ��
			// ** , *
			if (ch == '*' && (i < n - 1 && (src[i + 1] == '*')) && !incode && !inautolink) {
				i++;
				instrong ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(strong));
				instrong = !instrong;
				continue;
			}
			if (ch == '*' && !incode && !instrong && !inautolink) {
				inem ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(em));
				inem = !inem;
				continue;
			}
			// __ , _
			if (ch == '_' && (i < n - 1 && (src[i + 1] == '_')) && !incode && !inautolink) {
				i++;
				instrong ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(strong));
				instrong = !instrong;
				continue;
			}
			if (ch == '_' && !incode && !instrong && !inautolink) {
				inem ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(em));
				inem = !inem;
				continue;
			}

			// ͼƬ
			if (ch == '!' && (i < n - 1 && src[i + 1] == '[') && !incode && !instrong && !inem && !inautolink) {
				v->ch.push_back(new node(image));
				for (i += 2; i < n - 1 && src[i] != ']'; i++) {
					v->ch.back()->elem[0] += string(1, src[i]);
				}
				i++; // src[i] == '('
				for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++) {
					v->ch.back()->elem[1] += string(1, src[i]);
				}
				if (src[i] != ')') { // with title
					for (i++; i < n - 1 && src[i] != ')'; i++) {
						if (src[i] != '"') {
							v->ch.back()->elem[2] += string(1, src[i]);
						}
					}
				}
				v->ch.push_back(new node(nul));
				continue;
			}

			// ������
			if (ch == '[' && !incode && !instrong && !inem && !inautolink) {
				v->ch.push_back(new node(href));
				for (i++; i < n - 1 && src[i] != ']'; i++) {
					v->ch.back()->elem[0] += string(1, src[i]);
				}
				i++; // src[i] == '('
				for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++) {
					v->ch.back()->elem[1] += string(1, src[i]);
				}
				if (src[i] != ')') { // �� title
					for (i++; i < n - 1 && src[i] != ')'; i++) {
						if (src[i] != '"') {
							v->ch.back()->elem[2] += string(1, src[i]);
						}
					}
				}
				v->ch.push_back(new node(nul));
				continue;
			}

			// �������ڴ����е� '<' '>'
			if (incode) {
				if (ch == '<') {
					v->ch.back()->elem[0] += "&lt;";
				}
				else if (ch == '>') {
					v->ch.back()->elem[0] += "&gt;";
				}
				else {
					v->ch.back()->elem[0] += ch;
				}
				continue;
			}
			v->ch.back()->elem[0] += string(1, ch);
		}

		if (src.size() >= 2) {
			if (src.at(src.size() - 1) == ' ' && src.at(src.size() - 2) == ' ')
				v->ch.push_back(new node(br));
		}
	}

	void dfs(node *v) {
		if (v->type == paragraph && v->elem[0].empty() && v->ch.empty()) {
			return;
		}

		content += frontTag[v->type];
		bool flag = true;

		if (isHeading(v)) {
			content += "id=\"" + v->elem[0] + "\">";
			flag = false;
		}

		if (isHref(v)) {
			content += "<a href=\"" + v->elem[1] + "\" title=\"" + v->elem[2] + "\">" + v->elem[0] + "</a>";
			flag = false;
		}

		if (isImage(v)) {
			content += "<img alt=\"" + v->elem[0] + "\" src=\"" + v->elem[1] + "\" title=\"" + v->elem[2] + "\" />";
			flag = false;
		}

		// ���������������ֱ���������
		if (flag) {
			content += v->elem[0];
			flag = false;
		}

		for (int i = 0; i < (int)v->ch.size(); i++) {
			dfs(v->ch[i]);
		}

		content += backTag[v->type];
	}

	bool isCutline(char *src) {
		int cnt = 0;
		char *ptr = src;
		if (ptr == nullptr) {
			return false;
		}

		while (*ptr) {
			if (*ptr != ' ' && *ptr != '\t' && *ptr != '-') {
				return false;
			}
			if (*ptr == '-') {
				cnt++;
			}
			ptr++;
		}

		return (cnt >= 3);
	}
public:
	MarkdownTransform(const string &filename) {
		Croot = new Cnode("");
		root = new node(nul);
		now = root;
		
		ifstream fin(filename);
		if (!fin) {
			cerr << "no such file" << endl;
			exit(0);
		}

		bool newpara = false;
		bool inblock = false;
		while (!fin.eof()) {
			fin.getline(s, maxLength);
			
			if (!inblock && isCutline(s)) {
				now = root;
				now->ch.push_back(new node(br));
				newpara = false;
				continue;
			}

			pair<int, char *> ps = caluBeginSpace(s);
			// ���ڴ�����У���û��ͳ�Ƶ��ո� �� Tab��ֱ�Ӷ�ȡ��һ��
			if (!inblock && ps.second == nullptr) {
				now = root;
				newpara = true;
				continue;
			}
			pair<int, char *> jt = judgeType(ps.second);

			// ���������
			if (jt.first == blockcode) {
				// ��λ�ڴ�����У���push һ�������ͽڵ�
				inblock ? now->ch.push_back(new node(nul)) : now->ch.push_back(new node(blockcode));
				inblock = !inblock;
				continue;
			}
			// �ڴ�����е�����ֱ��ƴ�ӵ���ǰ�ڵ�
			if (inblock) { // ??? ������ͷ�ļ�Ϊ����ʾ��ȫ
				string code;
				for (int i = 0; i < (int)strlen(s); i++) {
					if (s[i] == '<') {
						code += "&lt;";
					}
					else if (s[i] == '>') {
						code += "&gt;";
					}
					else {
						code += s[i];
					}
				}
				now->ch.back()->elem[0] += code + '\n';
				continue;
			}

			// ��ͨ����
			if (jt.first == paragraph) {
				if (now == root) {
					now = findnode(ps.first);
					now->ch.push_back(new node(paragraph));
					now = now->ch.back();
				}
				bool flag = false;
				if (newpara && !now->ch.empty()) {
					node *ptr = nullptr;
					for (auto i : now->ch) {
						if (i->type == nul) {
							ptr = i;
						}
					}
					if (ptr != nullptr) {
						mkpara(ptr);
					}
					flag = true;
				}
				if (flag) {
					now->ch.push_back(new node(paragraph));
					now = now->ch.back();
				}
				now->ch.push_back(new node(nul));
				insert(now->ch.back(), string(jt.second));
				newpara = false;
				continue;
			}


			now = findnode(ps.first);

			// ����������
			if (h1 <= jt.first && jt.first <= h6) {
				now->ch.push_back(new node(jt.first));
				now->ch.back()->elem[0] = "tag" + to_string(++cntTag);
				insert(now->ch.back(), string(jt.second));
				Cins(Croot, jt.first - h1 + 1, string(jt.second), cntTag);
			}

			// �����б�
			if (jt.first == ul) {
				if (now->ch.empty() || now->ch.back()->type != ul) {
					now->ch.push_back(new node(ul));
				}
				now = now->ch.back();
				bool flag = false;
				if (newpara && !now->ch.empty()) {
					node *ptr = nullptr;
					for (auto i : now->ch) {
						if (i->type == li) {
							ptr = i;
						}
					}
					if (ptr != nullptr) {
						mkpara(ptr);
					}
					flag = true;
				}
				now->ch.push_back(new node(li));
				now = now->ch.back();
				if (flag) {
					now->ch.push_back(new node(paragraph));
					now = now->ch.back();
				}
				insert(now, string(jt.second));
			}

			// �����б�
			if (jt.first == ol) {
				if (now->ch.empty() || now->ch.back()->type != ol) {
					now->ch.push_back(new node(ol));
				}
				now = now->ch.back();
				bool flag = false;
				if (newpara && !now->ch.empty()) {
					node *ptr = nullptr;
					for (auto i : now->ch) {
						if (i->type == li) {
							ptr = i;
						}
					}
					if (ptr != nullptr) {
						mkpara(ptr);
					}
					flag = true;
				}
				now->ch.push_back(new node(li));
				now = now->ch.back();
				if (flag) {
					now->ch.push_back(new node(paragraph));
					now = now->ch.back();
				}
				insert(now, string(jt.second));
			}

			// ���������
			if (jt.first == quote) {
				if (now->ch.empty() || now->ch.back()->type != quote) {
					now->ch.push_back(new node(quote));
				}
				now = now->ch.back();
				if (newpara || now->ch.empty()) {
					now->ch.push_back(new node(paragraph));
				}
				insert(now->ch.back(), string(jt.second));
			}

			newpara = false;
		}

		fin.close();

		dfs(root);

		TOC += "<ul>";
		for (int i = 0; i < (int)Croot->ch.size(); i++) {
			Cdfs(Croot->ch[i], to_string(i + 1) + ".");
		}
		TOC += "</ul>";
	}

	string getTableOfContents() {
		return TOC;
	}
	string getContents() {
		return content;
	}

	~MarkdownTransform()
	{
		destroy<node>(root);
		destroy<Cnode>(Croot);
	}
};



