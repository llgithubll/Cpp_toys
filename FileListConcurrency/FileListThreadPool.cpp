/*
FileList可能不是一个好的用来使用线程池的例子
使用线程池的通常是task-based 任务
固定的task数量, 固定的thread数量,
把task依次丢到线程池中, 各个线程会自动接手任务
当线程完成了所有的任务时, 获取所有结果

在FileList中, task的数量需要根据执行的情况(listDir中的文件夹数目)来决定
使用线程池的程序会从逻辑上退化成FileListMove.cpp中的模式
在代码上可能变得冗余了
*/


#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "timer.h" // 计时工具
#include "ThreadPool.h" // 线程池

using namespace std;
using namespace std::tr2::sys;

int main()
{
	cout << "hello, thread pool" << endl;
	return 0;
}

