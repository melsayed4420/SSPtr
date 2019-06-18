/*
 * SSPtr, (Safe-Sturdy Pointer)
 *      Author : Mohamed ElSayed
 *      m.elsayed4420@gmail.com
 *      llcode.wordpress.com
 */


#include <exception>
#include <cstdlib>
#include <string>
class Exc {
    int index_val, size;
public :

    Exc(){}
    Exc(int index_val_p, int size_p) :
        index_val(index_val_p), size(size_p) {}

    const std::string what() const throw() {
        return std::string ("Out of scope exception\nIndexer : " + std::to_string(index_val) + "\tArray size : " + std::to_string(size) + "\n");
    }
};

