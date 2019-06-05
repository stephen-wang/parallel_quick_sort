#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

//
// DataGenerator is a wrapper of random number generator based on std::mt19937
// and std::uniform_int_distribution<>.
//


#include <vector>
#include <random>

class DataGenerator
{
public:
    DataGenerator(int min_, int max_, int seed=std::random_device{}()):
        gen_{seed}, dist_{min_, max_} {}
    DataGenerator(const DataGenerator &) = delete;
    DataGenerator(DataGenerator &&) = delete;
    DataGenerator operator=(const DataGenerator &) = delete;
    DataGenerator operator=(DataGenerator &&) = delete;

    std::vector<int> get_int_nums(int size);
    int get_rand_int();

private:
    std::mt19937 gen_;
    std::uniform_int_distribution<int32_t> dist_;
};


#endif // DATAGENERATOR_H
