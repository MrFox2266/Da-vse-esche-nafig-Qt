#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

class MathFunction {
public:
    virtual double calculate(double x) = 0;
    virtual ~MathFunction() {}
};

class TrigFunction : public MathFunction {
public:
    double calculate(double x) override {
        if (abs(sin(x)) < 0.001 || abs(cos(x)) < 0.001) {
            return NAN;
        }

        double result = sqrt((cos(x) / sin(x)) * (sin(x) / cos(x)));

        return result;
    }
};

class OutputHandler {
private:
    ofstream file;

public:
    OutputHandler(const string& filename) {
        file.open(filename);
        if (!file.is_open()) {
            cout << "Error: could not open file " << filename << endl;
        }
    }

    ~OutputHandler() {
        if (file.is_open()) {
            file.close();
        }
    }

    bool isReady() {
        return file.is_open();
    }

    void printTable(double a, double b, double h, MathFunction& func) {
        cout << fixed << setprecision(4);
        cout << "x\t\ty(x)\n";
        cout << "------------------------\n";

        if (file.is_open()) {
            file << fixed << setprecision(4);
            file << "x\t\ty(x)\n";
            file << "------------------------\n";
        }

        int i = 0;
        double x = a;
        for (; x <= b; i++, x = a + i * h) {
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
    double a, b, h;

    cout << "Enter interval start a: ";
    cin >> a;
    cout << "Enter interval end b: ";
    cin >> b;
    cout << "Enter step h: ";
    cin >> h;

    if (h <= 0) {
        cout << "Error: step must be positive!" << endl;
        return 1;
    }

    if (a > b) {
        cout << "Error: start must be less than end!" << endl;
        return 1;
    }

    TrigFunction trigFunc;
    OutputHandler output("result.txt");

    if (!output.isReady()) {
        return 1;
    }

    output.printTable(a, b, h, trigFunc);

    cout << "\nResults saved to file result.txt" << endl;

    cout << "Press Enter to close...";
    cin.ignore();
    cin.get();

    return 0;
}
