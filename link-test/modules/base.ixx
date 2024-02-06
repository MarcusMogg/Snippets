export module base;
export __attribute__((noinline)) int f1() {
  static int a{0};
  return a++;
}

export int f2() {
  static int a{0};
  return a++;
}

export inline int f3() {
  static int a{0};
  return a++;
}