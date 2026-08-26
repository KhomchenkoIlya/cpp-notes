#include "audit.hpp"

#include <vector>

struct BadReporter {
    void print(const AuditResult&) {}
};

int main() {
    const std::vector<CourseBlock> blocks{{"09_templates", true, true, 1, 1}};
    const StrictPolicy policy;
    BadReporter reporter;
    (void)run_audit(blocks, policy, reporter);  // Missing on_result and on_finish.
}
