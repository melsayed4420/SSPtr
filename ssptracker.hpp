/*
 * SSPtr, (Safe-Sturdy Pointer)
 *      Author : Mohamed ElSayed
 *      m.elsayed4420@gmail.com
 *      llcode.wordpress.com
 */


template <class T> class SSPTracker {

public:
    volatile unsigned refCounter;
    T *memPtr;
    bool isArray;
    unsigned arraySize;

    SSPTracker(T *mPtr, unsigned size = 0):
    refCounter(1), memPtr(mPtr) {
        size != 0 ? isArray = true : isArray = false;
        arraySize = size;
    }
};

template <class T> bool operator==(const SSPTracker<T> &arg1, const SSPTracker<T> &arg2){
    return (arg1.memPtr == arg2.memPtr);
}

