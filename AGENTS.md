# AGENTS.md — C++ course notes

This file is the operational contract for an autonomous coding agent.
Read it first, then read `policy.txt`, `codex/STATE.md`, and
`codex/BLOCK_WORKFLOW.md`.

## Scope

Project: `~/LATEX/projects/C++`
Git repository root: `~/LATEX/projects`
Git project prefix: `C++/`
Primary branch: `main`

This is a monorepo. Never modify, stage, reset, clean, commit, push, or
otherwise touch sibling projects outside `C++/`.

## One unit of work = one block

A **block** is exactly:
`lectures/NN_slug.tex`
plus
`examples/NN_slug/`

One Codex iteration works on one block only unless the user explicitly
authorizes a broader scope.

Every completed lecture must have at least 1–2 real practical artifacts:
programs, experiments, tests, CMake projects, compiler/linker experiments,
sanitizer runs, benchmarks, assembly investigations, or similarly executable
material appropriate to the lecture.

Substantial examples shown in the notes should exist as real files under the
matching `examples/NN_slug/` whenever practical.

## Before changing anything

1. Read `policy.txt`.
2. Read `codex/STATE.md`.
3. Read the target lecture and target examples directory.
4. Read the immediately previous completed lecture for continuity.
5. Inspect relevant local sources in `sources/` if present.
6. Inspect `main.tex`, `preamble.tex`, and helper scripts only as needed.
7. Run `git -C ~/LATEX/projects status --short -- C++/`.
8. Do not overwrite unrelated user changes.

## Technical discipline

Distinguish:
- standard guarantee;
- practical mental model;
- typical implementation;
- compiler/toolchain-specific behavior;
- historical teaching simplification;
- modern recommendation.

Prefer strong primary/official sources when available:
WG21 drafts, cppreference, GCC/Clang/MSVC docs, official CMake docs,
strong books and university materials.

Do not silently promote a teaching simplification to a standard guarantee.

## Pedagogy

Introduce concepts in dependency order. Early lectures use simple
abstractions. Do not drag advanced templates, ranges, concepts,
metaprogramming, custom framework helpers, or clever generic code into an
early block merely because it is shorter.

For an important construct explain, as appropriate:
problem -> syntax -> non-obvious parts -> compile time -> runtime ->
memory/lifetime/ownership -> hazards -> alternatives -> modern style ->
cost -> minimal example -> real example.

Code comments should be short but meaningful. Explain why, an invariant,
a non-obvious line, or an observed effect. Do not narrate trivial syntax.

## C++ policy

Primary standard: C++17.
Use C++20+ only when genuinely useful and label it explicitly.

Default style:
- no `using namespace std;` as the default style;
- use `const` where it communicates intent;
- RAII for resources;
- STL containers when manual memory is not the lesson;
- `std::unique_ptr` for unique ownership;
- `std::shared_ptr` only for real shared ownership;
- `override` for overrides;
- naked `new`/`delete` only when teaching them;
- avoid C-style casts.

## Tools

Lean 4 is available, but do not use it merely because it exists.
Use Lean only when a formal statement genuinely benefits from mechanized
formalization, and state why.

Use target-based CMake. Do not hide build behavior behind unexplained magic.

## LaTeX

Use the existing `preamble.tex`.
Do not duplicate global formatting machinery inside lectures.
Do not redesign `main.tex` during an ordinary block.

A lecture is connected to `main.tex` only after its content and experiments
pass validation. Keep numeric order and never input a lecture twice.

Main PDF: `build/c++.pdf`.
Temporary preview PDFs are validation artifacts only.

## Shell and generated files

Persistent `.sh` scripts must:
- be hard-bound to `~/LATEX/projects/C++` when mutating this project;
- print a colorized description of actions and danger;
- ask `y/n`;
- do nothing without `y`.

Do not use broad destructive commands such as `git reset --hard`,
`git clean -fd`, or broad `rm -rf`.

Generated artifacts belong in build/generated directories and should not be
committed.

## Git

Do not commit or push unless the user explicitly requests it.
Do not stage files outside `C++/`.
Prefer no staging during a normal Codex implementation iteration.

## Autonomous execution

Do not stop for small implementation choices that are resolvable from
`policy.txt`, existing style, sources, or the selected direction.

Work through:
baseline -> implementation -> experiments -> tests -> LaTeX build -> audit ->
self-fix -> final report.

Stop as BLOCKED only for a real ambiguity that materially changes content,
scope, or user-visible design and cannot be resolved from existing
instructions.

## Definition of done

A block is DONE only if:
- target lecture is substantively written;
- terminology is introduced before use;
- no obvious duplication with previous lectures;
- at least 1–2 real practical artifacts exist;
- C++ examples compile with appropriate warnings;
- runnable experiments actually run;
- requested tests/sanitizers/benchmarks pass;
- LaTeX builds;
- common `build/c++.pdf` builds after integration;
- no new unresolved references or duplicate labels are introduced;
- generated artifacts are ignored appropriately;
- `git diff --check -- C++/` passes;
- only intended block/global-support files changed;
- final report follows `codex/BLOCK_REPORT_TEMPLATE.md`.

Do not declare PASS merely because files were written.
