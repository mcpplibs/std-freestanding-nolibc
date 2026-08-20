# std-freestanding-nolibc

The C surface a freestanding C++ library still needs — **five functions and four
headers** — for targets that have no C library.

⭐ **Measured: with this package, 94 of the 103 headers in `std-freestanding`
compile for a target whose `sysroot` is empty.** Without it, 15 of the 21
most-used ones do.

The obstacle was never that the subset wants a C library. libc++ ships
*wrappers* for the C headers — `include/c++/v1/string.h` and its siblings —
which reach the real header through `#include_next` to obtain `size_t`,
`mbstate_t`, `time_t` and `EOF`. With no C library the chain has nothing to
continue to, and the wrapper fails on a missing **type** rather than a missing
header, which is why the cause is not obvious from the error.

```toml
# Usually not named directly — the subset's `nolibc` feature brings it:
[dependencies]
std-freestanding = { version = "0.4.0", features = ["nolibc"] }
```

A project that uses no C++ standard library at all can depend on it directly,
for the five functions alone:

```toml
[dependencies]
std-freestanding-nolibc = "^0.2.0"
```

## ⚠️ For the zero-libc tier only, and the failure mode is silent

An earlier version of this note predicted a duplicate-definition error when a
board package also links `-lc`. That prediction was wrong, and measuring it is
what found the real hazard.

A C library ships as an **archive**, and an archive member is pulled only while
the symbol is still undefined. This package's object files enter the link
unconditionally, so they define `memcpy` first and the C library's member is
never pulled. The build succeeds.

> Measured on `riscv64-none-elf` with picolibc present: a cold build links, and
> `nm` reports exactly one definition of `memcpy` — this one.

⭐ **That is worse than an error.** The program silently receives the
byte-at-a-time implementations below in place of the C library's word-at-a-time
and vectorised ones, and nothing reports the substitution.

Use this package when the target's C library has been declined:

```toml
[target.riscv64-none-elf]
sysroot = ""
```

`mcpp new <name> --template riscv-virt-rt:nolibc` produces a project already in
that arrangement.

## Why these five

Four of them are an obligation rather than a convenience. [intro.compliance]
has a freestanding implementation provide `memcpy`, `memmove`, `memset` and
`memcmp`, because the compiler lowers structure assignment, array
initialisation and comparison onto them — a translation unit that names none of
them can still require them.

`strlen` is the one genuine library call, reached from constructing a
`std::string_view` out of a character pointer.

Measured on `riscv64-none-elf`: a translation unit using `array`,
`ranges::sort`, `optional`, `atomic`, `span`, `string_view` and `to_chars`
references exactly `memmove` and `strlen`, plus two symbols the subset package
supplies itself.

## Correctness over speed, deliberately

These are byte-at-a-time implementations. A C library's versions are
word-at-a-time and vectorised, and a project that cares should be using one. The
audience here is a kernel or a bootloader before its own C library exists, where
being correct and having no dependencies is the whole requirement.

⚠️ They are built with `-fno-builtin`. Without it a compiler is entitled to
recognise the body of `memcpy` as a memcpy and replace it with a call to itself,
and the resulting infinite recursion appears only under optimisation.
