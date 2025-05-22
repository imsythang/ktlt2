/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any
// other libraries.

#include "hcmcampaign.h"

using namespace std;

void g_satc_01() {
  cout << "----- Sample Testcase 02 -----" << endl;
  Vehicle tankOfLiber(20, 20, Position(1, 2), MORTAR);
  Vehicle truckOfLiber(15, 10, Position(2, 2), TRUCK);
  Infantry sniperOfLiber(10, 20, Position(1, 1), SNIPER);

  Vehicle tankOfAr(5, 20, Position(3, 2), MORTAR);
  Vehicle truckOfAr(1, 10, Position(3, 1), TRUCK);
  Infantry sniperOfAr(7, 20, Position(3, 3), SNIPER);

  Unit **unitArrayOfLiber = new Unit *[3];
  unitArrayOfLiber[0] = &tankOfLiber;
  unitArrayOfLiber[1] = &truckOfLiber;
  unitArrayOfLiber[2] = &sniperOfLiber;

  Unit **unitArrayOfAr = new Unit *[3];
  unitArrayOfAr[0] = &tankOfAr;
  unitArrayOfAr[1] = &truckOfAr;
  unitArrayOfAr[2] = &sniperOfAr;

  LiberationArmy *liberationArmy =
      new LiberationArmy(unitArrayOfLiber, 3, "LiberationArmy", 0);
  cout << liberationArmy->str() << endl;
  ARVN *arvn = new ARVN(unitArrayOfAr, 3, "ARVN", 0);
  cout << arvn->str() << endl;

  Army *enemyLiber = arvn;
  liberationArmy->fight(enemyLiber, false);
  Army *enemyAr = liberationArmy;
  arvn->fight(enemyAr, false);

  cout << "* After the fight" << endl;
  cout << liberationArmy->str() << endl;
  cout << arvn->str() << endl;

  delete liberationArmy;
  delete arvn;
  delete[] unitArrayOfLiber;
  delete[] unitArrayOfAr;
}
int main(int argc, const char *argv[]) {
  cout << "Expected: " << endl;
  cout << "----- Sample Testcase 02 -----" << endl;
  cout << "LiberationArmy[LF=29,EXP=200,unitList=UnitList[count_vehicle=2;"
          "count_infantry=1;Infantry[infantryType=SNIPER,quantity=10,weight=20,"
          "position=(1,1)],Vehicle[vehicleType=MORTAR,quantity=20,weight=20,"
          "position=(1,2)],Vehicle[vehicleType=TRUCK,quantity=15,weight=10,"
          "position=(2,2)]],battleField=]"
       << endl;
  cout << "ARVN[LF=15,EXP=180,unitList=UnitList[count_vehicle=2;count_infantry="
          "1;Infantry[infantryType=SNIPER,quantity=9,weight=20,position=(3,3)],"
          "Vehicle[vehicleType=MORTAR,quantity=5,weight=20,position=(3,2)],"
          "Vehicle[vehicleType=TRUCK,quantity=1,weight=10,position=(3,1)]],"
          "battleField=]"
       << endl;
  cout << "* After the fight" << endl;
  cout << "LiberationArmy[LF=20,EXP=180,unitList=UnitList[count_vehicle=2;"
          "count_infantry=1;Infantry[infantryType=SNIPER,quantity=9,weight=20,"
          "position=(3,3)],Vehicle[vehicleType=TRUCK,quantity=16,weight=10,"
          "position=(2,2)],Vehicle[vehicleType=MORTAR,quantity=5,weight=20,"
          "position=(3,2)]],battleField=]"
       << endl;
  cout << "ARVN[LF=0,EXP=0,unitList=UnitList[count_vehicle=0;count_infantry=0],"
          "battleField=]"
       << endl;
  cout << "Result: " << endl;
  g_satc_01();
  return 0;
}
