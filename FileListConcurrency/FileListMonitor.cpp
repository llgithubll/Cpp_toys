/*
任务描述: 遍历磁盘上的所有文件, 并将文件名存储在vector<string> files中

Monitor模式, 每一个线程会有一个对files的引用, 使用互斥量, 保证正常的对共享数据进行读写

*/

#include <thread>
#include <future>
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <cassert>
#include <chrono>
#include <mutex>

#include "timer.h" // 计时工具

using namespace std;
using namespace std::tr2::sys;


struct Result {
	vector<string> files;
	vector<path> dirs;
};


// monitor模式
// pattern of monitor
class MonitorResult {
	Result _result;
	mutex _mutex;
public:
	void putFile(string && filename) {
		lock_guard<mutex> lck(_mutex);
		_result.files.push_back(filename);
	}

	void putDir(path && pth) {
		lock_guard<mutex> lck(_mutex);
		_result.dirs.push_back(pth);
	}

	vector<path> getDirs(int n) {
		lock_guard<mutex> lck(_mutex);
		vector<path> dirs;
		for (int i = 0; i < n && !_result.dirs.empty(); i++) {
			dirs.push_back(std::move(_result.dirs.back()));
			_result.dirs.pop_back();
		}
		return dirs;
	}

	bool isDirsEmpty() {
		lock_guard<mutex> lck(_mutex);
		return _result.dirs.empty();
	}

	auto dirsSize() {
		lock_guard<mutex> lck(_mutex);
		return _result.dirs.size();
	}

	auto filesSize() {
		lock_guard<mutex> lck(_mutex);
		return _result.files.size();
	}
};

void listDir(path && dir, MonitorResult & result)
{
	for (directory_iterator it(dir); it != directory_iterator(); ++it)
	{
		if (is_regular_file(it->status()))
		{
			result.putFile(it->path().filename().string());
		}
		else if (is_directory(it->status())) {

			result.putDir(path(it->path()));
		}
	}
}


void listDirRec(string root, int thread_num = 8)
{
	MonitorResult result;
	vector<future<void>> futures;

	result.putDir(path(root));

	while (!result.isDirsEmpty()) {
		vector<path> dirs = result.getDirs(thread_num);
		
		while (!dirs.empty()) {
			futures.emplace_back(async(listDir, std::move(dirs.back()), std::ref(result)));
			dirs.pop_back();
		}

		for (auto & fut : futures) {
			fut.wait();  // 对于没有返回值的函数调用,等待执行结束
		}

		futures.clear();
	}
	cout << "total files: " << result.filesSize() << endl;
	assert(result.dirsSize() == 0);
}

int main()
{
	cout << "hello, FileListMonitor" << endl;

	timer t;
	double total_seconds = 0.0;
	int total_iter = 10;

	/*
	第一次迭代, 会对文件名进行cache, 所以后续的迭代花费时间通常比第一次短
	本程序中, vs2017, x64-debug和x64-release速度并没有太大差异
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
