#ifndef HECTOR_OF_TROY
#define HECTOR_OF_TROY
#include <functional>

// a minimal vector class, none of the clunkiness and none of the mess
// uses doubling strategy for insertion time
// supports higher order functions
template <class C> class hector{
    int size;
    int capacity;
    C* container;
    std::function<void (C*)> dtor;//takes a lambda expression for explicit clean up
    std::function<void (C*,C*)> cpy;
    void cleanup();
    void resize();
    public:
    hector(int = 0, std::function<void (C*)> = NULL, std::function<void (C*,C*)> = NULL);
    hector(hector<C>&);
    hector(hector<C>&&);
    hector<C>& operator=(hector<C>&);
    hector<C>& operator=(hector<C>&&);
    C& operator[](int);
    int insert(C&);
    int remove(C&);
    hector<C> filter(std::function<bool (C&)>);
    template <class S> hector<S> map(std::function<S(C&)>,std::function<void(S*)> = NULL,std::function<void(S*,S*)> = NULL);
    template <class S> S foldr(std::function<S(S,C&)>,S,int=0);
    template <class S> S foldl(std::function<S(S,C&)>,S,int=0);
    int length();
    ~hector();
};

template <class C>
hector<C>::hector(int size,std::function<void (C*)> d, std::function<void (C*,C*)> c):size(size),dtor(d),cpy(c){
    capacity = 4;
    while(size>capacity){
        capacity*=2;
    }
    container = new C[capacity];
}

template <class C>
hector<C>::hector(hector<C>& o):capacity(o.capacity),size(o.size),container(new C[o.capacity]),dtor(o.dtor),cpy(o.cpy){
    for(int i=0;i<size;i++){
        if(cpy)
            cpy(container+i,o.container+i);
        else
            container[i]=o.container[i];
    }
}

template <class C>
hector<C>::hector(hector<C>&& o):capacity(o.capacity),size(o.size),container(o.container),dtor(o.dtor),cpy(o.cpy){
    o.container=NULL;
    o.size=0;
    o.capacity=0;
}

template <class C>
hector<C>& hector<C>::operator=(hector<C>& o){
    cleanup();
    capacity = o.capacity;
    size = o.size;
    container = new C[capacity];
    dtor = o.dtor;
    cpy = o.cpy;
    for(int i=0;i<size;i++){
        if(cpy)
            cpy(container+i,o.container+i);
        else
            container[i]=o.container[i];
    }
    return *this;
}

template <class C>
hector<C>& hector<C>::operator=(hector<C>&& o){
    int temp = size;
    size=o.size;
    o.size = temp;
    temp = capacity;
    capacity=o.capacity;
    o.capacity = temp;
    C* tptr = container;
    container=o.container;
    o.container = tptr;
    auto t1 = dtor;
    dtor=o.dtor;
    o.dtor = t1;
    auto t2 = cpy;
    cpy=o.cpy;
    o.cpy = t2;
    return *this;
}

template <class C>
C& hector<C>::operator[](int i){
    if(i>=size || i<=-size){
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
    int ix=-1;
    for(int i=0;i<size;i++){
        if (ix!=-1){
            container[i-1] = container[i];
        }
        if(container[i]==content){
            ix = i;
            if(dtor!=NULL)
                dtor(container+i);
        }
    }
    size--;
    resize();
    return ix;
}

template <class C>
void hector<C>::resize(){
    if(size*4>capacity)
        return;
    capacity/=2;
    C* newcon = new C[capacity];
    for(int i=0;i<size;i++){
        newcon[i] = container[i];
    }
    delete[] container;
    container = newcon;
}

template<class C>
hector<C> hector<C>::filter(std::function<bool(C&)> f){
    hector<C> result{0,dtor,cpy};
    for(int i=0;i<size;i++){
        if(f(container[i])){
            result.insert(container[i]);
        }
    }
    return result;
}

template <class C>
template <class S>
hector<S> hector<C>::map(std::function<S(C&)> fun,std::function<void(S*)> d,std::function<void(S*,S*)> c){
    hector<S> result{size,d,c};
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

#endif
