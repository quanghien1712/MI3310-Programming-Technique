#pragma once

#include<stdexcept>

template<typename T>
class DynamicArray {
    private:
        T* arr;
        int currentSize;
        int maxSize;
        
        void resize(int newMaxSize) {
            T* newArr = new T[newMaxSize];
            for (int i = 0; i < currentSize; ++i) {
                newArr[i] = arr[i];
            }
            delete[] arr;
            arr = newArr;
            maxSize = newMaxSize;
        }
        
        void validateIndex(int index) const {
            if (index >= currentSize || index < 0) {
                throw std::out_of_range("Index nằm ngoài phạm vi truy cập");
            }
        }
        
    public:
        DynamicArray()
            : currentSize(0), maxSize(1), arr(new T[1]) {}

        DynamicArray(const DynamicArray& other)
            : currentSize(other.currentSize), maxSize(other.maxSize), arr(new T[other.maxSize]) {
            for (int i = 0; i < currentSize; ++i) {
                arr[i] = other.arr[i];
            }
        }

        ~DynamicArray() {
            delete[] arr;
        }

        DynamicArray& operator = (const DynamicArray& other) {
            if (this != &other) {
                delete[] arr;
                currentSize = other.currentSize;
                maxSize = other.maxSize;
                arr = new T[maxSize];
                for (int i = 0; i < currentSize; ++i) {
                    arr[i] = other.arr[i];
                }
            }
            return *this;
        }

        int size() const {
            return currentSize; 
        }

        bool isEmpty() const {
            return currentSize == 0;
        }

        void add(const T& value) {
            if (currentSize == maxSize) {
                resize(maxSize * 2);
            }
            arr[currentSize] = value;
            currentSize++;
        }

        void removeAt(int index) {
            validateIndex(index);
            for (int i = index; i < currentSize - 1; ++i) {
                arr[i] = arr[i + 1];
            }
            currentSize--;
        }

        // Lấy ra và có thể thay đổi phần tử
        T& operator [] (int index) { 
            validateIndex(index);
            return arr[index];
        }

        // Lấy ra và không thể thay đổi phần tử
        const T& operator [] (int index) const {
            validateIndex(index);
            return arr[index];
        }

        void clear() {
            currentSize = 0;
        }

};