#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

void producer(std::queue<std::string>* downloaded_pages, std::mutex* m, int index, std::condition_variable* cv) {
    for (int i = 0; i < 5; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));
        std::string content = "Website: " + std::to_string(i) + " from thread(" + std::to_string(index) + ")";
        m->lock();
        downloaded_pages->push(content);
        m->unlock();

        // wait 중인 쓰레드를 하나 깨운다.
        cv->notify_one();
    }
}

void consumer(std::queue<std::string>* downloaded_pages, std::mutex* m, int* num_processed, std::condition_variable* cv) {
    while(*num_processed < 25) {
        std::unique_lock<std::mutex> lock(*m);

        // Condition variable 은 wait 함수의 조건이 참이 될 때 까지 기다린다.
        // 만약 조건이 만족되지 않는다면 lock 을 unlock 한 상태로 영원히 sleep 한다.
        cv->wait(lock, [&]{ return !downloaded_pages->empty() || *num_processed == 25; });

        if (*num_processed == 25) {
            lock.unlock();
            return;
        }

        std::string content = downloaded_pages->front();
        downloaded_pages->pop();

        (*num_processed)++;
        lock.unlock();

        std::cout << content << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
}

int main() {

    std::queue<std::string> downloaded_pages;
    std::mutex m;
    std::condition_variable cv;

    std::vector<std::thread> producers;
    for (int i = 0; i < 5; i++) {
        producers.push_back(
            std::thread(producer, &downloaded_pages, &m, i + 1, &cv)
        );
    }

    int num_processed = 0;
    std::vector<std::thread> consumers;
    for (int i = 0; i < 3; i++) {
        consumers.push_back(
            std::thread(consumer, &downloaded_pages, &m, &num_processed, &cv)
        );
    }

    for (int i = 0; i < producers.size(); i++) {
        producers[i].join();
    }

    // 아직 wait 하고 있는 쓰레드가 있을 수 있으므로 깨워준다.
    cv.notify_all();

    for (int i = 0; i < consumers.size(); i++) {
        consumers[i].join();
    }
}