/*
 * SSPtr, (Safe-Sturdy Pointer)
 *      Author : Mohamed ElSayed
 *      m.elsayed4420@gmail.com
 *      llcode.wordpress.com
 */


#include "indexer.hpp"
#include <iostream>
#include "ssptracker.hpp"
#include <thread>
#include <list>
#include <mutex>
#include <cstdlib>

std::mutex locker;
template <class T, int size>
std::thread tfree;

template <class T, int size = 0> class SSPtr {
private:
    static std::list<SSPTracker<T>> addrList;
    T *addr;
    bool isArray;
    unsigned arrSize;
    static bool initState;
    typename std::list<SSPTracker<T>>::iterator matchAddress(T* ptr) throw();
    static void freeFun();

public:
    typedef Index<T> SSPIndexer;
    typedef Exc SSPExc;
    SSPtr(T *t=nullptr);
    constexpr SSPtr(const SSPtr&);
    ~SSPtr();
    static void free();
    constexpr static void freeAll() noexcept;
    constexpr static int addrListSize() noexcept(true) { return addrList.size(); }
    T* operator=(T*) noexcept;
    SSPtr &operator=(SSPtr &) throw();

    T &operator*() const { return *addr; }
    T *operator->() const {return addr; }
    T &operator[](int i) const {
        if ( (i > size) || (i < 0) )
            throw SSPExc(i, size);
        return addr[i];
    }
    operator T*() const noexcept { return addr; }

    Index<T> first() {
        int tsize;
        isArray ? tsize = arrSize : tsize = 1;

        return Index<T>(addr, addr, addr + tsize);
    }

    Index<T> last() {
        int tsize;
        isArray ? tsize = arrSize : tsize = 1;

        return Index<T>(addr + tsize, addr, addr + tsize);
    }
};

template<class T, int size>
SSPtr<T, size>::SSPtr(T *t){
    if(initState)
        std::atexit(freeAll);
    initState = false;

    typename std::list<SSPTracker<T>>::iterator p;
    p = matchAddress(t);

    if(p != addrList.end()) {
        p->refCounter++;
    }
    else{
        SSPTracker<T> SSP(t, size);
        addrList.push_back(SSP);
    }

    addr = t;
    arrSize = size;
    if(size > 0) isArray = true;
    else isArray = false;
}

template<class T, int size>
constexpr SSPtr<T, size>::SSPtr(const SSPtr &ob){
    typename std::list<SSPTracker<T>>::iterator p;
    p = matchAddress(ob.addr);
    p->refCounter++;

    addr = ob.addr;
    arrSize = ob.arrSize;

    if(size > 0) isArray = true;
    else isArray = false;
}

template <class T, int size>
void SSPtr<T, size>::free() {
    tfree<T, size> = std::thread(freeFun);
    tfree<T, size>.joinable() ? tfree<T, size>.detach() : SSPtr<T, size>::freeFun();
}

template <class T, int size>
std::list<SSPTracker<T>> SSPtr<T, size>::addrList;

template <class T, int size>
bool SSPtr<T, size>::initState = true;

template <class T, int size>
SSPtr<T, size>::~SSPtr() {
    tfree<T, size> = std::thread(freeFun);
    typename std::list<SSPTracker<T>>::iterator p;
    p = matchAddress(addr);
    if(p->refCounter) p->refCounter-- ;

    tfree<T, size>.joinable() ? tfree<T, size>.join() : freeFun();
}

template <class T, int size>
void SSPtr<T, size>::freeFun() {
    std::lock_guard<std::mutex> guard (locker);

    typename std::list<SSPTracker<T>>::iterator p;
    do {
        for(p = addrList.begin(); p != addrList.end(); p++) {
            if(p->refCounter == 0) continue;

            addrList.remove(*p);

            if(p->memPtr) {
                if(p->isArray) {
                    delete [] p->memPtr;
                }
                else {
                    delete p->memPtr;
                }
            }
            break;
        }
    }while(p != addrList.end());
}

template <class T, int size>
auto SSPtr<T, size>::operator=(T *t) noexcept -> T* {
    typename std::list<SSPTracker<T>>::iterator p;
    p = matchAddress(addr);
    p->refCounter--;

    p = matchAddress(t);
    if(p != addrList.end())
        p->refCounter ++;
    else {
        SSPTracker<T> SSP(t, size);
        addrList.push_front(SSP);
    }

    addr = t;
    return t;
}

template <class T, int size>
auto SSPtr<T, size>::operator=(SSPtr &arg) throw() -> SSPtr<T, size>& {
    typename std::list<SSPTracker<T>>::iterator p;
    p = matchAddress(addr);
    p.refCounter --;

    p = matchAddress(arg.addr);
    p->refCounter++;
    addr = arg.addr;

    return arg;
}

template <class T, int size>
auto SSPtr<T, size>::matchAddress(T *ptr) throw() -> typename std::list<SSPTracker<T>>::iterator {
    typename std::list<SSPTracker<T>>::iterator p;

    for(p = addrList.begin(); p != addrList.end(); p++)
    {
        if(p->memPtr == ptr)
            return p;
    }
    return p;
}

template <class T, int size>
constexpr void SSPtr<T, size>::freeAll() noexcept {

    if(addrListSize() == 0) return ;

    tfree<T, size> = std::thread(freeFun);
    typename std::list<SSPTracker<T>>::iterator p;

    for(p = addrList.begin(); p != addrList.end(); p++)
    {
        p->refCounter = 0;
    }
    tfree<T, size>.joinable() ? tfree<T, size>.join() : freeFun();
}
