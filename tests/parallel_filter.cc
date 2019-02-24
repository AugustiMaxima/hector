#include <iostream>
#include "../hector.h"

bool fib(const int& n){
    int p;
    if (n==0 || n==1)
        p = 1;
    else
        p = fib(n-1) + fib(n-2);
    return p%2;
}

int main(){
    int n = 32;
    hector<int> seed{n};
    for (int i=0;i<n;i++){
        seed[i] = n/2 + 20;
    }
    std::function<bool(int&)> fibn = fib;
    hector<int> fibs = seed.filter(fibn);
    return 0;
}
