#include <functional>

// a minimal vector class, none of the clunkiness and none of the mess
template <class C> struct hector{
    int size;
    C* container;
    std::function<void (C*)> dtor;//takes a lambda expression for explicit clean up
    hector(int, std::function<void (C*)> = NULL);
    C& operator[](int);
    int length();
    ~hector();
};

template <class C>
hector<C>::hector(int size,std::function<void (C*)> d):size(size),dtor(d),container(new C[size]){}

template <class C>
C& hector<C>::operator[](int i){
    if(i>size || i<-size){
        throw "Error: Index out of bound";
    }
    else if(i<0){// a convenient python like reverse element access function
        i+=size;
    }
    return container[i];
}

template <class C>
int hector<C>::length(){return size;}

template <class C>
hector<C>::~hector(){
    if(dtor){
        for(int i=0;i<size;i++){
            dtor(container+i);
        }
    }
    delete[] container;
}
