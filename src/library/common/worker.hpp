
#ifndef worker
#define worker

#include "queue.hpp"
#include <tuple>
#include <thread>
#include <atomic>
#include <cstdio>
#include <memory>
#include <vector>

using namespace std;

class WorkerBase {
public:
    virtual ~WorkerBase() = default;
};

template <class I, class O>
class Worker: public WorkerBase{
    
    private:
        void work(){
            while (this->run.load()){
                I input;
                bool popResult = inputQueue.pop(input, false);
                if(popResult){
                    O output = this->_func(input);
                    outputQueue.push(output);
                }
            }
            int done = 1;
        }

        O(*_func)(I);

    public:
        std::thread _thread;
        Queue<I>& inputQueue;
        Queue<O>& outputQueue;
        std::atomic<bool> run{true};

        // Worker(O(*func)(I)): 
        // _func(func),
        // inputQueue(Queue<I>()),
        // outputQueue(Queue<O>()){};

        // Worker(O(*func)(I), Queue<O>& outputQ): 
        // _func(func), 
        // inputQueue(Queue<I>()), 
        // outputQueue(outputQ){};

        // Worker(O(*func)(I), Queue<I>& inputQ): 
        // _func(func), 
        // inputQueue(inputQ), 
        // outputQueue(Queue<O>()){};

        Worker(O(*func)(I), Queue<I>& inputQ, Queue<O>& outputQ): 
        _func(func), 
        inputQueue(inputQ), 
        outputQueue(outputQ){};

        void invoke(){
            _thread = std::thread(&Worker<I,O>::work, this);
        };

};

#endif
