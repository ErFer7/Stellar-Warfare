#ifndef dynamic_array_h
#define dynamic_array_h

#include <stdexcept>

#include "../os/include/traits.h"

__USING_API

template <typename T>
class DynamicArray {
   public:
    DynamicArray() {
        this->_size = 0;
        this->_array = nullptr;
    }

    DynamicArray(unsigned int initial_size, T empty_value) {
        this->_size = initial_size;
        this->empty_value = empty_value;
        this->_array = new T[this->_size];

        for (unsigned int i = 0; i < this->_size; i++) {
            this->_array[i] = this->empty_value;
        }
    }

    ~DynamicArray() {
        if (this->_array) {
            delete[] this->_array;
        }
    }

    // Os valores são operados sem a verificação de intervalo
    T& operator[](unsigned int index) { return this->_array[index]; }

    void fill(T value) {
        for (unsigned int i = 0; i < this->_size; i++) {
            this->_array[i] = value;
        }
    }

    unsigned int add(const T& value) {
        for (unsigned int i = 0; i < this->_size; i++) {
            if (this->_array[i] == this->empty_value) {
                this->_array[i] = value;
                return i;
            }
        }

        this->_size++;
        T* new_array = new T[this->_size];

        for (unsigned int i = 0; i < this->_size - 1; i++) {
            new_array[i] = this->_array[i];
        }

        new_array[this->_size - 1] = value;

        delete[] this->_array;
        this->_array = new_array;

        return this->_size - 1;
    }

    inline unsigned int size() { return this->_size; };

   private:
    T* _array;
    T empty_value;
    unsigned int _size;
};

#endif