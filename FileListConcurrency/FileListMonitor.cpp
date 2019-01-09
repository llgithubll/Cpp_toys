/*
��������: ���������ϵ������ļ�, �����ļ����洢��vector<string> files��

Monitorģʽ, ÿһ���̻߳���һ����files������, ʹ�û�����, ��֤�����ĶԹ������ݽ��ж�д

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

#include "timer.h" // ��ʱ����

using namespace std;
using namespace std::tr2::sys;


struct Result {
	vector<string> files;
	vector<path> dirs;
};


// monitorģʽ
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
			fut.wait();  // ����û�з���ֵ�ĺ�������,�ȴ�ִ�н���
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
	��һ�ε���, ����ļ�������cache, ���Ժ����ĵ�������ʱ��ͨ���ȵ�һ�ζ�
	��������, vs2017, x64-debug��x64-release�ٶȲ�û��̫�����
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
