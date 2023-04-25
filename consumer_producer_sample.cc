#include <chrono>
#include <queue>
#include <string>
#include <mutex>
#include <vector>
#include <mutex>
#include <thread>
#include <iostream>

void producer(std::queue<std::string>* downloaded_pages, std::mutex* m, int index) {
  for (int i = 0; i < 5; i++) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));
    std::string content = "웹사이트 : " + std::to_string(i) + " from thread(" + std::to_string(index) + ")";
    m->lock();
    downloaded_pages->push(content);
    m->unlock();
  }
}

void consumer(std::queue<std::string>* downloaded_pages, std::mutex* m, int* num_processed) {
  while(*num_processed < 25) {
    m->lock();
  
    // 큐에 아이템이 들어올 때 까지 대기한다.
    // 하지만, 이러한 polling 은 CPU 입장에서 비효율적이다. -> condition_variable 을 통해 해결.
    if (downloaded_pages->empty()) {
      m->unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      continue;
    }
  
    std::string content = downloaded_pages->front();
    downloaded_pages->pop();
  
    (*num_processed) += 1;
    m->unlock();
  
    std::cout << content << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(80));
  }
}

int main() {
  std::queue<std::string> downloaded_pages;
  std::mutex m;

  // Producer 는 큐에 아이템을 넣는 역할만 수행
  std::vector<std::thread> producers;
  for (int i = 0; i < 5; i++) {
    producers.push_back(std::thread(producer, &downloaded_pages, &m, i + 1));
  }

  // Consumer 는 큐에 있는 아이템을 처리하는 역할만 수행
  int num_processed = 0;
  std::vector<std::thread> consumers;
  for (int i = 0; i < 3; i++) {
    consumers.push_back(std::thread(consumer, &downloaded_pages, &m, &num_processed));
  }

  for (int i = 0; i < producers.size(); i++) {
    producers[i].join();
  }

  for (int i = 0; i < consumers.size(); i++) {
    consumers[i].join();
  }
}
