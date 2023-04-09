#ifndef safe_queue
#define safe_queue

#include <mutex>
#include <atomic>
#include <memory>
#include <queue>
#include <condition_variable>
#include <cstdlib>

using namespace std;
class QueueBase {
public:
    virtual ~QueueBase() = default;
};


template <typename T>
class Queue: public QueueBase{
    private:
        std::queue<T> _queue;
        std::mutex _mutex;
        std::condition_variable _emptyQueue;
        
    public:
        Queue() = default;

        void push(const T& value){
            std::unique_lock<std::mutex> lock(_mutex);
            _queue.push(value);
            _emptyQueue.notify_one();
        }

        bool empty() {
            std::lock_guard<std::mutex> lock(_mutex);
            bool isEmpty = _queue.empty();
            return isEmpty;
        }

        bool pop(T& retVal,bool wait = false){
            std::unique_lock<std::mutex> lock(_mutex);

            if(!wait && _queue.empty()){
                return false;
            }

            if(wait){
                _emptyQueue.wait(lock, [this] {return !_queue.empty();});
            }
            retVal = std::move(_queue.front());
            _queue.pop();
            return true;
        }


        int size() {
            std::lock_guard<std::mutex> lock(_mutex);
            int _size = static_cast<int>(_queue.size());
            return _size;
        }

        std::vector<T> drain(){
            std::vector<T> result = std::vector<T>();
            std::lock_guard<std::mutex> lock(_mutex);
            while(!_queue.empty()){
                result.push_back(_queue.front());
                _queue.pop();
            }
            return result;
        }

        // std::vector<T> drain(int expected){
        //     std::vector<T> result = std::vector<T>();
        //     std::lock_guard<std::mutex> lock(_mutex);
        //     int collectCount = 0;
        //     while(collectCount < expected){
        //         result.push_back(_queue.front());
        //         std::cout << "<output: " << _queue.front() << " count: " << retCounter << ">, ";
        //         _queue.pop();
        //     }
        //     return result;
        // }
};

#endif