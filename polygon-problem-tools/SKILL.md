---
name: polygon-problem-tools
description: Create, update, and validate Polygon-ready validator.cpp, generator.cpp, script_generator.cpp, script.txt, checker.cpp for special-judge tasks, and local testlib.h for algorithm-contest problems. Use when the user provides a problem statement and asks for Polygon data tools, 命题数据、validator、generator、数据生成脚本、script_generator、special judge、SPJ、checker，or asks to apply the established ECUSTPC problem-tooling conventions without restating them.
---

# Polygon Problem Tools

Deliver a complete, usable Polygon data-tooling directory rather than only suggesting code.

## Required workflow

1. Inspect `AGENTS.md`, the worktree status, nearby completed problem directories, and the matching statement/tutorial/solution materials.
2. Infer the target directory from repository conventions. Ask only when multiple materially different targets remain plausible.
3. Read [references/polygon-conventions.md](references/polygon-conventions.md) completely before editing Polygon sources.
4. Extract every input constraint and structural invariant from the statement. Do not invent unstated restrictions.
5. Derive the intended solution and likely wrong solutions before designing generator modes.
6. Determine the output judging model. For constructive, optimization, output-only, or any other non-unique-output task, implement or update `checker.cpp` and follow the special-judge conventions.
7. Implement the validator, generator, script generator, generated script, local `testlib.h` when the repository convention includes it, and `checker.cpp` when special judging is required.
8. Compile and validate all artifacts. Keep iterating until the sources and generated data pass the checks below.

## Scope discipline

- Preserve unrelated user changes in a dirty worktree.
- Modify the statement, tutorial, or standard solution only when explicitly requested or strictly required for consistency. Create or update `checker.cpp` when the output model requires special judging.
- Follow the nearest existing problem directory when it conflicts with generic conventions.
- Use Edit/Write tools for authored source changes.
- Do not commit changes or leave build executables in the problem directory unless requested or established by the repository.

## Test design

Build a targeted mode taxonomy from the problem's mathematics. Include applicable categories:

- samples and deterministic hand-crafted traps;
- tiny exhaustive or brute-force-friendly cases;
- small random cases with balanced or known answers;
- planted YES/NO constructions whose invariants are exact;
- strict-boundary, sign, indexing, duplicate, ordering, and state-reset cases;
- integer-overflow, floating-point, modular, or precision traps;
- maximum single-case size, maximum test count, and maximum aggregate size;
- performance cases that defeat plausible slower solutions;
- adversarial ordering and late blockers.

Do not rely on uniform random data alone. Randomize sound constructors, and check all generated coefficients, sizes, and aggregate limits inside the generator.

## Verification

- Compile sources with the statement's language standard, normally `g++ -std=c++17 -O2`, and enable useful warnings.
- Run `script_generator` and inspect `script.txt` for deterministic, valid Polygon commands.
- Pipe at least one seed of every mode through the validator; preferably validate every command in `script.txt`.
- Exercise validator rejection with representative malformed or out-of-range inputs.
- Verify semantic intent with an independent exact oracle or brute force. Check expected YES/NO distributions for planted modes.
- Exercise all advertised extrema such as maximum `n`, maximum `T`, aggregate bounds, and coefficient endpoints.
- For a special judge, compile `checker.cpp` and exercise valid jury/participant outputs, malformed or semantically invalid participant outputs, invalid jury outputs, impossible-vs-witness mismatches, and trailing-output rejection.
- Keep temporary oracles, executables, and generated large inputs outside the repository.

## Handoff

Report the created directory and files, the generator mode coverage, checker verdict branches when applicable, and the exact validation performed. Mention any assumption or check that could not be completed.
