#ifndef HYDRA
#define HYDRA

#include <thread>
#include <functional>

namespace hydra{

    template<class C, class S>
    void transform(C& base, S* product, std::function<S(C&)> transformation){
        *product = transformation(base);
    }

    template<class C, class S>
    void parallel_transform(int length, C* container, S* product, std::function<S(C&)> transformation){
        std::thread* threads = new std::thread[length];
        for (int i=0;i<length;i++){
            threads[i] = std::thread(transform<C,S>, std::ref(container[i]), product+i, transformation);
        }
        for (int i=0;i<length;i++){
            threads[i].join();
        }
    }

    template<class C>
    void validate(C& base, bool* valid_bit, std::function<bool(C&)>validation){
        *valid_bit = validation(base);
    }

    template<class C>
    bool* parallel_validate(int length, C* container, std::function<const bool(C&)> validation){
        bool* validate_array = new bool[length];
        std::thread* threads = new std::thread[length];
        for (int i=0;i<length;i++){
            threads[i] = std::thread(validate<C>, std::ref(container[i]), validate_array+i, validation);
        }
        for (int i=0;i<length;i++){
            threads[i].join();
        }
    }
}
#endif