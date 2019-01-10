cmake_minimum_required (VERSION 3.2)

project (FileListConcurrency)

add_executable(FileListMove FileListMove.cpp)

add_executable(FileListMonitor FileListMonitor.cpp)

add_executable(FileListMessageQueue FileListMessageQueue.cpp)

add_executable(FileListThreadPool FileListThreadPool.cpp)