# byStrand,noPolish settings incorrectly recognized as *true* by ccs under clang

Running ccs on my Mac (clang++), I get this failure:

```sh
[~/Dropbox/Sources/git-pbi/pbccs]$ bin/ccs --logLevel=DEBUG \
  --minZScore -100 --maxDropFrac 0.8 tests/data/100zmws.bam test.fq
Usage: ccs [OPTIONS] INPUT OUTPUT

ccs: error: option --byStrand: incompatible with --noPolish
```



We aren't specifying these flags and they do default to false, so why
are we getting this?  And why doesn't this happen on the g++/linux ci
build?

The problem is we made some site modifications to OptionParser.h
to streamline seemingly redundant conversion operator declarations in
the `optparse::Value` class (also making it capable of grokking "inf"
and "nan"; and also to fix some other issue... not sure what that
was.)  But unfortunately those changes seem to have made that code a
bit fragile!!


See `test.cpp` for a stripped-down version of the `Value` class in
question.

What happens when you compile/run this under different compilers?

```sh
[~/Dropbox/Sources/git-pbi/pbccs]$ g++-4.8 -g -std=c++11  ./test.cpp; ./a.out
0

[~/Dropbox/Sources/git-pbi/pbccs]$ clang++  -g -std=c++11  ./test.cpp; ./a.out
1
```

What's going on here is that the code *does not prescribe which
template types T will be instantiated*.  And since C++ has a lot of
terrible implicit conversions allowed, including for example implicit
conversion of pointers to a boolean context, it seems that under one
compiler (g++) it's instantiating `operator bool` and getting the
desired behavior; under clang++ it's deciding it can use the `operator
const char*` to get a boolean, and giving undesired behavior:

```sh
[~/Dropbox/Sources/git-pbi/pbccs]$ g++-4.8 -g -O0 -std=c++11  ./test.cpp; nm ./a.out | c++filt | grep Value::operator
0000000100000b2c T Value::operator char const*()
0000000100000b60 T Value::operator bool<bool, void>()

[~/Dropbox/Sources/git-pbi/pbccs]$  clang++ -g -std=c++11  ./test.cpp; nm ./a.out | c++filt | grep Value::operator
0000000100000be0 unsigned short Value::operator char const*()
```

(Aside: I don't understand the "unsigned short" business or the
visibility difference in the nm output)


This is either a compiler bug or unspecified behavior in the C++ spec,
and it's academic to me which it is because we shouldn't write code
that depends on this level of nuance.



## How do we fix this?

One thing I tried which *did not work* was to explicitly instantiate
the *operator bool*.  Even with it instantiated, clang++ preferred to
use the *operator const char** in the boolean context.  In retrospect
this makes sense, since the instantiation is only relevant at
link-time.

Anyway, two solutions I like are

- In `test-fix4.cpp`, we replace the qualified generic and `const
  char*` operators with a single qualified generic operator; this then
  calls out to a function template which has three implementations
  qualified by the type traits.

- In `test-fix5.cpp`, we dumb it down, explicity declaring every
  conversion operator inline.  This should leave no room for
  interpretation


These both work.  But looking at the nm output from compiling
`test-fix4.cpp`, the type signatures retain the "enable_if" construct
even though they reduce to simple type.  And frankly, it's not clear
why `test-fix4.cpp` works when the original code failed.  Clearly the
compiler got confused by the presence of the two operators in the
class; going back to a single unqualified operator and dealing with
the details in instantiations seems to have helped.

So you know what?  When your code is too clever to be compiled
reliably, it's a sign that you need to dumb down your code.  So I
think `test-fix5.cpp` is in fact the best solution---clear to the
reader, clear to the compiler, clear to the person attempting to debug
a failure here in the future.


## Lesson

Write boring, dumb C++ code.  Don't try to be clever in C++, it is not
Haskell or even Rust and the type system *will* let you down.
