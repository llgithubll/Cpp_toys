# Cpp_toys
These are some simple project write in c++


## BackTrack
A template for backtracking algorithm to solving combinatorial problem.

reference:[Solving Combinatorial Problems with STL and Backtracking](http://www.drdobbs.com/cpp/solving-combinatorial-problems-with-stl/184401194)

## BigInteger
A BigInteger class implement in C++.

## BigIntegerSingle
Just combine BigInteger.h and BigInteger.cpp.

## BipartiteHungarian
A implementation of hungarian algorithm for bipartite max
match problem.


## FourOperationsExercise
Just calculate +,-,*,/ expression, implement by AST.

reference:
[vczh article](http://www.cppblog.com/vczh/archive/2008/06/15/53373.html)

## MarkdownParser2
Parser markdown to HTML by SAX.

reference:[shiyanlou](https://www.shiyanlou.com/courses/569)

## INI Parser
Very easy to use when get information from \*.ini file.

reference:[inifile](https://github.com/Winnerhust/inifile2)

## NeuralNetwork
Simple neural network learning xor from scratch

reference:[SimpleNeuralNetwork](https://github.com/huangzehao/SimpleNeuralNetwork)

## FileListConcurrency

任务描述:  遍历磁盘上的所有文件, 并将文件名存储在vector<string> files中(或输出)

* FileListMove, 使用move语义, 对每一个线程产生的结果, 依次放入files中, 不存在data share和data race
* FileListMonitor, Monitor模式, 每一个线程会有一个对files的引用, 使用互斥量, 保证正常的对共享数据进行读写
* 使用消息队列模式(生产者与消费者问题)
    本背景环境下有两组消费者生产者
    1. 从dirQueue中取得文件夹(文件夹消费者), 进行遍历; 对遍历得到的文件夹加入dirQueue(文件夹生产者), 遍历得到的文件加入fileQueue(文件名生产者)
    2. 从fileQueue中取得文件名(文件名消费者)输出

    NOTE: 无限循环, 还没有实现程序终止

reference:
* [2012年,虽然旧,但质量很高的教程](https://www.youtube.com/playlist?list=PL1835A90FC78FF8BE)
* [相应的较新版本的代码](https://github.com/BartoszMilewski/Concurrency)
