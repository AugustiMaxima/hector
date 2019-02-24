#include <iostream>
#include "../hector.h"

//A(somewhat skewed) example of how thread caps improve performance

int fib(const int& n){
    if (n==0 || n==1)
        return 1;
    else
        return fib(n-1) + fib(n-2);
}

int main(){
    int n = 10000;
    int it = 16;
    hector<int> seed{n};
    for (int i=0;i<n;i++){
        seed[i] = it;
    }
    std::function<int(int&)> fibn = fib;
    hector<int> fibs = seed.map(fibn, 8);
    //insert your maximum number of threads here.
    return 0;
}
