#include <functional>

// a minimal vector class, none of the clunkiness and none of the mess
// update: now empowered by higher order functions
template <class C> struct hector{//doubling strategy
    int size;
    int capacity;
    C* container;
    std::function<void (C*)> dtor;//takes a lambda expression for explicit clean up
    hector(int = 0, std::function<void (C*)> = NULL);
    C& operator[](int);
    int insert(C&);
    int remove(C&);
    void resize();
    hector<C> filter(std::function<bool (C&)>);
    template <class S> hector<S> map(std::function<S(C&)>);
    template <class S> S foldr(std::function<S(S,C&)>,S,int=0);
    template <class S> S foldl(std::function<S(S,C&)>,S,int=0);
    int length();
    void cleanup();
    ~hector();
};//TODO: filter map fold

template <class C>
hector<C>::hector(int size,std::function<void (C*)> d):size(size),dtor(d){
    capacity = 16;
    while(size>capacity){
        capacity*=2;
    }
    container = new C[capacity];
}

template <class C>
C& hector<C>::operator[](int i){
    if(i>size || i<-size){
        throw "Index out of bound";
    }
    else if(i<0){// a convenient python like reverse element access function
        i+=size;
    }
    return container[i];
}

template <class C>
int hector<C>::length(){return size;}

template <class C>
int hector<C>::insert(C& content){
    if(size==capacity){
        capacity*=2;
        C* newcon = new C[capacity];
        for(int i=0;i<size;i++){
            newcon[i]=container[i];
        }
        delete[] container;
        container = newcon;
    }
    container[size++]=content;
    return size-1;
}

//unordered array -> O(n) runtime
template <class C>
int hector<C>::remove(C& content){
    bool shift = false;
    for(int i=0;i<size;i++){
        if (shift){
            container[i-1] = container[i];
        }
        if(container[i]==content){
            shift = true;
            if(dtor==NULL)
                dtor(container+i);
        }
    }
    size--;
    resize();
}

template <class C>
void hector<C>::resize(){
    if(size*4>capacity)
        return;
    capacity/=4;
    C* newcon = new C[capacity];
    for(int i=0;i<size;i++){
        newcon[i] = container[i];
    }
    delete[] container;
    container = newcon;
}

template<class C>
hector<C> hector<C>::filter(std::function<bool(C&)> f){
    hector<C> result{0};
    for(int i=0;i<size;i++){
        if(f(container[i])){
            result.insert(container[i]);
        }
    }
    return result;
}

template <class C>
template <class S>
hector<S> hector<C>::map(std::function<S(C&)> fun){
    hector<S> result{size};
    for(int i=0;i<size;i++){
        result[i] = fun(container[i]);
    }
    return result;
}

template <class C>
template <class S>
S hector<C>::foldr(std::function<S(S,C&)> fun, S state, int i){
    if(i==size)
        return state;
    return foldr(fun,fun(state,container[i]),i+1);
}

template <class C>
template <class S>
S hector<C>::foldl(std::function<S(S,C&)> fun, S state, int i){
    if(i==size)
        return state;
    return fun(foldl(fun,state,i+1),container[i]);
}

template <class C>
void hector<C>::cleanup(){
    if(dtor){
        for(int i=0;i<size;i++){
            dtor(container+i);
        }
    }
    delete[] container;
}

template <class C>
hector<C>::~hector(){
    cleanup();
}
