#pragma once

// int f1() {
//   static int a{0};
//   return a++;
// }

static int f2() {
  static int a{0};
  return a++;
}

inline int f3() {
  static int a{0};
  return a++;
}
