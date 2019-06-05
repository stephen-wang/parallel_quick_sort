#include "DataGenerator.h"

int  DataGenerator::get_rand_int()
{
    return dist_(gen_);
}

std::vector<int> DataGenerator::get_int_nums(int size)
{
    std::vector<int> result(size);
    for (int i=0; i<size; i++) {
        result[i] = get_rand_int();
    }
    return result;
}
