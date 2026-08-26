class Reordered {
public:
    Reordered() : second_{2}, first_{1} {}

private:
    int first_;
    int second_;
};

int main() {
    Reordered value;
    (void)value;
}
