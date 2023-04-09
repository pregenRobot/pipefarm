#include <stdio.h>
#include <iostream>
#include <random>
#include "library/farm.hpp"
#include <thread>
#include <vector>

// for convolution

using namespace std;



long fib(int n)
{
  long  i, Fnew, Fold, temp,ans;

    Fnew = 1;  Fold = 0;
    for ( i = 2;
	  i <= n;          /* apsim_loop 1 0 */
	  i++ )
    {
      temp = Fnew;
      Fnew = Fnew + Fold;
      Fold = temp;
    }
    ans = Fnew;
  return ans;
}

int main(int argc, char** argv){
    std::cout << "Initialized" << std::endl;


    Farm<int,long> parallelWait = Farm(&fib,10);

    std::vector<int> seeds = std::vector<int>();

    int test_size = 80;

    for(size_t i = 0; i < test_size; i ++){
        seeds.push_back(i);
    }

    parallelWait.feed(seeds);
    parallelWait.start();
    std::cout << "[";

    int retCounter = 0;
    while(retCounter < test_size){
      long output;
      bool popResult = parallelWait.outputStream.pop(output,false);

      if(popResult){
        std::cout << "<output: " << output << " count: " << retCounter+1 << ">, " << std::flush;
        retCounter +=1;
      }else{
        continue;
      }

    }

    parallelWait.stopAll();
    
    std::cout << "]" << std::endl;

    return 0;
}