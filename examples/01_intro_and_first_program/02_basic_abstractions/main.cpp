#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    // string хранит строку как готовую абстракцию.
    std::string station{"mipt-lab"};

    // vector объединяет однотипные значения в одну последовательность.
    std::vector<double> measurements{21.5, 22.0, 20.5};

    // Циклы ещё не изучены, поэтому три известных элемента складываем явно.
    double sum = measurements[0] + measurements[1] + measurements[2];

    // sum имеет тип double, поэтому размер vector участвует
    // в floating-point делении после неявного преобразования.
    double average = sum / measurements.size();

    // map хранит соответствие "строковый ключ -> целый счётчик".
    std::map<std::string, int> status_count;

    // Для отсутствующего ключа operator[] создаёт int со значением 0.
    status_count["ok"] += 1;
    status_count["ok"] += 1;
    status_count["warning"] += 1;

    std::cout << "Station: " << station << '\n';
    std::cout << "Average: " << average << '\n';
    std::cout << "ok: " << status_count["ok"] << '\n';
    std::cout << "warning: " << status_count["warning"] << '\n';

    return 0;
}
