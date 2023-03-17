#include "../mypthreads/mypthreads.h"
#include <gtest/gtest.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void *sleep_fn(void *arg) {
  int t = *(int *)arg;
  sleep(t);
  return NULL;
}

void *sleep_and_inc(void *arg) {
  intptr_t *t = (intptr_t *)arg;
  int *val = (int *)(t[1]);
  sleep(*t);
  int x = *val;
  x += 1;
  *val = x;
  printf("After %d s, cnt=%d\n", *t, *val);
  int *ret = (int *)calloc(1, sizeof(int));
  *ret = x;
  return ret;
}

TEST(MypthreadsTest, SleepVar) {
  int cnt = 0;
  intptr_t one_sec_params[2];
  intptr_t two_sec_params[2];

  one_sec_params[0] = 1;
  two_sec_params[0] = 2;
  one_sec_params[1] = intptr_t(&cnt);
  two_sec_params[1] = intptr_t(&cnt);

  mypthread_t first_thread, second_thread;
  time_t start = time(NULL);
  EXPECT_EQ(0, mypthread_create(&first_thread, sleep_and_inc, one_sec_params));
  EXPECT_EQ(0, mypthread_create(&second_thread, sleep_and_inc, two_sec_params));
  void *result;
  EXPECT_EQ(0, mypthread_join(first_thread, &result));
  EXPECT_EQ(*(int *)result, 1);
  free(result);
  EXPECT_EQ(0, mypthread_join(second_thread, &result));
  EXPECT_EQ(*(int *)result, 2);
  free(result);
  time_t finish = time(NULL);
  double diff = difftime(finish, start);
  printf("Elapsed %f s\n", diff);

  EXPECT_EQ(2, cnt);
  EXPECT_LT(diff, 2.5);
}

TEST(MypthreadsTest, Detach) {
  int sleep_t = 2;

  time_t start = time(NULL);
  mypthread_t first_thread, second_thread;
  EXPECT_EQ(0, mypthread_create(&first_thread, sleep_fn, &sleep_t));
  EXPECT_EQ(0, mypthread_detach(first_thread));
  time_t finish = time(NULL);
  double diff = difftime(finish, start);
  printf("Elapsed %f s\n", diff);
  EXPECT_LT(diff, 1);
}
