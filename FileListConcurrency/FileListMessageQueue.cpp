/*
任务描述:  遍历磁盘上的所有文件, 并将文件名输出

使用消息队列模式(生产者与消费者问题)

本背景环境下有两组消费者生产者
1. 从dirQueue中取得文件夹(文件夹消费者), 进行遍历; 对遍历得到的文件夹加入dirQueue(文件夹生产者), 遍历得到的文件加入fileQueue(文件名生产者)
2. 从fileQueue中取得文件名(文件名消费者)输出

NOTE: 无限循环, 还没有实现程序终止
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
#include <deque>
#include <condition_variable>

#include "timer.h" // 计时工具

using namespace std;
using namespace std::tr2::sys;


template <typename T>
class MsgQueue {
	deque<T> _queue;
	condition_variable _cond;
	mutex _mutex;
public:
	void send(T && msg) {
		{
			lock_guard<mutex> _lck(_mutex);
			_queue.push_front(std::move(msg));
		}
		_cond.notify_one();
	}

	T receive() {
		unique_lock<mutex> lck(_mutex);
		_cond.wait(lck, [this] {return !_queue.empty(); });
		T msg = std::move(_queue.back());
		_queue.pop_back();
		return msg;
	}
};


void listDirServer(MsgQueue<path> & dirQueue, MsgQueue<string> & fileQueue) {
	while (true) {
		path dir = dirQueue.receive();

		for (directory_iterator it(dir); it != directory_iterator(); ++it)
		{
			if (is_regular_file(it->status()))
			{
				fileQueue.send(it->path().filename().string());
			}
			else if (is_directory(it->status())) {
				dirQueue.send(path(it->path()));
			}
		}
	}
}


void printServer(MsgQueue<string> & nameQueue) {
	while (true) {
		string name = nameQueue.receive();
		cout << name << endl;
	}
}

void listAllFiles(string root, int thread_num = 8) {
	MsgQueue<path> dirQueue;
	MsgQueue<string> fileQueue;
	dirQueue.send(path(root));

	vector<future<void>> futures;
	for (int i = 0; i < thread_num; i++) {
		futures.emplace_back(async(listDirServer, std::ref(dirQueue), std::ref(fileQueue)));
	}
	futures.emplace_back(async(printServer, std::ref(fileQueue)));

	for (auto & fut : futures) {
		fut.wait();  // 对于没有返回值的函数调用,等待执行结束
	}

}


int main()
{
	cout << "hello, FileListMessageQueue" << endl;
	listAllFiles("F:\\Desktop\\日语");
	return 0;
}