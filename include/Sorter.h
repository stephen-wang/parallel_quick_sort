#ifndef SORTER_H
#define SORTER_H

//
// Class Sorter implements traditional quick sort and parallel quick sort. The
// parallel version is based on multi-threads: every time the data to be sorted
// is partitioned into two parts, the location pairs of the news parts are put
// into shared list, which is accessed exclusively by fixed number of threads.
// These threads fetch location pair from the shared list and proceeds to do
// partition and put new location pairs into shared list, and so on and so
// forth until all data are sorted.
//
//  Note: if the number of data to be sorted is less than minimum threshold
// (default 5000), parallel quick sort falls back to normal quick sort.
//

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <future>
#include <iostream>
#include <list>
#include <mutex>

using namespace std::chrono_literals;

class Sorter{
public:
    static void set_thread_num(int num)
    {
        threads_num_ = num;
    }

    static void set_parallel_gate(int gate)
    {
        parallel_gate_ = gate;
    }

    //
    //  Normal quick sort based on recursion
    //
    template<typename ItType>
    static void quick_sort(ItType first, ItType last)
    {
        auto distance = std::distance(first, last);
        if (distance < 2) {
            return;
        } else if (distance == 2 && (*first > *(first+1))) {
            std::swap(*first, *(first+1));
        }

        auto mid = partition(first, last);
        quick_sort(first, mid);
        quick_sort(mid+1, last);
    }

    //
    // Public interface of parallel quick sort
    //
    template<typename ItType>
    static void parallel_quick_sort(ItType first, ItType last)
    {
        if (std::distance(first, last) < parallel_gate_) {
            quick_sort(first, last);
        } else {
            // Initialize shared location pair list
            std::list<std::pair<ItType, ItType>> tasks;
            tasks.push_front(std::make_pair(first, last));

            // Start worker threads for sorting, the number is configurable
            std::vector<std::thread> threads;
            for (int i=0; i < threads_num_; i++) {
                std::thread t1(do_parallel_quick_sort<ItType>, &tasks);
                threads.push_back(std::move(t1));
            }

            for (int i=0; i < threads_num_; i++) {
                threads[i].join();
            }

        }

    }

private:
    //
    // Internal implementation of parallel quick sort
    //
    template<typename ItType>
    static void do_parallel_quick_sort(std::list<std::pair<ItType, ItType>> *pTasks)
    {
        while (true) {
            std::unique_lock<std::mutex> lck(mutex_);
            std::list<std::pair<ItType, ItType>> &tasks = *pTasks;

            //if (tasks.size() > max_public_list_size_) {
            //    max_public_list_size_ = tasks.size();
            //}

            if (!tasks.empty()) {
                // Fetch location pair, do partition, put new location pairs
                // into shared list
                task_num_++;
                auto first = tasks.back().first;
                auto last = tasks.back().second;

                tasks.pop_back();
                lck.unlock();

                auto mid = partition(first, last);
                if (std::distance(first, mid) >= parallel_gate_) {
                    lck.lock();
                    tasks.push_front(std::make_pair(first, mid));
                    cv_.notify_one();
                    lck.unlock();
                } else {
                    quick_sort(first, mid);
                }

                if (std::distance(mid+1, last) >= parallel_gate_) {
                    lck.lock();
                    tasks.push_front(std::make_pair(mid+1, last));
                    cv_.notify_one();
                    lck.unlock();
                } else {
                    quick_sort(mid+1, last);
                }

                task_num_--;
            } else {
                // If shared list is empty, let's take a break
                auto timeout = !cv_.wait_for(lck, 1s, [&tasks](){return !tasks.empty();});
                if (timeout && task_num_.load()==0) {
                    //std::cout << std::this_thread::get_id() << " bails out" << std::endl;
                    break;
                }
            }
        }
    }

    //
    // Take last number as pivot, partition data list into two parts: the
    // 1st part is less than/equal to pivot, the 2nd part is larger than pivot.
    //
    // Return the first data of the 2nd part.
    //
    template<typename IterType>
    static IterType partition(IterType first, IterType last)
    {
        // Always pick the last data as pivot, can be optimized more
        auto pivot = *(last-1);

        IterType it1 = first;
        IterType it2 = first;
        while (it2+1 != last) {
            if (*it2 <= pivot) {
                std::swap(*it1, *it2);
                it1++;
            }

            it2++;
        }

        std::swap(*it1, *(last-1));
        return it1;
    }

private:
    // how many threads are used for parallel sorting
    static int threads_num_;

    // the minimum data number for parallel sorting
    static int parallel_gate_;

    // number of threads doing parallel sorting
    static std::atomic<int> task_num_;

    // lock guarding public list
    static std::mutex mutex_;

    // the primitive synchronizing threads
    static std::condition_variable cv_;

public:
    static unsigned max_public_list_size_;
};

int Sorter::threads_num_ = 4;
int Sorter::parallel_gate_ = 5000;
std::atomic<int> Sorter::task_num_(0);
std::mutex Sorter::mutex_;
std::condition_variable Sorter::cv_;


#endif // SORTER_H
