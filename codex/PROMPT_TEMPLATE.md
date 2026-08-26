# Prompt template for one Codex block

Read `AGENTS.md`, `policy.txt`, `codex/STATE.md`, and
`codex/BLOCK_WORKFLOW.md` first.

Work on exactly one block:

- lecture: `lectures/<NN_slug>.tex`
- practice: `examples/<NN_slug>/`

Selected direction:
`<DIRECTION>`

Goals:
`<GOALS>`

Theory scope:
`<THEORY_SCOPE>`

Practical scope:
`<EXPERIMENT_SCOPE>`

Sources:
`<SOURCES>`

Specific constraints:
`<CONSTRAINTS>`

Run the complete autonomous workflow:
baseline -> theory -> practical artifacts -> execution -> integration ->
full validation -> editorial audit -> self-fix -> final report.

Do not commit or push.
Do not modify another lecture block except for minimum authorized global
integration such as `main.tex`.

Resolve small implementation choices yourself. Stop only for a genuine
content/scope ambiguity that cannot be safely resolved.

Return the final report exactly in the format required by
`codex/BLOCK_REPORT_TEMPLATE.md`.
