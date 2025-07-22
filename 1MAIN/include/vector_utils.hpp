//
// Created by n on 22/07/25.
//

#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H
#include <string>
#include <vector>

#include "capabilities.hpp"
#include "Handle.hpp"

// Template implementation in header file
template <typename T>
bool is_in(std::vector<T> &v, const T x)
{
    // create an iterator
    typename std::vector<T>::iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        if (*it == x)
            return true;
    }
    return false;
}

#endif //VECTOR_UTILS_H
