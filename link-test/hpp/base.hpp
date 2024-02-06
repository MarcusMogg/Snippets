#pragma once

// int f1() {
//   static int a{0};
//   return a++;
// }
extern "C" {
static int f2() {
  static int a{0};
  return a++;
}

inline int f3() {
  static int a{0};
  return a++;
}
}