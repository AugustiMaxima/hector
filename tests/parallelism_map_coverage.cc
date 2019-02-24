#include "../hector.h"
#include <iostream>
using namespace std;

//quick and dirty check for alignment problems with regards to map

int doubling(int& a){
    return a*2;
}

int main(){
    std::function<int(int&)> doubly = doubling;
    hector<int> a{5};
    for(int i=0;i<5;i++){
	a[i] = i+1;
    }
    hector<int> b{6};
    for(int i=0;i<6;i++){
	b[i] = i+1;
    }
    hector<int> c{7};
    for(int i=0;i<7;i++){
	c[i] = i+1;
    }
    hector<int> d{8};
    for(int i=0;i<8;i++){
	d[i] = i+1;
    }
    hector<int> a1 = a.map(doubly,4);
    hector<int> b1 = b.map(doubly,4);
    hector<int> c1 = c.map(doubly,4);
    hector<int> d1 = d.map(doubly,4);
    for(int i=0;i<5;i++)
	cout<<a1[i]<<endl;
    for(int i=0;i<6;i++)
	cout<<b1[i]<<endl;
    for(int i=0;i<7;i++)
	cout<<c1[i]<<endl;
    for(int i=0;i<8;i++)
	cout<<d1[i]<<endl;
}
