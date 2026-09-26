# Polygon conventions

## Repository discovery

Treat the repository as the source of truth:

1. Find the statement matching the user's title or content.
2. Compare at least two completed problem directories when available.
3. Prefer the newest complete convention over older partial layouts.
4. Map statement English names to root problem directories when that is the established layout.
5. Check for nested `AGENTS.md` before touching files.

The usual deliverable is:

```text
Problem_Name/
├── checker.cpp                 # when the output requires special judging
├── generator.cpp
├── script_generator.cpp
├── script.txt
├── testlib.h
└── validator.cpp
```

Omit or add files only when nearby completed problems establish a different convention.

## Special Judge

Create `checker.cpp` whenever contestants can output different valid witnesses, report impossibility, optimize a score, or otherwise need semantic output validation. Use `registerTestlibCmd(argc, argv)`.

Parse all test cases from `inf`, then parse and validate the complete `ans` and `ouf` outputs under the same grammar and semantic rules before comparing them. Check all of the following as applicable:

- every output token, row, count, range, fixed input condition, and witness invariant;
- the claimed construction by simulation or an exact verifier, rather than trusting its shape;
- all test cases and `seekEof()` on both `ans` and `ouf` so trailing tokens are rejected while trailing whitespace remains allowed;
- objective value, feasibility, and tolerance rules for optimization tasks.

Use `_fail` for a malformed or semantically invalid jury answer, and `_wa` for the corresponding participant error. For existence-style outputs such as `IMPOSSIBLE`, apply this matrix after both streams are fully validated:

| Jury answer | Participant answer | Verdict |
| --- | --- | --- |
| valid witness | valid witness | `_ok` |
| valid witness | `IMPOSSIBLE` | `_wa` |
| `IMPOSSIBLE` | valid witness | `_fail` |
| `IMPOSSIBLE` | `IMPOSSIBLE` | `_ok` |

For scoring tasks, replace the existence comparison with the documented objective comparison, and fail if the jury baseline is inconsistent. Keep checker diagnostics specific to the test case and violated condition.

## Validator

Use `testlib.h` and strict parsing:

```cpp
registerValidation(argc, argv);
int T = inf.readInt(minT, maxT, "T");
inf.readEoln();
for (int tc = 1; tc <= T; ++tc) {
    setTestCase(tc);
    // read fields with readInt/readLong, readSpace, readEoln
}
inf.readEof();
```

Apply these rules:

- Encode every stated numeric bound exactly.
- Check exclusions such as nonzero values with `ensuref`.
- Check aggregate limits immediately after each test is added to the running total.
- Check graph uniqueness, permutations, geometry validity, or other structural invariants when stated.
- Use descriptive indexed field names with `format` for diagnostics.
- Require exact spaces and line endings when repository validators do so.
- Reject trailing content with `readEof()`.
- Avoid semantic restrictions not present in the statement.

## Generator

Use the repository's standard testlib interface:

```cpp
int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    int mode = opt<int>(1);
    // dispatch mode
}
```

The command `generator <mode> <seed>` does not require manually parsing the seed; testlib incorporates command-line arguments into its RNG seed. Use `rnd`, not a separate time-based RNG.

Require every invocation to emit one complete input file, including `T`. Add helper functions that:

- validate constructed values with `ensuref` before narrowing to `int`;
- track `T`, per-case size, and aggregate size;
- print canonical whitespace;
- reject unknown modes with `ensuref(false, ...)`.

Prefer algebraic or combinatorial constructors with known outcomes. For planted YES cases, retain a witness and ensure every constraint accepts it. For planted NO cases, prove the blocker or complete coverage. Shuffle order when ordering should not matter, but preserve deliberate late-blocker modes.

## Recommended mode portfolio

Select only categories relevant to the problem, but cover correctness and performance:

1. Hand-crafted edge corpus.
2. Tiny exhaustive coefficients or structures.
3. Small random or brute-comparable cases.
4. Planted YES and planted NO cases.
5. Equality and strict-boundary cases.
6. Arithmetic extremes and overflow/precision traps.
7. Ordering, nesting, duplicates, disconnected parts, or state-reset traps.
8. Maximum `T` with aggregate size at its limit.
9. Maximum single `n` for expected-complexity pressure.
10. One or more cases aimed at plausible suboptimal algorithms.

Avoid modes that differ only cosmetically. Give deterministic modes one command and random modes several seeds. Give expensive maximum modes fewer seeds unless their randomness materially changes coverage.

## Script generator

Follow the established deterministic format:

```cpp
mt19937 g(114514);
freopen("script.txt", "w", stdout);
cout << "generator " << mode << ' ' << seed << " > $\n";
```

Use the actual Polygon generator source/program name. Emit positive seeds in a stable integer range. Compile and run this source locally so `script.txt` is delivered, not merely described.

## testlib.h

Polygon provides testlib during compilation, but a local copy enables standalone builds. If completed sibling problems include it, copy the same repository version and verify its hash rather than introducing another release.

## Validation checklist

1. Compile `validator.cpp`, `generator.cpp`, `script_generator.cpp`, and `checker.cpp` when present, independently.
2. Generate `script.txt` twice if determinism is important and compare it.
3. Run every mode with representative seeds and pipe output directly to the validator.
4. Confirm minimum and maximum constraint values occur where intended.
5. Confirm maximum-size modes actually hit the advertised bounds.
6. Use an independently written exact solver or brute force for semantic checks.
7. Compare planted-mode answer counts to construction expectations.
8. Test validator rejection for malformed formatting, out-of-range fields, excluded values, broken aggregates, and structural violations.
9. For a checker, test `_ok`, participant `_wa`, jury `_fail`, semantic witness rejection, and extra-output rejection with small hand-crafted input/answer/output files.
10. Inspect the final directory and remove unintended generated inputs or build artifacts.

## Common failures to prevent

- A generator mode silently falls back or emits an empty file.
- A random branch exceeds `sum n` or another aggregate constraint.
- Strict inequalities are generated or solved as non-strict ones.
- Test data never reaches coefficient or size limits.
- Random data is overwhelmingly one answer and misses hard structures.
- The script uses `./generator`, `.exe`, or a filename inconsistent with Polygon registration.
- The validator accepts trailing tokens, zero where forbidden, duplicate edges, or malformed layouts.
- Generated sources compile only because they accidentally include headers from another directory.
- A checker validates only `ouf`, trusts a malformed jury answer, skips later test cases, or accepts extra output.
- A checker compares `IMPOSSIBLE` claims before validating both streams, or fails to simulate/check the submitted witness.
