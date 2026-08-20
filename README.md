# std-freestanding-nolibc

`memcpy`, `memmove`, `memset`, `memcmp` and `strlen`, for targets that have no
C library.

```toml
[dependencies]
std-freestanding-nolibc = "0.1.0"
```

## ⚠️ For the zero-libc tier only

mcpp contributes a dependency package's object files to the consumer's link
unconditionally; there is no archive semantics by which a definition would apply
only when the symbol is otherwise missing. A project whose board package links
`-lc` therefore gets two definitions of `memcpy` and fails.

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
