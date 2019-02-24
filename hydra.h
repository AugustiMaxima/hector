#ifndef HYDRA
#define HYDRA

#include <thread>
#include <functional>

namespace hydra{
//thread scheduling has an overhead. If we are reasonably certain about the max number of threads that can execute concurrently, segmenting based on thread count will take advantage of core affinity and deliver the fastest result
//you can also use this to force multithreading off

    template<class C, class S>
    void transform(int length, C* base, S* product, std::function<S(C&)> transformation){
        for (int i=0;i<length;i++)
            product[i] = transformation(base[i]);
    }

    template<class C, class S>
    void parallel_transform(int length, C* container, S* product, std::function<S(C&)> transformation, int th_count){
	if(th_count>length || th_count <= 0)
	    th_count = length;
        std::thread* threads = new std::thread[th_count];
        int step = length/th_count + 1;
	int overhead = length%th_count;
	//distributing the overhead evenly will be optimal for parallelism
	for (int i=0;i<th_count;i++){
            if(!overhead--)
		step--;
	    threads[i] = std::thread(transform<C,S>, step, container, product, transformation);
	    container+=step;
	    product+=step;
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
    bool* parallel_validate(int length, C* container, std::function<bool(C&)> validation, int th_count){
	bool* valid = new bool[length];
	if(th_count>length || th_count <= 0)
	    th_count = length;
        std::thread* threads = new std::thread[th_count];
        int step = length/th_count + 1;
	int overhead = length%th_count;
	//distributing the overhead evenly will be optimal for parallelism
	for (int i=0;i<th_count;i++){
            if(!overhead--)
		step--;
	    threads[i] = std::thread(validate<C>, step, container, valid, validation);
	    container+=step;
	    valid+=step;
        }
        for (int i=0;i<th_count;i++){
            threads[i].join();
        }
	return valid;
    }
}

#endif
