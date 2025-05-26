#include <iostream>
using namespace std;
int main() {
  int quantity = 10;
  int weight = 20;
  int positionX = 5;
  int positionY = 10;
  int armyBelong = 0;             // 0 for LiberationArmy, 1 for ARVN
  string infantryType = "SNIPER"; // Example infantry type
  string vehicleType = "TANK";    // Example vehicle type
  // Example of creating an Infantry object
  cout << "Creating Infantry with quantity: " << quantity
       << ", weight: " << weight << ", position: (" << positionX << ", "
       << positionY << ")"
       << ", armyBelong: " << armyBelong << ", infantryType: " << infantryType
       << endl;

  return 0;
}
