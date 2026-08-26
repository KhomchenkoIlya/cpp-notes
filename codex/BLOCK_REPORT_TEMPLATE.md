# Codex block report template

Return:

```text
========== CODEX BLOCK REPORT ==========
BLOCK=NN_slug
STATUS=PASS|BLOCKED|FAIL
DIRECTION=<selected A/B/C/custom direction>

LECTURE=<path>
LECTURE_LINES=<integer>
LECTURE_CONNECTED_TO_MAIN=YES|NO

EXPERIMENTS=<integer>
EXPERIMENT_1=<short name>
EXPERIMENT_1_RESULT=PASS|FAIL
EXPERIMENT_2=<short name or NONE>
EXPERIMENT_2_RESULT=PASS|FAIL|NONE

CPP_BUILD=PASS|FAIL|N/A
TESTS=PASS|FAIL|N/A
SANITIZERS=PASS|FAIL|N/A
BENCHMARKS=PASS|FAIL|N/A
CMAKE=PASS|FAIL|N/A

LATEX_BUILD=PASS|FAIL
PDF=<path>
PDF_PAGES=<integer or N/A>
LATEX_WARNINGS=<integer or N/A>
UNRESOLVED_REFERENCES=<integer or N/A>
DUPLICATE_LABELS=<integer or N/A>

DIFF_CHECK=PASS|FAIL
FILES_CHANGED=<integer>
OUTSIDE_CPP_PROJECT_CHANGES=0|<integer>

LEAN_USED=NO|YES:<reason>

SELF_FIXES=<short summary or NONE>
BLOCKERS=<NONE or precise blocker>

GIT_COMMIT=NOT_RUN
GIT_PUSH=NOT_RUN
========== END CODEX BLOCK REPORT ==========
```

After the block, add only a few short sentences on the most important
technical/editorial choices. Do not dump full build logs unless diagnosis
requires them.
