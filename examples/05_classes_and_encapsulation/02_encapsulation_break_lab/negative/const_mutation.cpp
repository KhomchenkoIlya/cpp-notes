class CourseProgress {
public:
    void mark_complete() { complete_ = true; }
    bool is_complete() const { return complete_; }

private:
    bool complete_{};
};

int main() {
    const CourseProgress progress;
    progress.mark_complete(); // Expected error: non-const method through const object.
    return progress.is_complete() ? 0 : 1;
}
