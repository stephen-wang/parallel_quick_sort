#include <iostream>

#include "DataGenerator.h"
#include "Sorter.h"
#include "utils.h"

extern "C" {
    #include "timeit.h"
}


int main()
{
    DataGenerator dg(5233, 70000000);
    int gate = 5000;
    int threads_nums[] = {4};
    int data_nums[] = {1000000, 5000000, 10000000};

    for (int i = 0; i < (sizeof threads_nums/sizeof threads_nums[0]); i++) {
        for (int j = 0; j < sizeof data_nums/sizeof data_nums[0]; j++) {
            auto nums = dg.get_int_nums(data_nums[j]);
            auto nums2(nums);
            std::cout << "Parallel Sorting: data num <" << data_nums[j] << ">, thread num <" << threads_nums[i]
                  << ">, parallel_gate <" << gate << ">,  time consumption ";
            timeit(1, Sorter::parallel_quick_sort, nums.begin(), nums.end());

            std::cout << "Traditional Quick Sorting: data num <" << data_nums[j] <<">, time consumption ";
            timeit(1, Sorter::quick_sort, nums2.begin(), nums2.end());
            std::cout << std::endl;
        }
    }
    return 0;
}
