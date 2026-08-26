# Current handoff state

## Project

Project root: `~/LATEX/projects/C++`
Git root: `~/LATEX/projects`
Branch: `main`
Remote: `https://github.com/KhomchenkoIlya/latex-projects.git`

## Structure

There are 15 lecture files and 15 matching example directories.

Mapping:
`lectures/NN_slug.tex <-> examples/NN_slug/`

One working unit is called a **block**.

## Completed content

Block 01:
`01_intro_and_first_program`

Known state:
- lecture 01 contains substantive material;
- two experiments exist:
  - compilation pipeline;
  - basic abstractions with string/vector/map and CMake;
- lecture 01 is connected to common `main.tex`;
- the common PDF has built successfully;
- the lecture intentionally begins with simple abstractions.

Block 02:
`02_declarations_flow_and_errors`

Known state:
- lecture 02 substantively covers declarations, state, expressions, control
  flow, scopes, error classes, warnings, and sanitizers;
- exactly two experiments exist:
  - `course_status`, a read-only inspector for all 15 lecture/example pairs;
  - `control_flow_lab`, with deterministic branching/loop/short-circuit cases
    and verified warning, compile-error, logic-error, and UBSan cases;
- lecture 02 is connected to common `main.tex` exactly once after lecture 01;
- C++17 builds, experiment runs, CTest, UBSan, and the common PDF pass.

Block 03:
`03_pointers_memory_and_arrays`

Known state:
- lecture 03 substantively covers the object/address model, pointers, raw
  arrays, array-to-pointer conversion, pointer arithmetic, `std::array`,
  `std::vector`, lifetime/invalidation, memory errors, and performance;
- exactly two experiments exist:
  - `course_inventory`, a read-only size/line/file inventory of all 15 blocks;
  - `array_vs_vector_lab`, with safe storage cases and three verified
    ASan/UBSan cases for out-of-bounds, dangling, and invalidated pointers;
- lecture 03 is connected to common `main.tex` exactly once after lecture 02;
- C++17 builds, warnings, runs, CTest, sanitizer diagnostics, and the common
  PDF pass.

Block 04:
`04_functions_references_and_const`

Known state:
- lecture 04 substantively covers functions as a design/decomposition tool,
  declarations/definitions/calls, value/reference/const-reference contracts,
  return values, scope/lifetime, overloads, default arguments, introductory
  function pointers, and an accurate cost model;
- exactly two experiments exist:
  - `course_inventory_refactored`, a functionally equivalent decomposition of
    the block-03 inventory utility;
  - `function_contract_lab`, with positive contract cases, three verified
    compile-failure cases, and an isolated ASan/UBSan dangling-return case;
- lecture 04 is connected to common `main.tex` exactly once after lecture 03;
- C++17 builds, warnings, equivalence checks, experiment runs, CTest,
  sanitizer diagnostics, and the common PDF pass.

Block 05:
`05_classes_and_encapsulation`

Known state:
- lecture 05 substantively covers user-defined types, struct/class, objects,
  members, access control, encapsulation, invariants, const member functions,
  meaningful interfaces, responsibility/cohesion, basic composition, lifetime,
  layout caveats, and an accurate cost model;
- exactly two experiments exist:
  - `course_inventory_classes`, a behavior-preserving class-based continuation
    of block 04 built around the encapsulated `CourseBlock` domain type;
  - `encapsulation_break_lab`, with positive public/private, const, independent
    object, and method/free-function cases plus two verified compile failures;
- lecture 05 is connected to common `main.tex` exactly once after lecture 04;
- C++17 builds, warning flags, behavioral equivalence, experiment runs, CTest,
  real negative diagnostics, and the common PDF pass.

Block 06:
`06_constructors_destructors_and_copy`

Known state:
- lecture 06 substantively covers construction, member initializer lists and
  initialization order, invariants, destruction and lifetime, copy
  construction versus copy assignment, shallow/deep copy, Rule of Three,
  Rule of Zero, ownership, copy elision, and an accurate copy cost model;
- exactly two experiments exist:
  - `copy_semantics_lab`, with plain and vector value copies, lifetime tracing,
    a real reorder warning, an isolated ASan/UBSan shallow-copy double-free,
    a sanitizer-clean Rule-of-Three repair, and a Rule-of-Zero equivalent;
  - `inventory_snapshot`, which copy-constructs and copy-assigns real
    `CourseBlock` data and proves independent mutable analysis snapshots without
    adding unnecessary custom special members to the domain type;
- lecture 06 is connected to common `main.tex` exactly once after lecture 05;
- C++17 builds, warning flags, runs, CTest, real sanitizer diagnostics,
  sanitizer-clean safe cases, and the common PDF pass.

Block 07:
`07_inheritance_and_polymorphism`

Known state:
- lecture 07 substantively covers is-a versus has-a, base/derived access,
  static and dynamic types, overriding, virtual and non-virtual dispatch,
  slicing, abstract classes, virtual destruction, hierarchy lifetime,
  composition tradeoffs, and an implementation-aware cost model;
- exactly two experiments exist:
  - `polymorphism_failure_lab`, with safe dispatch/slicing/abstract/destructor
    traces, three verified compile failures, and an isolated ASan-diagnosed
    non-virtual-destructor deletion case;
  - `inheritance_vs_composition_project_lab`, which audits the real block 07
    through both designs and verifies equivalent observable results;
- lecture 07 is connected to common `main.tex` exactly once after lecture 06;
- C++17 builds, warning flags, runs, CTest, expected diagnostics, sanitizer
  evidence, and the common PDF pass.

Block 08:
`08_design_solid_and_patterns`

Known state:
- lecture 08 presents SOLID as practical guidelines and derives Strategy,
  canonical Factory Method, and Observer from concrete design pressures;
- exactly two experiments exist:
  - `patterns_lab`, with direct baselines and verified Strategy, Factory
    Method, Observer, and simple-design comparison cases;
  - `course_audit_patterns`, a read-only 15-block audit using replaceable
    strategies and three non-owning observers without an artificial Factory
    Method hierarchy;
- lecture 08 is connected to common `main.tex` exactly once after lecture 07;
- C++17 builds, warning flags, experiment runs, CTest, extension checks, and
  the common PDF pass.

Block 09:
`09_templates`

Known state:
- lecture 09 derives generic programming from type duplication and covers
  function/class templates, deduction, instantiation, C++17 requirements,
  non-type parameters, specialization, header visibility, diagnostics, and
  an accurate compile-time/runtime polymorphism cost comparison;
- exactly two experiments exist:
  - `mini_generic_library`, a reusable header with multi-type utilities,
    `FixedBuffer<T, N>`, a focused full specialization, and a verified unmet
    comparison requirement;
  - `generic_course_audit`, a read-only 15-block audit whose one generic
    workflow works with two unrelated reporter types and verifies an invalid
    reporter at compile time;
- lecture 09 is connected to common `main.tex` exactly once after lecture 08;
- C++17 builds, warning flags, library tests, real negative diagnostics,
  audit runs, CTest, integration checks, and the common PDF pass.

Block 10:
`10_exceptions_and_raii`

Known state:
- lecture 10 compares explicit status and exception API contracts, then covers
  throw/try/catch, matching, rethrow, stack unwinding, RAII, constructor and
  destructor failure rules, exception-safety guarantees, transactional update,
  cost, and a deliberate top-level error boundary;
- exactly two experiments exist:
  - `error_model_lab`, which runs the same three-level file transformation with
    status and exception transport and verifies unwinding trace, rethrow, and
    temporary-report cleanup on normal, return, and exception paths;
  - `course_audit_error_boundary`, which audits all 15 blocks with shared domain
    logic, treats incomplete future blocks as data, compares status/exception
    transport for synthetic I/O failure, and chooses a hybrid CLI policy;
- lecture 10 is connected to common `main.tex` exactly once after lecture 09;
- C++17 warning builds, 15 deterministic tests, error-model/RAII cleanup checks,
  audit boundary cases, full PDF integration, references, labels, and diff
  validation pass.

Block 11:
`11_smart_pointers`

Known state:
- lecture 11 builds ownership-aware APIs from the distinction between object
  access and ownership, then covers `std::unique_ptr`, `std::shared_ptr`,
  `std::weak_ptr`, borrowing through references/raw pointers, factory ownership,
  shared cycles, `weak_ptr::lock`, exception safety, and an implementation-aware
  cost model;
- exactly two experiments exist:
  - `ownership_factory_lab`, which upgrades the earlier Factory Method example
    to return `std::unique_ptr<Product>`, verifies polymorphic destruction,
    unique ownership transfer, borrowing APIs, reset, and a real compile failure
    for attempted `unique_ptr` copying;
  - `shared_weak_graph_lab`, which verifies real shared lifetime, demonstrates
    a strong-reference cycle through destructor counts, breaks the ownership
    cycle with a weak back-reference, and checks `weak_ptr::lock` before and
    after expiration;
- lecture 11 is connected to common `main.tex` exactly once after lecture 10;
- C++17 warning builds, CTest, real negative diagnostics, ownership/destructor
  traces, full PDF integration, references, labels, and diff validation pass.

Block 12:
`12_move_semantics_and_casts`

Known state:
- lecture 12 connects the ownership model from block 11 with value categories
  and move semantics, covering lvalue/xvalue/prvalue, rvalue references,
  `std::move`, move construction/assignment, moved-from contracts, Rule of
  Five/Zero, `noexcept`, C++17 copy elision, forwarding references,
  reference collapsing, and `std::forward`;
- the same lecture treats explicit conversions as type-system contracts and
  distinguishes `static_cast`, `dynamic_cast`, `const_cast`,
  `reinterpret_cast`, C-style casts, runtime checks, const-correctness,
  low-level representation concerns, and UB boundaries;
- exactly two experiments exist:
  - `move_ownership_pipeline`, a move-only `Packet` with unique ownership,
    real move construction/assignment traces, value-category overload checks,
    forwarding, C++17 copy-elision observation, two compile-failure cases, and
    a sanitizer-clean run;
  - `casts_lab`, with static/dynamic/const/reinterpret cast cases, successful
    and failed polymorphic downcasts, a safe const-cast boundary, pointer/integer
    round trip, two compile-failure cases, a compile-only C-style const-removal
    example, and a sanitizer-clean run;
- lecture 12 is connected to common `main.tex` exactly once after lecture 11;
- C++17 CMake/CTest, `-Werror`, real negative diagnostics, ASan/UBSan,
  full PDF integration, references, labels, and diff validation pass.

Block 13:
`13_type_deduction_lambdas_and_function`

Known state:
- lecture 13 develops type deduction and callable APIs in one line, covering
  `auto`, reference/const deduction, `decltype`, `decltype(auto)`, trailing
  return types, structured bindings, CTAD, callable objects, lambda closure
  types, value/reference/default/init captures, mutable and generic lambdas;
- the lecture then distinguishes template callables from runtime type erasure
  through `std::function`, including empty wrappers, `std::bad_function_call`,
  copyability constraints in C++17, capture lifetime, and the cost model;
- `std::optional<T>` is used as an explicit found/not-found value contract,
  with `std::nullopt`, access rules, `std::bad_optional_access`, ownership/view
  distinctions, and limits of optional as an error model;
- exactly two experiments exist:
  - `course_query_pipeline`, which verifies real deduced types with
    `static_assert`, `decltype`, CTAD, structured bindings, generic/value/reference
    lambdas, a template predicate query, `std::optional<CourseBlock>`, one real
    missing-capture compile failure, and a sanitizer-clean run;
  - `callback_registry_lab`, which stores heterogeneous free-function,
    function-object and lambda callbacks through `std::function`, checks mutable
    capture state, weak lifetime observation, empty-wrapper exception behavior,
    a template-callable path, one real move-only-target compile failure, and a
    sanitizer-clean run;
- lecture 13 is connected to common `main.tex` exactly once after lecture 12;
- C++17 CMake/CTest, `-Werror`, negative diagnostics, ASan/UBSan, full PDF
  integration, references, labels, and diff validation pass.

Block 14:
`14_stl_containers_iterators_algorithms`

Known state:
- lecture 14 presents STL as contracts between containers, iterators/ranges,
  algorithms and callable policies, covering `std::array`, `std::vector`,
  `std::deque`, `std::list`, ordered/unordered associative containers,
  adapters, iterator categories, half-open ranges, const iterators, complexity,
  cache-locality models and invalidation;
- the algorithm layer covers `find`/`find_if`, `count_if`, `copy_if`,
  `transform`, `accumulate`, `sort`, `stable_sort`, `lower_bound`,
  all/any/none predicates, output iterators, member-vs-generic algorithms,
  erase-remove in C++17, reserve/capacity and structural mutation contracts;
- exactly two experiments exist:
  - `course_catalog_pipeline`, which uses a vector source of truth plus an
    unordered index and ordered summary, and verifies lookup, filtering,
    sorting, transformation, aggregation, predicates and optional not-found
    behavior in a real 15-block catalog;
  - `invalidation_and_erase_lab`, which verifies reserve/capacity behavior,
    forced vector reallocation without UB in the safe path, erase-remove,
    erase return iterators, list stability, unordered rehash/reference behavior,
    a real expected ASan use-after-free diagnostic for a stale vector pointer,
    and a real compile failure for `std::sort` over list iterators;
- lecture 14 is connected to common `main.tex` exactly once after lecture 13;
- C++17 CMake/CTest, `-Werror`, negative diagnostics, ASan/UBSan including the
  expected invalidation failure, full PDF integration, references, labels and
  diff validation pass.

Block 15:
`15_modern_cpp_and_next_steps`

Known state:
- lecture 15 keeps C++17 as the base course standard and explicitly marks the
  final practice as C++20, covering `constexpr`, `consteval`, `constinit`,
  feature/toolchain checks, Concepts, `requires`, constrained overloads,
  ranges algorithms/views, projections, `std::span`, designated initializers,
  defaulted `<=>`, `std::erase_if`, `starts_with`, and modern-library evolution;
- coroutines, modules, allocators/`std::pmr`, `std::jthread`, source-location,
  C++23 directions and further study are presented as scoped next-step topics
  with their real lifetime, build-tooling and portability boundaries rather
  than as toy claims of full mastery;
- exactly two C++20 experiments exist and pass CTest, negative compile checks,
  `-Werror` and sanitizer-clean runs;
- lecture 15 is connected to common `main.tex` exactly once after lecture 14;
- full PDF integration, references, labels and diff validation pass.

## Course completion

`ALL_15_PLANNED_BLOCKS_COMPLETE`

Final direct audit:
- 15/15 lecture files are substantive and connected to `main.tex`;
- 15/15 matching example directories satisfy the strict block bijection;
- 30 practical experiment directories exist, exactly two per block;
- all 29 CMake-based experiment projects configure, build and pass CTest;
- historical intermediate-course audit expectations were updated to the
  completed syllabus:
  - block 08 strict audit now expects all 15 substantive blocks;
  - block 09 generic audit now expects both reporters to pass all 15 blocks;
  - block 10's old `--expect-incomplete` CTest is now an explicit expected
    failure on the completed course;
- block 15 additionally passes explicit C++20, negative-compile, `-Werror` and
  ASan/UBSan checks;
- the shared XeLaTeX PDF builds cleanly with no overfull boxes, unresolved
  references, duplicate labels or rerun warnings;
- the base 15-block course is complete. Any future material should be planned
  as a separate extension rather than silently changing the completed syllabus.

## Next block

None. The planned 15-block base course is complete.

Any future extension must start with a new explicit scope and plan; it is not a
continuation of the completed base syllabus by default.

## Post-course compaction and experiment menu

- all 15 lecture files received a compact editorial pass after the base course
  was completed;
- all `itemize`, `enumerate`, and `description` environments in the lectures
  were rewritten as shorter neutral prose while preserving labels, code
  environments, section structure, technical claims, and experiment links;
- the selected verbose/style-candidate phrases from the course-wide audit were
  rewritten in shorter neutral form;
- `course_menu.sh` is the single interactive terminal launcher for all 30
  experiments: block/experiment selection, a short explanation before launch,
  explicit `y/n` confirmation, direct same-terminal execution, and a
  code/features browser are available;
- this is a post-course refinement. `NEXT_BLOCK` remains none and the completed
  15-block syllabus is unchanged.

## Global formatting status

The table-of-contents number-width issue has been visually checked after the
robust fix and is confirmed resolved.

TOC_VISUAL_CHECK=PASS

## Git

Codex does not commit or push by default.
The user/coordinating chat closes Git explicitly after reviewing a block.
