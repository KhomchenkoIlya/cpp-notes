# Block workflow

A block is one `lectures/NN_slug.tex` plus the matching
`examples/NN_slug/`.

## Phase 0 — selected direction

Before any Codex implementation prompt, the coordinating chat offers the user
exactly three meaningful directions for the next block.

The user chooses A/B/C, combines them, or proposes a custom direction.
The selected direction becomes part of the Codex prompt.

## Phase 1 — baseline

Read:
- `AGENTS.md`;
- `policy.txt`;
- `codex/STATE.md`;
- target block;
- previous completed lecture;
- relevant local sources.

Inspect project status and protect unrelated changes.

## Phase 2 — plan

Internally plan:
- lecture sections;
- dependency order;
- 1–2+ experiments;
- files to touch;
- validations.

Do not create extra planning files unless requested.

## Phase 3 — theory

Write from simple ideas toward deeper mechanics.
Separate standard guarantees from implementation models and recommendations.
Do not introduce advanced machinery prematurely.

## Phase 4 — practice

Implement real runnable/compilable artifacts in the matching examples
directory.

Use short meaningful comments. Isolate generated output. Add appropriate
`.gitignore`. Persistent shell scripts keep the y/n safety convention.

## Phase 5 — execute

Compile and actually run the practical artifacts.
If an experiment demonstrates diagnostics, linking, sanitizers, performance,
assembly, or UB detection, verify that the observed result really supports the
claim. Never fabricate output.

## Phase 6 — integrate

After the block passes, connect the lecture to `main.tex` exactly once and in
numeric order. Do not connect future empty lectures.

## Phase 7 — full validation

At minimum:
- relevant C++ build/tests;
- experiments run;
- full LaTeX build;
- unresolved-reference / duplicate-label check where applicable;
- `git diff --check -- C++/`;
- `git status --short -- C++/`;
- confirm no sibling project was touched.

Fix in-scope failures autonomously.

## Phase 8 — editorial audit

Check terminology order, unexplained magic, accidental advanced abstractions,
filler/repetition, code comments, continuity with the previous lecture, and
technical accuracy.

## Phase 9 — report

Return the report required by `BLOCK_REPORT_TEMPLATE.md`.
Do not commit or push unless explicitly requested.
