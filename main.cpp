#include "hcmcampaign.h"
using namespace std;
void g_satc_01() {
  cout << "----- Sample Testcase 01 -----" << endl;
  Configuration *config = new Configuration("config.txt");
  cout << config->str() << endl;
  delete config;
  cout << "Expected:" << endl;
  cout << "----- Sample Testcase 01 -----" << endl;
  cout
      << "[num_rows=10,num_cols=8,arrayForest=[(1,2),(3,5)],arrayRiver=[(0,0),("
         "0,4)],arrayFortification=[(6,6)],arrayUrban=[(2,0)],arraySpecialZone="
         "[(9,7)],liberationUnits=[Vehicle[vehicleType=TANK,quantity=5,weight="
         "2,position=(1,2)],Infantry[infantryType=REGULARINFANTRY,quantity=5,"
         "weight=2,position=(1,1)]],ARVNUnits=[Vehicle[vehicleType=TANK,"
         "quantity=5,weight=2,position=(3,2)],Infantry[infantryType="
         "REGULARINFANTRY,quantity=5,weight=2,position=(3,3)]],eventCode=23]"
      << endl;
}
/**
 * @brief Sample Testcase 02
 *
 *
 * @version 1.0.0
 * @author Your Full Name <your.email@company.com>
 * @date 2025-05-27
 * @copyright Copyright (c) 2025 Your Company Name.
 * @copyright Licensed under MIT License.
 */
void g_satc_02() {
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

  cout << "Expected:" << endl;
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
}


void g_satc_03() {
  cout << "----- Sample Testcase 03 -----" << endl;
  HCMCampaign *campaign = new HCMCampaign("config.txt");
  cout << "** Before the fight" << endl;
  cout << campaign->printResult() << endl;
  cout << "** After the fight" << endl;
  campaign->run();
  cout << campaign->printResult() << endl;
  delete campaign;

  cout << "Expected:" << endl;
  cout << "----- Sample Testcase 03 -----" << endl;
  cout << "** Before the fight " << endl;
  cout << "LIBERATIONARMY[LF=62,EXP=290]-ARVN[LF=62,EXP=290]" << endl;
  cout << "** After the fight " << endl;
  cout << "LIBERATIONARMY[LF=62,EXP=290]-ARVN[LF=0,EXP=0]" << endl;
}

void g_satc_05() {
  cout << "----- Sample Testcase 05 -----" << endl;
  Vehicle tankOfLiber5(20, 20, Position(1, 2), MORTAR);
  cout << tankOfLiber5.str() << endl;
  cout << "Expected:" << endl;
  cout << "----- Sample Testcase 05 -----" << endl;
  cout << "Vehicle[vehicleType=MORTAR,quantity=20,weight=20,position=(1,2)]"
  << endl;

}

void g_satc_06() {
  cout << "----- Sample Testcase 07 -----" << endl;
  Vehicle tankOfLiber1(20, 20, Position(1, 2), MORTAR);
  Vehicle tankOfLiber2(5, 20, Position(3, 2), MORTAR);
  Infantry sniperOfLiber1(10, 20, Position(1, 0), SNIPER);
  Infantry sniperOfLiber2(10, 20, Position(1, 1), SNIPER);
  Unit **unitArrayOfLiber8 = new Unit *[4];
  unitArrayOfLiber8[0] = &tankOfLiber1;
  unitArrayOfLiber8[1] = &tankOfLiber2;
  unitArrayOfLiber8[2] = &sniperOfLiber1;
  unitArrayOfLiber8[3] = &sniperOfLiber2;
  LiberationArmy *liberationArmy8 =
      new LiberationArmy(unitArrayOfLiber8, 4, "LiberationArmy", 0);
  cout << liberationArmy8->str() << endl;
  cout << "Expected:" << endl;
  cout << "----- Sample Testcase 07 -----" << endl;
  cout << "LiberationArmy[LF=38,EXP=400,unitList=UnitList[count_vehicle=1;"
          "count_infantry=1;Infantry[infantryType=SNIPER,quantity=24,weight=20,"
          "position=(1,0)],Vehicle[vehicleType=MORTAR,quantity=25,weight=20,"
          "position=(1,2)]],battleField=]"
       << endl;
}

void g_satc_07() {
  cout << "----- Sample Testcase 06 -----" << endl;
  Unit **unitArrayOfLiber6 = new Unit *[1];
  Vehicle tankOfLiber6(20, 20, Position(1, 2), MORTAR);
  unitArrayOfLiber6[0] = &tankOfLiber6;
  LiberationArmy *liberationArmy6 =
      new LiberationArmy(unitArrayOfLiber6, 0, "LiberationArmy", 0);
  cout << liberationArmy6->str() << endl;
  cout << "Expected:" << endl;
  cout << "----- Sample Testcase 06 -----" << endl;
  cout << "LiberationArmy[LF=0,EXP=0,unitList=UnitList[count_vehicle=0;count_"
          "infantry=0],battleField=]"
       << endl;
}

int main(int argc, const char *argv[]) {
  cout << "Result:" << endl;
  g_satc_01();
  cout << "Result:" << endl;
  g_satc_02();
  cout << "Result:" << endl;
  g_satc_03();
  cout << "Result:" << endl;
  g_satc_05();
  cout << "Result:" << endl;
  g_satc_06();
  cout << "Result:" << endl;
  g_satc_07();
  return 0;
}
