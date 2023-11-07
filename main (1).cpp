#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Excursion {
    std::string name;
    std::string destination;
    double cost;
};

// Функция для добавления новой экскурсии в файл
void addExcursionToFile(const Excursion& excursion) {
    std::ofstream file("excursions.txt", std::ios::app);
    if (file.is_open()) {
        file << excursion.name << "," << excursion.destination << "," << excursion.cost << "\n";
        file.close();
        std::cout << "Экскурсия добавлена в файл.\n";
    } else {
        std::cerr << "Ошибка открытия файла.\n";
    }
}

// Функция для поиска экскурсии по названию в файле
void searchExcursionByName(const std::string& name) {
    std::ifstream file("excursions.txt");
    if (file.is_open()) {
        std::string line;
        bool found = false;
        while (std::getline(file, line)) {
            std::string excursionName = line.substr(0, line.find(','));
            if (excursionName == name) {
                std::cout << "Найденная экскурсия: " << line << "\n";
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "Экскурсия с названием '" << name << "' не найдена.\n";
        }
        file.close();
    } else {
        std::cerr << "Ошибка открытия файла.\n";
    }
}

// Функция для сортировки экскурсий по месту посещения или стоимости
void sortExcursions(const std::string& criteria) {
    std::ifstream file("excursions.txt");
    if (file.is_open()) {
        std::vector<Excursion> excursions;
        std::string line;
        while (std::getline(file, line)) {
            std::string name, destination;
            double cost;
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            name = line.substr(0, pos1);
            destination = line.substr(pos1 + 1, pos2 - pos1 - 1);
            cost = std::stod(line.substr(pos2 + 1));
            excursions.push_back({name, destination, cost});
        }
        file.close();

        if (criteria == "place") {
            std::sort(excursions.begin(), excursions.end(), [](const Excursion& a, const Excursion& b) {
                return a.destination < b.destination;
            });
        } else if (criteria == "cost") {
            std::sort(excursions.begin(), excursions.end(), [](const Excursion& a, const Excursion& b) {
                return a.cost < b.cost;
            });
        } else {
            std::cerr << "Некорректный критерий сортировки.\n";
            return;
        }

        std::ofstream outFile("excursions.txt");
        if (outFile.is_open()) {
            for (const auto& excursion : excursions) {
                outFile << excursion.name << "," << excursion.destination << "," << excursion.cost << "\n";
            }
            outFile.close();
            std::cout << "Экскурсии отсортированы по критерию: " << criteria << "\n";
        } else {
            std::cerr << "Ошибка открытия файла для записи.\n";
        }
    } else {
        std::cerr << "Ошибка открытия файла.\n";
    }
}

// Функция для вывода экскурсий с стоимостью не превышающей заданное значение
void filterExcursionsByCost(double maxCost) {
    std::ifstream file("excursions.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.rfind(',');
            double cost = std::stod(line.substr(pos + 1));
            if (cost <= maxCost) {
                std::cout << line << "\n";
            }
        }
        file.close();
    } else {
        std::cerr << "Ошибка открытия файла.\n";
    }
}

int main() {
    std::string choice;
    do {
        std::cout << "1. Добавить экскурсию\n";
        std::cout << "2. Поиск экскурсии по названию\n";
        std::cout << "3. Сортировка экскурсий\n";
        std::cout << "4. Вывод экскурсий по стоимости\n";
        std::cout << "5. Выйти\n";
        std::cout << "Выберите действие: ";
        std::cin >> choice;

        if (choice == "1") {
            Excursion newExcursion;
            std::cin.ignore(); // Clear the input buffer
            std::cout << "Введите название экскурсии: ";
            std::getline(std::cin, newExcursion.name);
            if (newExcursion.name.empty()) {
                std::cout << "Название экскурсии не может быть пустым.\n";
                continue;
            }
            std::cout << "Введите место посещения: ";
            std::getline(std::cin, newExcursion.destination);
            std::cout << "Введите стоимость экскурсии: ";
            std::cin >> newExcursion.cost;
            addExcursionToFile(newExcursion);
        } else if (choice == "2") {
            std::cin.ignore(); // Clear the input buffer
            std::string searchName;
            std::cout << "Введите название экскурсии для поиска: ";
            std::getline(std::cin, searchName);
            searchExcursionByName(searchName);
        } else if (choice == "3") {
            std::cout << "Выберите критерий сортировки (place/cost): ";
            std::string criteria;
            std::cin >> criteria;
            sortExcursions(criteria);
        } else if (choice == "4") {
            double maxCost;
            std::cout << "Введите максимальную стоимость экскурсии: ";
            std::cin >> maxCost;
            filterExcursionsByCost(maxCost);
        } else if (choice != "5") {
            std::cout << "Некорректный ввод. Попробуйте еще раз.\n";
        }

    } while (choice != "5");

    return 0;
}