#include <iostream>
#include "hector.h"

int main(){
    hector<int> h;
    int i;
    while(std::cin>>i){
	h.insert(i);
    }
    std::function<int(int&,int&)> cmp = [](int& a, int& b){return a-b;};
    h.sort(cmp);
    std::function<int(int&)> print = [](int& i){std::cout<<i<<std::endl;return 0;};
    h.map(print);
}
