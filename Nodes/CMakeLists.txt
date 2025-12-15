cmake_minimum_required(VERSION 4.0)
project(Nodes)

set(CMAKE_CXX_STANDARD 20)

add_executable(Nodes main.cpp
        FileSystemNode.h
        FileSystem.h
        Trie.h
)
