#pragma once

#include <opencv2/core.hpp>
#include <string.h>

template <class T>
class Histogram {
    public:
        Histogram(T min_input, T max_input);
        inline void insert(T input);
        void insert_image(cv::Mat& input);
        void print();
        void clear();
        ~Histogram();

        Histogram(const Histogram<T>& hist); //Copy constructor
        Histogram(Histogram<T>&& src) noexcept; //Copy constructor [C++11]
        Histogram<T>& operator=(Histogram<T> rhs); //Copy on assignment
        Histogram<T>& operator=(Histogram<T>&& rhs) noexcept; //Copy on assignment [C++11]
        void swap(Histogram<T>& rhs); //Swap
    private:
        T min_input, max_input;
        unsigned long accumulator;
        unsigned int* bins = nullptr;
        unsigned int bin_count();
        size_t array_size();
};

template <class T>
void Histogram<T>::clear() {
    memset(bins, 0, array_size());
}

template <class T>
void Histogram<T>::print() {
    for (int i = 0; i < bin_count(); i++) {
        printf("%i: %u\n", i + min_input, bins[i]);
    }
}

template <class T>
void Histogram<T>::insert(T input) {
    if (input >= min_input && input <= max_input) {
        bins[input - min_input]++;
        accumulator++;
    }
}

template <>
void Histogram<unsigned short>::insert_image(cv::Mat& input) {
    if (input.type() != CV_16UC1) throw std::logic_error("Matrix type must be CV_16UC1.");
    unsigned short* input_array = (unsigned short*)input.data;
    unsigned short* input_array_end = (unsigned short*)input.dataend;
    while (input_array < input_array_end) insert(*input_array++);
}

template <class T>
unsigned int Histogram<T>::bin_count() {
    return (max_input - min_input) + 1; //Include last and first
}

template <class T>
size_t Histogram<T>::array_size() {
    return sizeof(*bins) * bin_count();
}

template <class T>
Histogram<T>::Histogram(T min_input, T max_input) : min_input(min_input), max_input(max_input) {
    if (min_input > max_input) throw std::logic_error("Range has negative size.");
    bins = new unsigned int[bin_count()]();
}

template <class T>
Histogram<T>::Histogram(const Histogram<T>& hist) {
    accumulator = hist.accumulator;
    max_input = hist.max_input;
    min_input = hist.min_input;
    bins = new unsigned int[this->bin_count()]();
    memcpy(bins, hist.bins, this->array_size());
}

template <class T>
Histogram<T>& Histogram<T>::operator=(Histogram<T> rhs) {
    rhs.swap(*this);
    return *this;
}

template <class T>
Histogram<T>::Histogram(Histogram<T>&& src) noexcept {
    src.swap(*this);
}

template <class T>
Histogram<T>& Histogram<T>::operator=(Histogram<T>&& rhs) noexcept {
    rhs.swap(*this);
    return *this;
}

template <class T>
void Histogram<T>::swap(Histogram<T>& rhs) {
    std::swap(this->bins, rhs.bins);
    std::swap(this->accumulator, rhs.accumulator);
    std::swap(this->min_input, rhs.min_input);
    std::swap(this->max_input, rhs.max_input);
}

template <class T>
Histogram<T>::~Histogram() {
    delete[] bins;
}
