#ifndef matrix_h
#define matrix_h

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

    // TODO: Erros
    void set(int x, int y, const T& value) {
        if ((x >= 0 && x < get_width()) && (y >= 0 && y < get_height())) {
            this->_matrix[y * get_width() + x] = value;
        }
    }

    // TODO: Erros
    const T& get(int x, int y) const {
        if ((x >= 0 && x < get_width()) && (y >= 0 && y < get_height())) {
            return this->_matrix[y * get_width() + x];
        }
    }

    void rotate(bool clockwise) {
        T* new_matrix = new T[get_width() * get_height()];

        if (clockwise) {
            for (int i = 0; i < get_width(); i++) {
                for (int j = 0; j < get_height(); j++) {
                    new_matrix[i * get_height() + j] = this->_matrix[(get_height() - j - 1) * get_width() + i];
                }
            }
        } else {
            for (int i = 0; i < get_width(); i++) {
                for (int j = 0; j < get_height(); j++) {
                    new_matrix[i * get_height() + j] = this->_matrix[j * get_width() + (get_width() - i - 1)];
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