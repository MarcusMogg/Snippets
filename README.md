# Snippets


## vs

```shell
xmake project -k vsxmake -m "debug,release"
```

## llvm

```shell
## can't understand, xmake only use ldflags when check_cxxsnippets
## xmake f -m release --toolchain=llvm --cxxflags=-stdlib=libc++ --ldflags=-stdlib=libc++ 
xmake mf llvm
```