// The C functions a freestanding C++ translation unit still reaches for.
//
// WHY THIS IS A SEPARATE PACKAGE AND NOT PART OF THE SUBSET
//
// These are C library symbols. A board package that links `-lc` already has
// them, and mcpp contributes a dependency package's object files to the
// consumer's link unconditionally — there is no archive semantics to make a
// definition apply only when one is missing. Placing them in the subset would
// therefore make every ordinary bare-metal project fail with a duplicate
// definition. They belong to the projects that have no C library at all.
//
// WHY THESE FIVE
//
// Measured on riscv64-none-elf: a translation unit using array, ranges::sort,
// optional, atomic, span, string_view and to_chars references exactly
// `memmove`, `strlen`, and two symbols the subset package supplies itself. The
// remaining three are here because the compiler is permitted to emit calls to
// them from code that names none of them:
//
//   [intro.compliance] a freestanding implementation still provides memcpy,
//   memmove, memset and memcmp, because the compiler lowers structure
//   assignment, array initialisation and comparison onto them.
//
// So four of the five are an ABI obligation rather than a library convenience,
// and `strlen` is the one genuine C library call — reached from constructing a
// `string_view` out of a character pointer.
//
// ⚠️ CORRECTNESS OVER SPEED, DELIBERATELY.
//
// These are byte-at-a-time implementations. A C library's versions are word-at-
// a-time and vectorised, and any project that cares should be using one. The
// audience here is a kernel or a bootloader before its own C library exists,
// where being correct and having no dependencies is the whole requirement.

using size_type = __SIZE_TYPE__;

extern "C" {

void* memcpy(void* dst, const void* src, size_type n) {
    auto* d = static_cast<unsigned char*>(dst);
    const auto* s = static_cast<const unsigned char*>(src);
    for (size_type i = 0; i < n; ++i) d[i] = s[i];
    return dst;
}

void* memmove(void* dst, const void* src, size_type n) {
    auto* d = static_cast<unsigned char*>(dst);
    const auto* s = static_cast<const unsigned char*>(src);
    if (d == s || n == 0) return dst;
    // Overlap: copy in whichever direction leaves the source intact ahead of
    // the write. This is the whole difference from memcpy and the reason
    // libc++ calls this one from every container move.
    if (d < s) { for (size_type i = 0; i < n; ++i) d[i] = s[i]; }
    else       { for (size_type i = n; i-- > 0;)   d[i] = s[i]; }
    return dst;
}

void* memset(void* dst, int c, size_type n) {
    auto* d = static_cast<unsigned char*>(dst);
    const auto v = static_cast<unsigned char>(c);
    for (size_type i = 0; i < n; ++i) d[i] = v;
    return dst;
}

int memcmp(const void* a, const void* b, size_type n) {
    const auto* x = static_cast<const unsigned char*>(a);
    const auto* y = static_cast<const unsigned char*>(b);
    for (size_type i = 0; i < n; ++i)
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    return 0;
}

size_type strlen(const char* s) {
    const char* p = s;
    while (*p) ++p;
    return static_cast<size_type>(p - s);
}

}  // extern "C"
