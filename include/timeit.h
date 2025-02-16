/*
 * Copyright (c) 2019 wangwenzhi@sina.cn. All rights reserved.
 */

#ifndef TIMEIT_H_
#define TIMEIT_H_

#include <stdio.h>
#include <stdint.h>  // for int64_t
#include <string.h>  // for strlen


void get_desc_time(uint64_t ns, char *buf, int len)
{
	uint64_t nano_per_sec = 1000000000;
	uint64_t nano_per_milli = 1000000;
	uint64_t nano_per_micro = 1000;
	uint64_t unit = 1000;

	uint64_t nanos = ns % unit;
	uint64_t micros = ns / nano_per_micro % unit;
	uint64_t millis = ns / nano_per_milli % unit;
	uint64_t seconds = ns / nano_per_sec;

	memset(buf, '\0', len);
	if (seconds > 0) {
		snprintf(buf+strlen(buf), len-strlen(buf), "%llus", seconds);
	}

	if (millis > 0) {
		snprintf(buf+strlen(buf), len-strlen(buf), "%llums", millis);
	}

	if (micros > 0) {
		snprintf(buf+strlen(buf), len-strlen(buf), "%lluus", micros);
	}

	if (nanos > 0) {
		snprintf(buf+strlen(buf), len-strlen(buf), "%lluns", nanos);
	}
}

#if defined(__linux__) || defined(__APPLE__)
#include <time.h>    // for clock_gettime

#define timeit(n, func, ...) do {                                           \
    uint64_t duration;                                                      \
    struct timespec start, end;                                             \
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);                        \
                                                                            \
    for (int i=0; i<n; i++) {                                               \
        func(__VA_ARGS__);                                                  \
    }                                                                       \
                                                                            \
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);                          \
    duration = (end.tv_sec - start.tv_sec) * 1000000000                     \
                + (end.tv_nsec - start.tv_nsec);                            \
    char strSpan[256];                                                      \
    char strAvgSpan[256];                                                   \
    get_desc_time(duration, strSpan, sizeof(strSpan));                      \
    get_desc_time(duration/n, strAvgSpan, sizeof(strAvgSpan));              \
    if (n > 1) {                                                            \
        printf("%d loops, %s, avg: %s per loop\n", n, strSpan, strAvgSpan); \
    } else {                                                                \
        printf("avg: %s (%llu ns)\n", strAvgSpan, duration);                \
    }                                                                       \
} while (0)

#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>

#define timeit(n, func, ...) do {                                           \
    uint64_t duration;                                                      \
    LARGE_INTEGER start, end, frequency, elapse;                            \
    QueryPerformanceFrequency(&frequency);                                  \
    QueryPerformanceCounter(&start);                                        \
                                                                            \
    for (int i=0; i<n; i++) {                                               \
        func(__VA_ARGS__);                                                  \
    }                                                                       \
                                                                            \
    QueryPerformanceCounter(&end);                                          \
    elapse.QuadPart = (end.QuadPart - start.QuadPart) * 1000000;            \
    elapse.QuadPart /= frequency.QuadPart;                                  \
    duration = elapse.QuadPart * 1000;                                      \
                                                                            \
    char strSpan[256];                                                      \
    char strAvgSpan[256];                                                   \
    get_desc_time(duration, strSpan, sizeof(strSpan));                      \
    get_desc_time(duration/n, strAvgSpan, sizeof(strAvgSpan));              \
    if (n > 1) {                                                            \
        printf("%d loops, %s, avg: %s per loop\n", n, strSpan, strAvgSpan); \
    } else {                                                                \
        printf("avg: %s (%lu ns)\n", strAvgSpan, duration);                 \
    }                                                                       \
} while (0)
#endif //defined(_WIN32) || defined(_WIN64)


#endif // TIMEIT_H_
