/*
FileList���ܲ���һ���õ�����ʹ���̳߳ص�����
ʹ���̳߳ص�ͨ����task-based ����
�̶���task����, �̶���thread����,
��task���ζ����̳߳���, �����̻߳��Զ���������
���߳���������е�����ʱ, ��ȡ���н��

��FileList��, task��������Ҫ����ִ�е����(listDir�е��ļ�����Ŀ)������
ʹ���̳߳صĳ������߼����˻���FileListMove.cpp�е�ģʽ
�ڴ����Ͽ��ܱ��������
*/


#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "timer.h" // ��ʱ����
#include "ThreadPool.h" // �̳߳�

using namespace std;
using namespace std::tr2::sys;

int main()
{
	cout << "hello, thread pool" << endl;
	return 0;
}

