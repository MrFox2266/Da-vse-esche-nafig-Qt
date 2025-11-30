#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

// Базовый класс для математических функций
class MathFunction {
protected:
    double x; // переменная x
    
public:
    // Конструктор
    MathFunction(double val = 0) : x(val) {}
    
    // Виртуальная функция для вычисления значения
    virtual double calculate() = 0;
    
    // Установка значения x
    void setX(double val) {
        x = val;
    }
    
    // Виртуальный деструктор
    virtual ~MathFunction() {}
};

// Класс для тригонометрической функции y(x) = sqrt(cot(x)*tan(x))
class TrigFunction : public MathFunction {
public:
    // Конструктор
    TrigFunction(double val = 0) : MathFunction(val) {}
    
    // Реализация вычисления функции
    double calculate() override {
        // Проверяем, не равны ли sin(x) или cos(x) нулю (с небольшим запасом)
        if (abs(sin(x)) < 0.001 || abs(cos(x)) < 0.001) {
            return -1; // специальное значение для неопределенности
        }
        
        double cot = cos(x) / sin(x); // котангенс
        double tan = sin(x) / cos(x); // тангенс
        return sqrt(cot * tan); // вычисляем sqrt(cot*tan)
    }
};

int main() {
    double a, b, h; // начало, конец интервала и шаг
    
    // Ввод параметров с консоли
    cout << "Enter interval start a: ";
    cin >> a;
    cout << "Enter interval end b: ";
    cin >> b;
    cout << "Enter step h: ";
    cin >> h;
    
    // Проверка корректности введенных данных
    if (h <= 0) {
        cout << "Error: step must be positive!" << endl;
        return 1;
    }
    
    if (a > b) {
        cout << "Error: start must be less than end!" << endl;
        return 1;
    }
    
    // Открываем файл для записи результата
    ofstream file("result.txt");
    
    if (!file.is_open()) {
        cout << "Error: failed to create file!" << endl;
        return 1;
    }
    
    // Устанавливаем формат вывода с 4 знаками после запятой
    cout << fixed << setprecision(4);
    file << fixed << setprecision(4);
    
    // Заголовок таблицы в консоли
    cout << "x\t\ty(x)\n";
    file << "x\t\ty(x)\n"; // заголовок в файл
    cout << "------------------------\n";
    file << "------------------------\n"; // разделитель в файле
    
    // Создаем объект функции
    TrigFunction func;
    
    // Проходим по интервалу с заданным шагом
    for (double x = a; x <= b; x += h) {
        func.setX(x); // устанавливаем текущее значение x
        double result = func.calculate(); // вычисляем значение функции
        
        if (result == -1) { // если функция не определена
            cout << x << "\tundefined\n";
            file << x << "\tundefined\n";
        } else {
            cout << x << "\t" << result << "\n"; // выводим в консоль
            file << x << "\t" << result << "\n"; // записываем в файл
        }
    }
    
    file.close(); // закрываем файл
    
    cout << "Results saved to file result.txt" << endl;
    
    return 0;
}
