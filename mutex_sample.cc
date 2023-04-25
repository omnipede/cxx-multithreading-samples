#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

void worker(int& counter, std::mutex& m) {
  for (int i = 0; i < 10000; i++) {
    // m.lock() 을 해도 되지만 smart pointer 처럼 m.unlock() 을 안해도 되도록 하는 기능.
    std::lock_guard<std::mutex> lock(m);
    counter += 1;

    // 스코프를 빠져 나가면 자동으로 unlock 된다.
  }
}

int main() {
  int counter = 0;
  std::vector<std::thread> workers;
  std::mutex m;

  for (int i = 0; i < 4; i++) {
    workers.push_back(std::thread(worker, std::ref(counter), std::ref(m)));
  }

  for (int i = 0; i < workers.size(); i++) {
    workers[i].join();
  }

  std::cout << "Counter 최종 값: " << counter << std::endl;
}
