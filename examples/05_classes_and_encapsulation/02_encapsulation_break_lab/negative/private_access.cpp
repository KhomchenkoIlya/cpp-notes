#include <string>

class CourseProgress {
public:
    int number() const { return number_; }

private:
    int number_{5};
};

int main() {
    CourseProgress progress;
    progress.number_ = 99; // Expected error: caller cannot name a private member.
    return progress.number();
}
