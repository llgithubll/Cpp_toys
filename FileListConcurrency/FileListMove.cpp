#include <thread>
#include <future>
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <cassert>
#include <chrono>

#include "timer.h" // 计时工具


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
	vector<path> dirs; // 做为缓存栈, 存储将要递归遍历的目录
	vector<string> files;  // 存储所有文件名
	vector<future<files_dirs_type>> futures;

	dirs.push_back(path(root));

	while (!dirs.empty()) {
		for (int i = 0; i < thread_num && !dirs.empty(); i++) {
			futures.emplace_back(async(listDir, dirs.back()));
			dirs.pop_back();
		}
		// 没有data share, 不存在data race, 不需要synchronization
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
	第一次迭代, 会对文件名进行cache, 所以后续的迭代花费时间通常比第一次短
	本程序中, VS2017, x64-Debug和x64-release速度并没有太大差异
	而且成倍的增加线程数量并不会让运行时间成倍的减少
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