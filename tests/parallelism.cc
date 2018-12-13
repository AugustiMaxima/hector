#include <iostream>
#include "../hector.h"

int fib(const int& n){
    if (n==0 || n==1)
        return 1;
    else
        return fib(n-1) + fib(n-2);
}

int main(){
    int n = 36;
    hector<int> seed{n};
    for (int i=0;i<n;i++){
        seed[i] = n;
    }
    std::function<int(int&)> fibn = fib;
    hector<int> fibs = seed.map(fibn);
    return 0;
}