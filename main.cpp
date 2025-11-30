#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <limits>

// Базовый класс для математических операций
class MathFunction {
protected:
    double x;
    double result;

public:
    // Конструктор
    MathFunction(double input_x) : x(input_x), result(0.0) {}
    
    // Виртуальная функция для вычисления - позволяет переопределять в дочерних классах
    virtual double calculate() = 0;
    
    // Геттеры для доступа к данным
    double getX() const { return x; }
    double getResult() const { return result; }
    
    // Виртуальный деструктор для правильного удаления объектов
    virtual ~MathFunction() {}
};

// Дочерний класс для вычисления тригонометрических функций
class TrigFunction : public MathFunction {
public:
    TrigFunction(double input_x) : MathFunction(input_x) {}
    
    // Реализация виртуальной функции из базового класса
    double calculate() override {
        // Проверяем, что tan(x) и cot(x) не равны нулю и не создают отрицательное значение под корнем
        double tan_x = tan(x);
        double cot_x = 1.0 / tan(x);  // cot(x) = 1/tan(x)
        
        // Проверка на корректность вычислений
        if (tan_x == 0 || std::isnan(tan_x) || std::isnan(cot_x)) {
            std::cout << "Error: Invalid input for trigonometric functions!" << std::endl;
            return std::numeric_limits<double>::quiet_NaN();
        }
        
        double product = cot_x * tan_x;
        
        // Проверяем, что произведение положительно для извлечения корня
        if (product < 0) {
            std::cout << "Error: Cannot calculate square root of negative number!" << std::endl;
            return std::numeric_limits<double>::quiet_NaN();
        }
        
        result = sqrt(product);
        return result;
    }
    
    // Метод для получения значения cot(x) отдельно
    double getCot() const {
        if (tan(x) == 0) return std::numeric_limits<double>::quiet_NaN();
        return 1.0 / tan(x);
    }
    
    // Метод для получения значения tan(x) отдельно
    double getTan() const {
        return tan(x);
    }
};

// Класс для работы с файлами - второй иерархический класс
class FileHandler {
private:
    std::string filename;

public:
    FileHandler(const std::string& fname) : filename(fname) {}
    
    // Метод для записи результата в файл
    void writeToFile(double x, double result, double tan_x, double cot_x) {
        std::ofstream file(filename, std::ios::app); // Открываем файл для добавления
        if (file.is_open()) {
            file << "Input x: " << x << std::endl;
            file << "tan(x): " << tan_x << std::endl;
            file << "cot(x): " << cot_x << std::endl;
            file << "Result y(x) = sqrt(cot(x) * tan(x)): " << result << std::endl;
            file << "------------------------" << std::endl;
            file.close();
            std::cout << "Result saved to file: " << filename << std::endl;
        } else {
            std::cout << "Error: Could not open file " << filename << " for writing!" << std::endl;
        }
    }
    
    // Метод для очистки файла
    void clearFile() {
        std::ofstream file(filename, std::ios::trunc); // Очищаем содержимое файла
        file.close();
    }
};

// Вспомогательный класс для вывода информации в консоль
class ConsoleOutput {
public:
    static void printHeader() {
        std::cout << "=========================================" << std::endl;
        std::cout << "Mathematical Function Calculator" << std::endl;
        std::cout << "Formula: y(x) = sqrt(cot(x) * tan(x))" << std::endl;
        std::cout << "=========================================" << std::endl;
    }
    
    static void printResult(double x, double result, double tan_x, double cot_x) {
        std::cout << "\nCalculation Results:" << std::endl;
        std::cout << "Input x: " << x << std::endl;
        std::cout << "tan(x): " << tan_x << std::endl;
        std::cout << "cot(x): " << cot_x << std::endl;
        std::cout << "y(x) = sqrt(cot(x) * tan(x)): " << result << std::endl;
        std::cout << std::endl;
    }
    
    static void printError() {
        std::cout << "Error occurred during calculation!" << std::endl;
    }
};

int main() {
    // Очищаем консоль и выводим заголовок
    ConsoleOutput::printHeader();
    
    // Создаем объект для работы с файлом
    FileHandler fileHandler("results.txt");
    fileHandler.clearFile(); // Очищаем файл перед новыми записями
    
    // Основной цикл программы
    char continueChoice;
    do {
        double x;
        std::cout << "Enter value of x (in radians): ";
        std::cin >> x;
        
        // Создаем объект дочернего класса TrigFunction
        TrigFunction func(x);
        
        // Вычисляем результат
        double result = func.calculate();
        
        // Выводим результат в консоль
        if (!std::isnan(result)) {
            ConsoleOutput::printResult(func.getX(), result, func.getTan(), func.getCot());
            
            // Записываем результат в файл
            fileHandler.writeToFile(func.getX(), result, func.getTan(), func.getCot());
        } else {
            ConsoleOutput::printError();
        }
        
        std::cout << "Do you want to calculate another value? (y/n): ";
        std::cin >> continueChoice;
        
    } while (continueChoice == 'y' || continueChoice == 'Y');
    
    std::cout << "Program finished. Results saved to results.txt" << std::endl;
    
    return 0;
}
