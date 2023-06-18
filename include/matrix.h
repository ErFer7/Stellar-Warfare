#ifndef matrix_h
#define matrix_h

#include <stdexcept>

#include "../os/include/traits.h"

__USING_API

template <typename T>
class Matrix {
   public:
    Matrix() { this->_matrix = nullptr; }

    Matrix(int width, int height, const T& default_value) {
        this->_width = width;
        this->_height = height;
        this->_matrix = new T[width * height];

        for (int i = 0; i < width * height; i++) {
            this->_matrix[i] = default_value;
        }
    }

    ~Matrix() {
        if (this->_matrix) {
            delete[] this->_matrix;
            this->_matrix = nullptr;
        }
    }

    void fill(int width, int height, T* values) {
        for (int i = 0; i < width * height; i++) {
            this->_matrix[i] = values[i];
        }
    }

    void set(int x, int y, const T& value) {
        if ((x >= 0 && x < this->_width) && (y >= 0 && y < this->_height)) {
            this->_matrix[y * this->_width + x] = value;
        } else {
            throw std::out_of_range("x or y out of range");
        }
    }

    const T& get(int x, int y) const {
        if ((x >= 0 && x < this->_width) && (y >= 0 && y < this->_height)) {
            return this->_matrix[y * this->_width + x];
        } else {
            throw std::out_of_range("x or y out of range");
        }
    }

    void rotate(bool clockwise) {
        T* new_matrix = new T[this->_width * this->_height];

        if (clockwise) {
            for (int i = 0; i < this->_width; i++) {
                for (int j = 0; j < this->_height; j++) {
                    new_matrix[i * this->_height + j] = this->_matrix[(this->_height - j - 1) * this->_width + i];
                }
            }
        } else {
            for (int i = 0; i < this->_width; i++) {
                for (int j = 0; j < this->_height; j++) {
                    new_matrix[i * this->_height + j] = this->_matrix[j * this->_width + (this->_width - i - 1)];
                }
            }
        }

        delete[] this->_matrix;
        this->_matrix = new_matrix;

        int temp = this->_width;
        this->_width = this->_height;
        this->_height = temp;
    }

    inline int get_width() { return this->_width; };
    inline int get_height() { return this->_height; };

   private:
    T* _matrix;
    int _width;
    int _height;
};

#endif