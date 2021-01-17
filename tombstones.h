using namespace std;

void printName() {
    //prints my name and course number
    cout << "Brian Klein\nCS-415\n";
}

void printDError() {
    //prints a message when you find a dangling pointer
    cout << "You may have a dangling reference.";
    exit(1);
}

void printMError() {
    //print a message when you find a memory leak
    cout << "You may have a memory leak.";
}

template <class T> class Ptr;
template <class T> class MyTombstone;

template <class T>
class MyTombstone {
    
public:

    T* pointerToObj;
    int refCount;
    bool deleted;
    
   MyTombstone<T>(){
       pointerToObj = NULL;
       refCount = 0;
       deleted = false;
   }
};

template <class T>
class Ptr {
public:
    
    //variables
    MyTombstone <T> *ptr;
    
    // default constructor
    Ptr<T>() {
        ptr = new MyTombstone<T>;
        ptr->deleted = false;
    }
    
    // copy constructor
    Ptr<T>(Ptr<T>& target) {
        if (target.ptr->deleted) {
            printDError();
        }
        ptr = target.ptr;
        if (ptr->refCount != 0) {
            ptr->refCount++;
        }
    }
    
    // bootstrapping constructor, argument should always be a call to new
    Ptr<T>(T* data) {
        //Create new tombstone using default constructor.
        ptr = new MyTombstone<T>();
    
        //Set data to call param
        ptr->pointerToObj = data;
        ptr->deleted = false;
    
        //Set count to one unless data is nullptr.
        //In this case assume user is trying to create a Ptn in a similar fashion to the default constructor.
        if (data == NULL) {
            ptr->refCount = 0;
        } else {
            ptr->refCount = 1;
        }
    }
    
    // deconstructor
    ~Ptr<T>() {
        ptr->refCount--;
        if (ptr->refCount < 1 && !ptr->deleted) {
            printMError();
        }
        ptr = NULL;
    }
    
    T& operator*() const{
        if(ptr->refCount==0){
            printMError();
        }
        else if(ptr->deleted == true){
            printDError();    // if rip is true, then it's a dangling reference exception
        }
            return *(ptr->pointerToObj); //otherwise return the actual value



    }
    T* operator->() const{
       if(ptr->refCount==0){
            printMError();// usually when reference counting reaches to 0, we are going to run into memory leak excetion
        }
        else if(ptr->deleted == true){
            printDError();    // if true, then it's a dangling reference exception
        }
            return ptr->pointerToObj; //otherwise return the the pointer to the object

    }

    Ptr<T>& operator=(const Ptr<T>& t){
        ptr->refCount--;
        if(ptr->refCount == 0){
            printMError();
        }
        ptr = t.ptr;
        ptr->refCount++;
        if(ptr->deleted){
            printDError();
        }
        return *this;
    }
    
    // delete pointed-at object
    // This is essentially the inverse of the new inside the call to the bootstrapping constructor.
    friend void release(Ptr<T>& t){
        // delete pointed-at object
        //cout<<"Free: "<<t.ptr->referenceCounting<<endl;
        if(t.ptr->deleted){
            printDError();    // if true, then it's a dangling reference exception
        }else{
            delete t.ptr->pointerToObj; //delete the pointer to the object
            t.ptr->deleted = true; // set this tombstone to deleted
        }
    }
    
    // equality comparisons:
    bool operator==(const Ptr<T>& target) const {
        if (ptr->deleted || (target.ptr->deleted)) {
            printDError();
        }

        return ptr->pointerToObj == (target.ptr->pointerToObj);
    }
    
    bool operator!=(const Ptr<T>& target) const {
//        if(ptr->deleted || (target.ptr->deleted)) {
//
//        }

        return ptr->pointerToObj != (target.ptr->pointerToObj);
    }
    
    // true iff Ptr is null and int is zero
    bool operator==(const int refInt) const {
        
        return ptr->pointerToObj == NULL && refInt == 0;
    }
    
    // false iff Ptr is null and int is zero};
    bool operator!=(const int refInt) const {
        
        return !(ptr->pointerToObj == NULL && refInt == 0);
    }
};
