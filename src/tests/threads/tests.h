#ifndef TESTS_THREADS_TESTS_H
#define TESTS_THREADS_TESTS_H

void run_test (const char *);

typedef void test_func (void);

extern test_func test_stride_one;
extern test_func test_stride_two;
extern test_func test_stride_multiple;
extern test_func test_stride_overflow;

void msg (const char *, ...);
void fail (const char *, ...);
void pass (void);

#endif /* tests/threads/tests.h */

