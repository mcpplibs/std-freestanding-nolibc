#ifndef _MCPP_NOLIBC_STDIO_H
#define _MCPP_NOLIBC_STDIO_H
/* ⚠️ ONE MACRO, AND NOTHING ELSE.
 *
 * The standard library's character traits compare against EOF and reach this
 * header for that constant alone. An earlier version also declared `FILE`,
 * which made libc++'s <cstdio> fail with "target of using declaration
 * conflicts with declaration already in scope": it hoists a set of names into
 * namespace std, and a partial set here disagrees with the set it expects.
 *
 * A shim that declares LESS is therefore not merely smaller, it is more
 * correct: what it does not name, it cannot name wrongly. */
#define EOF (-1)

/* ⚠️ ONE FUNCTION, AND IT IS NOT AN OVERSIGHT THAT IT HAS NO DEFINITION.
 *
 * libc++'s <cstdio> hoists the C names into namespace std with
 * `using ::remove _LIBCPP_USING_IF_EXISTS;`. Where `::remove` does not exist,
 * that declaration collides with `std::remove` — the ALGORITHM, already in
 * scope — and every header that reaches <cstdio> fails with "target of using
 * declaration conflicts with declaration already in scope". Measured: <vector>,
 * <string>, <bitset> and <queue> all fail on exactly that line.
 *
 * Declaring it gives the using-declaration a target and the collision goes
 * away. A program that CALLS it fails at the link, naming `remove` — which is
 * the truthful outcome on a target with no filesystem. */
#ifdef __cplusplus
extern "C" {
#endif
int remove(const char*);
#ifdef __cplusplus
}
#endif
#endif
