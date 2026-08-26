#ifndef GENERIC_COURSE_AUDIT_HPP
#define GENERIC_COURSE_AUDIT_HPP

#include <cstddef>
#include <string>

struct CourseBlock {
    std::string name;
    bool lecture_exists{};
    bool examples_exist{};
    std::size_t lecture_lines{};
    std::size_t example_files{};
};

struct AuditResult {
    std::string block;
    bool passed{};
    std::string detail;
};

struct AuditSummary {
    std::size_t checked{};
    std::size_t passed{};
};

struct StrictPolicy {
    AuditResult evaluate(const CourseBlock& block) const {
        const bool pass = block.lecture_exists && block.examples_exist &&
                          block.lecture_lines > 0 && block.example_files > 0;
        return {block.name, pass,
                "lines=" + std::to_string(block.lecture_lines) +
                    ", files=" + std::to_string(block.example_files)};
    }
};

template <typename Blocks, typename Policy, typename Reporter>
AuditSummary run_audit(const Blocks& blocks, const Policy& policy, Reporter& reporter) {
    AuditSummary summary;
    for (const auto& block : blocks) {
        const AuditResult result = policy.evaluate(block);
        ++summary.checked;
        if (result.passed) {
            ++summary.passed;
        }
        reporter.on_result(result);
    }
    reporter.on_finish(summary);
    return summary;
}

#endif
