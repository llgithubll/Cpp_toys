/*
��������:  ���������ϵ������ļ�, �����ļ������

ʹ����Ϣ����ģʽ(������������������)

������������������������������
1. ��dirQueue��ȡ���ļ���(�ļ���������), ���б���; �Ա����õ����ļ��м���dirQueue(�ļ���������), �����õ����ļ�����fileQueue(�ļ���������)
2. ��fileQueue��ȡ���ļ���(�ļ���������)���

NOTE: ����ѭ��, ��û��ʵ�ֳ�����ֹ
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

#include "timer.h" // ��ʱ����

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
		fut.wait();  // ����û�з���ֵ�ĺ�������,�ȴ�ִ�н���
	}

}


int main()
{
	cout << "hello, FileListMessageQueue" << endl;
	listAllFiles("F:\\Desktop\\����");
	return 0;
}