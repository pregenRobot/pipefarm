#ifndef farm
#define farm

#include "common/queue.hpp"
#include "common/worker.hpp"


#include <vector>
#include <atomic>
#include <condition_variable>
#include <thread>


using namespace std;

template <class I, class O>
class Farm{

    private:

        int _workerCount;
        O (*_func)(I);
        std::vector<Worker<I,O>*> _workers;
        int _workerIndex = 0;

    public:

        Queue<O> outputStream;
        std::vector<Queue<I>> inputStreams;

        Farm(O(*func)(I), int workCount): 
        _workerCount(workCount),
        _func(func), 
        outputStream(Queue<O>()),
        inputStreams(std::vector<Queue<I>>(workCount)),
        _workers(std::vector<Worker<I,O>*>())
        {
            for(int i = 0; i < _workerCount; i++){
                Queue<I>& is = inputStreams[i];
                Worker<I,O>* w = new Worker<I,O>(
                    _func, 
                    is,
                    outputStream
                );
                _workers.push_back(w);
            }
        };

        void stopAll(){
            for(Worker<I,O>* w: _workers){
                w->run.store(false);
            }

            for(Worker<I,O>* w:_workers){
                w->_thread.join();
            }
        }

        void start(){
            for(Worker<I,O>* w: _workers){
                w->invoke();
            }
        }

        void feed(I& input){
            // _workers[_workerIndex]->inputQueue.push(input);
            inputStreams[_workerIndex].push(input);
            _workerIndex = (_workerIndex + 1) % _workerCount;
        }

        void feed(std::vector<I>& inputs){
            for(I& input: inputs){
                this->feed(input);
            }
        }
};

#endif