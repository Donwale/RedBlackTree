#include "r_b_tree.hpp"
#include <benchmark/benchmark.h>
#include <chrono>
#include <random>
#include <vector>

static void BM_instert_ascending_order(benchmark::State &state) {

  tree t;
  for (auto _ : state) {

    for (int i = 0; i < state.range(0); i++) {
      t.insert(i);
    }
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_instert_ascending_order)
    ->RangeMultiplier(8)
    ->Range(1 << 1, 1 << 17)
    ->Complexity(benchmark::oNLogN);

static void BM_remove_ascending_order(benchmark::State &state) {

  tree t;
  for (int i = 0; i < state.range(0); i++) {
    t.insert(i);
  }
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); i++) {
      t.remove(i);
    }
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_remove_ascending_order)
    ->RangeMultiplier(8)
    ->Range(1 << 1, 1 << 17)
    ->Complexity(benchmark::oNLogN);

static void BM_instert_from_middle(benchmark::State &state) {

  tree t;
  int middle = static_cast<int>(state.range(0) / 2);
  for (auto _ : state) {

    for (int i = 0; i < state.range(0); i++) {
      t.insert(middle + i);
      t.insert(middle - i);
    }
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_instert_from_middle)
    ->RangeMultiplier(8)
    ->Range(1 << 1, 1 << 17)
    ->Complexity(benchmark::oNLogN);

static void BM_remove_from_middle(benchmark::State &state) {

  tree t;
  int middle = static_cast<int>(state.range(0) / 2);
  for (int i = 0; i < state.range(0); i++) {
    t.insert(middle + i);
    t.insert(middle - i);
  }
  for (auto _ : state) {
    for (int i = 0; i < state.range(0); i++) {
      t.remove(middle + i);
      t.remove(middle - i);
    }
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_remove_from_middle)
    ->RangeMultiplier(8)
    ->Range(1 << 1, 1 << 17)
    ->Complexity(benchmark::oNLogN);

static void BM_instert_random(benchmark::State &state) {

  tree t;
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(0, state.range(0) + 10);

  std::vector<int> values;
  int next_value = 0;
  int not_correct = true;
  for (size_t i = 0; i < state.range(0); i++) {
    do {
      next_value = distribution(generator);
      not_correct = false;
      for (size_t j = 0; j < values.size(); j++)
        if (next_value == values[j])
          not_correct = true;

    } while (not_correct);
    values.push_back(next_value);
  }

  for (auto _ : state) {

    for (int i = 0; i < values.size(); i++) {
      t.insert(values[i]);
    }
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_instert_random)
    ->RangeMultiplier(8)
    ->Range(1 << 1, 1 << 14)
    ->Complexity(benchmark::oNLogN);

static void BM_remove_random(benchmark::State &state) {

  tree t;
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(0, state.range(0) + 10);

  int deleted_count = static_cast<int>(state.range(0) * 0.9);

  std::vector<int> values;
  int next_value = 0;
  int not_correct = true;
  for (size_t i = 0; i < state.range(0); i++) {
    do {
      next_value = distribution(generator);
      not_correct = false;
      for (size_t j = 0; j < values.size(); j++)
        if (next_value == values[j])
          not_correct = true;

    } while (not_correct);
    values.push_back(next_value);
  }

  std::vector<int> deleted_values;

  for (size_t i = 0; i < deleted_count; i++) {
    do {
      next_value = distribution(generator);
      not_correct = true;
      for (size_t j = 0; j < values.size(); j++)
        if (next_value == values[j])
          not_correct = false;

      for (size_t j = 0; j < deleted_values.size(); j++)
        if (next_value == deleted_values[j])
          not_correct = true;
    } while (not_correct);

    deleted_values.push_back(next_value);
  }

  for (int i = 0; i < values.size(); i++) {
    t.insert(values[i]);
  }

  for (auto _ : state) {

    for (int i = 0; i < deleted_values.size(); i++) {
      t.remove(deleted_values[i]);
    }
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_remove_random)
    ->RangeMultiplier(8)
    ->Range(1 << 1, 1 << 14)
    ->Complexity(benchmark::oNLogN);




BENCHMARK_MAIN();