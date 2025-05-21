#include <iostream>
using namespace std;
class Integer {
private:
  int value;

public:
  Integer(int value) : value(value) {}
  int sum(int b) { return value + b; }
  int sub(int b) { return value - b; }
  int getValue() { return value; }
  int setValue(int value) {
    this->value = value;
    return this->value;
  }
};
int main() {
  Integer a(5);
  cout << a.sum(3) << endl;       // 8
  cout << a.sub(2) << endl;       // 3
  cout << a.getValue() << endl;   // 5
  cout << a.setValue(10) << endl; // 10
  cout << "Rốt đẹp trai" << endl;
  return 0;
}
