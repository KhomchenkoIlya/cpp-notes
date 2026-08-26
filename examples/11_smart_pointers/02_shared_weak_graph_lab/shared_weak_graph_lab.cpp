#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

int destroyed_sessions = 0;
int destroyed_shared_nodes = 0;
int destroyed_tree_nodes = 0;

class AuditSession {
public:
    explicit AuditSession(std::string name) : name_{std::move(name)} {}

    ~AuditSession() {
        ++destroyed_sessions;
        std::cout << "destroy session " << name_ << '\n';
    }

    const std::string& name() const {
        return name_;
    }

private:
    std::string name_;
};

class ConsoleView {
public:
    explicit ConsoleView(std::shared_ptr<AuditSession> session)
        : session_{std::move(session)} {}

    std::string render() const {
        return "console:" + session_->name();
    }

private:
    std::shared_ptr<AuditSession> session_;
};

class StatisticsView {
public:
    explicit StatisticsView(std::shared_ptr<AuditSession> session)
        : session_{std::move(session)} {}

    std::string render() const {
        return "stats:" + session_->name();
    }

private:
    std::shared_ptr<AuditSession> session_;
};

struct SharedNode {
    explicit SharedNode(std::string value) : value{std::move(value)} {}

    ~SharedNode() {
        ++destroyed_shared_nodes;
        std::cout << "destroy shared node " << value << '\n';
    }

    std::string value;
    std::shared_ptr<SharedNode> peer;
};

class TreeNode : public std::enable_shared_from_this<TreeNode> {
public:
    explicit TreeNode(std::string value) : value_{std::move(value)} {}

    ~TreeNode() {
        ++destroyed_tree_nodes;
        std::cout << "destroy tree node " << value_ << '\n';
    }

    void add_child(const std::shared_ptr<TreeNode>& child) {
        children_.push_back(child);
        child->parent_ = shared_from_this();
    }

    std::shared_ptr<TreeNode> parent() const {
        return parent_.lock();
    }

    const std::string& value() const {
        return value_;
    }

    std::size_t child_count() const {
        return children_.size();
    }

private:
    std::string value_;
    std::vector<std::shared_ptr<TreeNode>> children_;
    std::weak_ptr<TreeNode> parent_;
};

bool shared_lifetime_case() {
    destroyed_sessions = 0;

    std::shared_ptr<AuditSession> root = std::make_shared<AuditSession>("block-11");
    const std::weak_ptr<AuditSession> observer = root;

    bool alive_after_root_reset = false;
    bool expired_after_views = false;

    {
        ConsoleView console{root};
        StatisticsView stats{root};

        const bool outputs =
            console.render() == "console:block-11" &&
            stats.render() == "stats:block-11";

        root.reset();
        alive_after_root_reset =
            !observer.expired() &&
            observer.lock() != nullptr &&
            destroyed_sessions == 0;

        if (!outputs) {
            std::cout << "SHARED_LIFETIME=FAIL\n";
            return false;
        }
    }

    expired_after_views =
        observer.expired() &&
        observer.lock() == nullptr &&
        destroyed_sessions == 1;

    const bool pass = alive_after_root_reset && expired_after_views;

    std::cout
        << "alive_after_root_reset=" << (alive_after_root_reset ? "yes" : "no") << '\n'
        << "expired_after_views=" << (expired_after_views ? "yes" : "no") << '\n'
        << "destroyed_sessions=" << destroyed_sessions << '\n'
        << "SHARED_LIFETIME=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool strong_cycle_case() {
    destroyed_shared_nodes = 0;

    {
        auto left = std::make_shared<SharedNode>("left");
        auto right = std::make_shared<SharedNode>("right");

        left->peer = right;
        right->peer = left;

        std::cout
            << "left_use_count=" << left.use_count() << '\n'
            << "right_use_count=" << right.use_count() << '\n';
    }

    const bool pass = destroyed_shared_nodes == 0;

    std::cout
        << "CYCLE_DESTROYED=" << destroyed_shared_nodes << '\n'
        << "STRONG_CYCLE=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool weak_graph_case() {
    destroyed_tree_nodes = 0;

    std::weak_ptr<TreeNode> observed_child;

    {
        const auto root = std::make_shared<TreeNode>("root");
        const auto child = std::make_shared<TreeNode>("child");

        root->add_child(child);
        observed_child = child;

        const std::shared_ptr<TreeNode> parent = child->parent();

        const bool graph_ok =
            root->child_count() == 1 &&
            parent != nullptr &&
            parent->value() == "root" &&
            !observed_child.expired();

        if (!graph_ok) {
            std::cout << "WEAK_GRAPH=FAIL\n";
            return false;
        }
    }

    const bool pass =
        observed_child.expired() &&
        observed_child.lock() == nullptr &&
        destroyed_tree_nodes == 2;

    std::cout
        << "weak_expired=" << (observed_child.expired() ? "yes" : "no") << '\n'
        << "destroyed_tree_nodes=" << destroyed_tree_nodes << '\n'
        << "WEAK_GRAPH=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

bool lock_case() {
    std::weak_ptr<AuditSession> observer;

    bool first_lock = false;
    bool second_lock_empty = false;

    {
        const auto owner = std::make_shared<AuditSession>("lock-case");
        observer = owner;

        const auto alive = observer.lock();
        first_lock = alive != nullptr && alive->name() == "lock-case";
    }

    second_lock_empty = observer.lock() == nullptr;

    const bool pass = first_lock && second_lock_empty;

    std::cout
        << "lock_while_alive=" << (first_lock ? "yes" : "no") << '\n'
        << "lock_after_expire_empty=" << (second_lock_empty ? "yes" : "no") << '\n'
        << "WEAK_LOCK=" << (pass ? "PASS" : "FAIL") << '\n';

    return pass;
}

} // namespace

int main(int argc, char* argv[]) {
    std::string selected{"all"};

    if (argc == 3 && std::string{argv[1]} == "--case") {
        selected = argv[2];
    } else if (argc != 1) {
        std::cerr
            << "Usage: shared_weak_graph_lab "
            << "[--case shared|cycle|weak|lock]\n";
        return 2;
    }

    bool pass = true;

    if (selected == "all" || selected == "shared") {
        pass = shared_lifetime_case() && pass;
    }
    if (selected == "all" || selected == "cycle") {
        pass = strong_cycle_case() && pass;
    }
    if (selected == "all" || selected == "weak") {
        pass = weak_graph_case() && pass;
    }
    if (selected == "all" || selected == "lock") {
        pass = lock_case() && pass;
    }

    if (selected != "all" &&
        selected != "shared" &&
        selected != "cycle" &&
        selected != "weak" &&
        selected != "lock") {
        std::cerr << "Unknown case: " << selected << '\n';
        return 2;
    }

    std::cout << "SHARED_WEAK_GRAPH_LAB=" << (pass ? "PASS" : "FAIL") << '\n';
    return pass ? 0 : 1;
}
