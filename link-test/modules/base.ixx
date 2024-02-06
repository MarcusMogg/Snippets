export module base;

export int f1() {
  static int a{0};
  return a++;
}

static int f2() {
  static int a{0};
  return a++;
}

export int expf2() { return f2(); }

export inline int f3() {
  static int a{0};
  return a++;
}

static int f4() {
  static int a{0};
  return a++;
}
export inline int expinlinef4() { return f4(); }

namespace {
int f5() {
  static int a{0};
  return a++;
}
}  // namespace
export inline int expf5() { return f5(); }