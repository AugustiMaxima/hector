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
    void cleanup();
    public:
    hector(int = 0);
    hector(hector<C>&);
    hector(hector<C>&&);
    hector<C>& operator=(hector<C>&);
    hector<C>& operator=(hector<C>&&);
    C& operator[](int);
    int insert(C&);
    int remove(C&);
    void resize(int = 0);
    void sort(std::function<const int (C&,C&)>);//when the comparison function returns a value less than 0, the sorting algorithm places the first element before the second element
    hector<C> filter(std::function<const bool (C&)>);
    template <class S> hector<S> map(std::function<S(C&)>);
    template <class S> S foldr(std::function<S(S,C&)>,S,int=0);
    template <class S> S foldl(std::function<S(S,C&)>,S,int=0);
    int length();
    ~hector();
};

template <class C>
hector<C>::hector(int size):size(size){
    capacity = size? size: 4;
    container = new C[capacity];
}

template <class C>
hector<C>::hector(hector<C>& o):capacity(o.capacity),size(o.size),container(new C[o.capacity]){
    for(int i=0;i<size;i++){
        container[i]=o.container[i];
    }
}

template <class C>
hector<C>::hector(hector<C>&& o):capacity(o.capacity),size(o.size),container(o.container){
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
    for(int i=0;i<size;i++){
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
            newcon[i]=std::move(container[i]);
	    //implicit r-value copy, may save some time for classes that did implement r-value assignment
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
        if(container[i]==content){
            ix = i;
        }
	if (ix!=-1 && i<size-1){
            container[i] = std::move(container[i+1]);
        } 
    }
    size--;
    resize();
    return ix;
}

template <class C>
void hector<C>::resize(int nc){
    if(nc==0){
	if(size*4>capacity)
	    return;
	capacity/=2;
    }
    else{
	while(capacity<nc){
	    capacity*=2;
	}
	
    }
    C* newcon = new C[capacity];
    for(int i=0;i<size;i++){
        newcon[i] = std::move(container[i]);
    }
    if(nc>size){
	size = nc;
    }
    delete[] container;
    container = newcon;
	
}

template <class C>
void heapinsert(hector<C>& A, int n, int ptr, std::function<const int(C&,C&)> cmp){
    int si = ptr*2 + 1;
    if(ptr*2+2>n)
        return ;
    else if(ptr*2+2<n){
        if (cmp(A[ptr*2+1], A[ptr*2+2])<0) si++;
    }
    if(cmp(A[ptr],A[si])<0){
        C temp = std::move(A[si]);
        A[si] = std::move(A[ptr]);
        A[ptr] = std::move(temp);
        return heapinsert(A,n,si,cmp);
    }
}

template <class C>
void heapify(hector<C>& A, int n, std::function<const int(C&,C&)> cmp){
    C temp;
    for(int i=n-1;i>0;i--){
        if(cmp(A[(i-1)/2],A[i])<0){
            temp = std::move(A[(i-1)/2]);
            A[(i-1)/2] = std::move(A[i]);
            A[i] = std::move(temp);
            heapinsert(A,n,i,cmp);
        }
    }
}

//heapsort. not stable order.
//guaranteed runtime of O(nlog(n)) with very little memory overhead
template<class C> 
void hector<C>::sort(std::function<const int(C&,C&)> cmp){
    C temp;
    heapify(*this,length(),cmp);
    for(int i=length()-1;i>0;i--){
	temp = std::move(container[i]);
	container[i] = std::move(container[0]);
	container[0] = std::move(temp);
	heapinsert(*this,i,0,cmp);
    }
}

template<class C>
hector<C> hector<C>::filter(std::function<const bool(C&)> f){
    hector<C> result;
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
    delete[] container;
}

template <class C>
hector<C>::~hector(){
    cleanup();
}

#endif
