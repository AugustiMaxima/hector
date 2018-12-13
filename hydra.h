#ifndef HYDRA
#define HYDRA

#include <thread>
#include <functional>

namespace hydra{
//thread scheduling has an overhead. If we are reasonably certain about the max number of threads that can execute concurrently, segmenting based on thread count will take advantage of core affinity and deliver the fastest result
//you can also use this to force multi-threading, i've kept some of the older code for reference, though

    template<class C, class S>
    void transform(int length, C* base, S* product, std::function<S(C&)> transformation){
        for (int i=0;i<length;i++)
            product[i] = transformation(base[i]);
    }

    template<class C, class S>
    void parallel_transform(int length, C* container, S* product, std::function<S(C&)> transformation, int step){
        int th_count = length%step ? length/step + 1 : length/step;
        std::thread* threads = new std::thread[th_count];
        for (int i=0;i<th_count;i++){
            int th_length = (i+1)*step >length? length - i*step : step;
            threads[i] = std::thread(transform<C,S>, th_length, container+i, product+i, transformation);
        }
        for (int i=0;i<th_count;i++){
            threads[i].join();
        }
    }

    template<class C>
    void validate(int length, C* base, bool* valid_bit, std::function<bool(C&)>validation){
        for (int i=0; i<length; i++)
            valid_bit[i] = validation(base[i]);
    }

    template<class C>
    bool* parallel_validate(int length, C* container, std::function<const bool(C&)> validation, int step){
        int th_count = length%step ? length/step + 1 : length/step;
        std::thread* threads = new std::thread[th_count];
        bool* validate_array = new bool[th_count];
        for (int i=0;i<th_count;i++){
            int th_length = (i+1)*step >length? length - i*step : step;
            threads[i] = std::thread(validate<C>, th_length, container+i, validate_array+i, validation);
        }
        for (int i=0;i<th_count;i++){
            threads[i].join();
        }
        return validate_array;
    }

}

#endif