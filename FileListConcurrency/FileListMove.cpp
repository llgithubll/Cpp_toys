#include <thread>
#include <future>
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <cassert>
#include <chrono>

#include "timer.h" // ��ʱ����


using namespace std;
using namespace std::tr2::sys;

typedef pair<vector<string>, vector<path>> files_dirs_type;

auto listDir(path && dir)
{
	vector<string> files;
	vector<path> dirs;
	for (directory_iterator it(dir); it != directory_iterator(); ++it)
	{
		if (is_regular_file(it->status()))
		{
			files.push_back(it->path().filename().string());
		}
		else if (is_directory(it->status())) {
			dirs.push_back(it->path());
		}
	}
	return std::move(make_pair(files, dirs));
}


void listDirRec(string root, int thread_num = 8)
{
	vector<path> dirs; // ��Ϊ����ջ, �洢��Ҫ�ݹ������Ŀ¼
	vector<string> files;  // �洢�����ļ���
	vector<future<files_dirs_type>> futures;

	dirs.push_back(path(root));

	while (!dirs.empty()) {
		for (int i = 0; i < thread_num && !dirs.empty(); i++) {
			futures.emplace_back(async(listDir, dirs.back()));
			dirs.pop_back();
		}
		// û��data share, ������data race, ����Ҫsynchronization
		for (auto & fut : futures) {
			auto fd = fut.get();
			std::move(fd.first.begin(), fd.first.end(), back_inserter(files));
			std::move(fd.second.begin(), fd.second.end(), back_inserter(dirs));
		}

		futures.clear();
	}

	//for_each(files.begin(), files.end(), [](string &s) {cout << "file:" << s << '\n'; });
	cout << "total files: " << files.size() << endl;
	assert(dirs.size() == 0);
}

int main()
{
	cout << "hello, FileListMove" << endl;

	timer t;
	double total_seconds = 0.0;
	int total_iter = 10;

	/*
	��һ�ε���, ����ļ�������cache, ���Ժ����ĵ�������ʱ��ͨ���ȵ�һ�ζ�
	��������, VS2017, x64-Debug��x64-release�ٶȲ�û��̫�����
	���ҳɱ��������߳�����������������ʱ��ɱ��ļ���
	*/
	for (int i = 0; i < total_iter; i++) {
		t.restart();
		listDirRec("D:\\");
		double seconds = t.elapsed();
		total_seconds += seconds;
		cout << i << ": " << seconds << " seconds." << endl;
	}
	cout << "total seconds: " << total_seconds << ", average seconds: " << total_seconds / total_iter << endl;

	return 0;
}