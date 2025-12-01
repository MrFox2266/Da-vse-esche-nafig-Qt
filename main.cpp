#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

// Абстрактный класс (реализует абстракцию)
class MathFunction {
public:
    // Чисто виртуальная функция - делает класс абстрактным
    virtual double calculate(double x) = 0;
    virtual ~MathFunction() {}
};

// Производный класс (реализует наследование и полиморфизм)
class TrigFunction : public MathFunction {
public:
    // Переопределяет виртуальную функцию базового класса
    double calculate(double x) override {
        // Проверяем, не равны ли sin(x) или cos(x) нулю (с небольшой погрешностью)
        if (abs(sin(x)) < 0.001 || abs(cos(x)) < 0.001) {
            return NAN; // Возвращаем специальное значение для неопределённости
        }

        // Вычисляем котангенс и тангенс
        double ctg = cos(x) / sin(x);
        double tg = sin(x) / cos(x);

        // Вычисляем y(x) = sqrt(cot(x)*tan(x))
        double result = sqrt(ctg * tg);

        return result;
    }
};

// Класс для обработки вывода
class OutputHandler {
private:
    ofstream file;

public:
    // Конструктор открывает файл
    OutputHandler(const string& filename) {
        file.open(filename);
    }

    // Деструктор закрывает файл
    ~OutputHandler() {
        if (file.is_open()) {
            file.close();
        }
    }

    // Метод для вывода таблицы значений
    void printTable(double a, double b, double h, MathFunction& func) {
        cout << fixed << setprecision(4);
        cout << "x\t\ty(x)\n";
        cout << "------------------------\n";

        // Выводим заголовок в файл, если он открыт
        if (file.is_open()) {
            file << fixed << setprecision(4);
            file << "x\t\ty(x)\n";
            file << "------------------------\n";
        }

        // Вычисляем и выводим значения функции на интервале
        for (double x = a; x <= b; x += h) {
            double result = func.calculate(x);

            if (isnan(result)) {
                cout << x << "\tundefined\n";
                if (file.is_open()) {
                    file << x << "\tundefined\n";
                }
            } else {
                cout << x << "\t" << result << "\n";
                if (file.is_open()) {
                    file << x << "\t" << result << "\n";
                }
            }
        }
    }
};

int main() {
    // Устанавливаем кодировку для корректного отображения русских символов
    system("chcp 65001 > nul");

    double a, b, h;

    // Вводим параметры интервала
    cout << "Enter interval start a: ";
    cin >> a;
    cout << "Enter interval end b: ";
    cin >> b;
    cout << "Enter step h: ";
    cin >> h;

    // Проверяем корректность введённых данных
    if (h <= 0) {
        cout << "Error: step must be positive!" << endl;
        return 1;
    }

    if (a > b) {
        cout << "Error: start must be less than end!" << endl;
        return 1;
    }

    // Создаём объекты классов
    TrigFunction trigFunc; // Объект для вычисления функции
    OutputHandler output("result.txt"); // Объект для вывода в файл

    // Выводим таблицу значений
    output.printTable(a, b, h, trigFunc);

    // Сообщаем пользователю о сохранении результатов
    cout << "\nResults saved to file result.txt" << endl;

    // Ждём нажатия клавиши перед завершением программы
    cout << "Press Enter to close...";
    cin.ignore();
    cin.get();

    return 0;
}
