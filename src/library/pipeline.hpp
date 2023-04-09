#ifndef pipeline
#define pipeline

#include <vector>
#include <tuple>
#include <any>
#include <functional>
#include "common/queue.hpp"
#include "common/worker.hpp"
#include <variant>


using namespace std;

template<typename Func>
struct function_traits;

template<typename R, typename Arg>
struct function_traits<R(*)(Arg)> {
    using return_type = R;
    using argument_tuple = Arg;
};

template<typename I, typename O>
class Pipeline{
    private:

        template<typename Func>
        void createWorker(int index, Func func) {
            using argument_type = typename function_traits<Func>::argument_tuple;
            using return_type = typename function_traits<Func>::return_type;
            Queue<argument_type>& inputStream = static_cast<Queue<argument_type>&>(streams[index]);
            Queue<return_type>& outputStream = static_cast<Queue<return_type>&>(streams[index+1]);
            

            Worker<argument_type, return_type>* w = new Worker<argument_type, return_type>(
                func,
                inputStream,
                outputStream
            );

            workers.push_back(w);
        }

        template<typename Func, typename... MoreF>
        void createWorker(int index, Func func, MoreF... funcs) {
            createWorker(index, func);
            createWorker(index + 1, funcs...);
        }

        


    public:
        std::vector<QueueBase> streams;
        std::vector<WorkerBase*> workers;

        template<typename F, typename... MoreF>
        Pipeline(F f, MoreF... funcs){
            int funcCount = static_cast<int>(sizeof...(MoreF));
            streams = std::vector<QueueBase>(funcCount);
            createWorker(0, f, funcs...);
        };
};
#endif