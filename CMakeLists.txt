cmake_minimum_required(VERSION 3.16)
project(csandbox)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(mutex_sample mutex_sample.cc)
add_executable(consumer_procuder_sample consumer_producer_sample.cc)
add_executable(cond_var_sample cond_var_sample.cc)
