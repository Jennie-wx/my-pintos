#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"
#define diff(a,b) ({ \
  int64_t _a = (a), _b = (b); \
  _a > _b ? _a - _b : _b - _a; })

struct thread_info
{
  int64_t start_time;
  int tick_count;
  int priority;
};

void test_stride_thread(struct thread_info* arg);
struct thread_info* test_stride(int);
void test_stride_two (void)
{
  struct thread_info* args = test_stride(2);
  // wait for all threads to finish
  timer_sleep(11 * TIMER_FREQ);
  int total_pri = 0;
  for(int i = 0; i < 2; ++ i){
    msg("Thread %d with priority %d ran for %d ticks", i, args[i].priority, args[i].tick_count);
    total_pri += args[i].priority;
  }
  for(int i = 0; i < 2; ++ i){
    if (diff(args[i].tick_count, 10 * TIMER_FREQ / total_pri * args[i].priority) > 10) {
      fail("Wrong time! Actually: %d, which should be: %d", args[i].tick_count, 10 * TIMER_FREQ / total_pri * args[i].priority );
    }
  }
  pass ();
}

void test_stride_multiple (void)
{
  fail ("This test isn't a public test.");
}

void test_stride_overflow (void)
{
  fail ("This test isn't a public test.");
}

struct thread_info* test_stride(int thread_count) {
  struct thread_info *args = malloc(thread_count * sizeof(struct thread_info));
  int i;
  for(i = 0; i < thread_count; ++ i){
    args[i].start_time = timer_ticks();
    args[i].tick_count = 0;
    args[i].priority = (i + 1) * 2; // 2,4,6,8, etc.
    thread_create("stride", args[i].priority, (void (*))test_stride_thread, &args[i]);
  }
  return args;
}

void test_stride_thread(struct thread_info* ti){
  int64_t sleep_time = 1 * TIMER_FREQ;
  int64_t spin_time = sleep_time + 10 * TIMER_FREQ;
  int64_t last_time = 0;

//  thread_set_nice (ti->nice);
  timer_sleep (sleep_time - timer_elapsed (ti->start_time));
  while (timer_elapsed (ti->start_time) < spin_time)
  {
    int64_t cur_time = timer_ticks ();
    if (cur_time != last_time)
      ti->tick_count++;
    last_time = cur_time;
  }
}
