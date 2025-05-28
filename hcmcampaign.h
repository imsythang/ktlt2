/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any
// other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

//! Trường hợp quantity hoặc weight thay đổi sau khi tăng atttackScore thì  sẽ
//! tính theo quantity và weight thay đổi đó Cập nhật ảnh hưởng địa hình trước
//! khi đánh, vậy là sẽ thay đổi attackScore trước khi đánh (khi đánh sẽ thay
//! đổi weight, quantity,...) => Chưa xử lý được
// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType {
  TRUCK,
  MORTAR,
  ANTIAIRCRAFT,
  ARMOREDCAR,
  APC,
  ARTILLERY,
  TANK
};
enum InfantryType {
  SNIPER,
  ANTIAIRCRAFTSQUAD,
  MORTARSQUAD,
  ENGINEER,
  SPECIALFORCES,
  REGULARINFANTRY
};
struct Node {
  Unit *unit;
  Node *next;
  Node(Unit *u) : unit(u), next(nullptr) {}
};
class Position {
private:
  int r, c;

public:
  Position(int r = 0, int c = 0);
  Position(const string &str_pos); // Example: str_pos = "(1,15)"
  int getRow() const;
  int getCol() const;
  void setRow(int r);
  void setCol(int c);
  string str() const; // Example: returns "(1,15)"
};
class Unit {
protected:
  int quantity, weight;
  Position pos;
  int attackScore;

public:
  Unit(int quantity, int weight, Position pos);
  virtual ~Unit();
  void setAttackScore(int score);
  virtual int getAttackScore() = 0;
  Position getCurrentPosition() const;
  virtual string str() const = 0;
  int getQuantity();
  void setQuantity(int quantity);
  int getWeight();
  void setWeight(int weight);
};
class Vehicle : public Unit {
private:
  VehicleType vehicleType;

public:
  Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType);
  int getAttackScore() override;
  string str() const override;
  VehicleType getVehicleType() const;
  static string getVehicleTypeName(VehicleType type);
};
class Infantry : public Unit {
private:
  InfantryType infantryType;
  bool isSquareNumber(int n) const;
  int digitalRoot(int n) const;
  bool boosted = false; // Chỉ áp dụng boost một lần

public:
  Infantry(int quantity, int weight, Position pos, InfantryType infantryType);
  int getAttackScore() override;
  string str() const override;
  InfantryType getInfantryType() const;
  static string getInfantryTypeName(InfantryType type);
  int calculateRawAttackScore() const;
  void applyBoostOnce();
  int getAttackScore2();
  void resetBoosted();
};
class UnitList {
private:
  Node *head;
  int capacity;
  int size;
  static bool canBeExpressedAsSumOfDistinctPowers(int S, int base, int power,
                                                  int sum);
  bool isSpecialNumber(int S) const;

public:
  UnitList(int S);
  ~UnitList();
  bool insert(Unit *unit);                 // return true if insert successfully
  bool isContain(VehicleType vehicleType); // return true if it exists
  bool isContain(InfantryType infantryType); // return true if it exists
  string str() const;
  Node *getHead() const { return head; }
  void remove(Unit *unit);
  void clear();
  void setCapacity(int S);
};

class Army {
protected:
  int LF, EXP;
  string name;
  UnitList *unitList;
  BattleField *battleField;

public:
  Army(Unit **unitArray, int size, string name, BattleField *battleField);
  virtual ~Army();
  virtual void fight(Army *enemy, bool defense = false) = 0;
  virtual string str() const = 0;
  int getLF() const;
  int getEXP() const;
  void setLF(int lf);
  void setEXP(int exp);
  UnitList *getUnitList();
  void removeUnitFromList(vector<Unit *> unitsToRemove);
  void updatePowerStats(); // Cập nhật LF và EXP
};
class LiberationArmy : public Army {
private:
  int findNearestFibonacci(int num);
  vector<Unit *> findOptimalCombination(vector<Unit *> units, int targetScore);
  void captureUnits(Army *enemy);
  void processBattleVictory(vector<Unit *> A, vector<Unit *> B, Army *enemy);

public:
  LiberationArmy(Unit **unitArray, int size, string name,
                 BattleField *battleField);
  void fight(Army *enemy, bool defense = false) override;
  string str() const override;
};
class ARVN : public Army {
public:
  ARVN(Unit **unitArray, int size, string name, BattleField *battleField);
  void fight(Army *enemy, bool defense = false) override;
  string str() const override;
};
class TerrainElement {
protected:
  double calculateDistance(const Position &p1, const Position &p2) const;
  Position position;

public:
  TerrainElement();
  TerrainElement(const Position &pos);
  virtual ~TerrainElement();
  virtual void getEffect(Army *army) = 0;
};

class Road : public TerrainElement {
public:
  // Constructor mặc định
  Road() : TerrainElement() {}
  Road(const Position &pos) : TerrainElement(pos) {}
  // Destructor
  ~Road() {}
  void getEffect(Army *army) override {}
};
class Forest : public TerrainElement {
public:
  // Constructor mặc định
  Forest() : TerrainElement() {}
  Forest(const Position &pos) : TerrainElement(pos) {}
  // Destructor
  ~Forest() {}
  void getEffect(Army *army) override;
};
class River : public TerrainElement {
public:
  River() : TerrainElement() {}
  River(const Position &pos) : TerrainElement(pos) {}
  ~River() {}
  void getEffect(Army *army) override;
};
class Mountain : public TerrainElement {
public:
  // Constructor mặc định
  Mountain() : TerrainElement() {}
  Mountain(const Position &pos) : TerrainElement(pos) {}
  // Destructor
  ~Mountain() {}
  void getEffect(Army *army) override;
};
class Urban : public TerrainElement {
public:
  // Constructor mặc định
  Urban() : TerrainElement() {}
  Urban(const Position &pos) : TerrainElement(pos) {}
  // Destructor
  ~Urban() {}
  void getEffect(Army *army) override;
};
class Fortification : public TerrainElement {
public:
  // Constructor mặc định
  Fortification() : TerrainElement() {}
  Fortification(const Position &pos) : TerrainElement(pos) {}
  // Destructor
  ~Fortification() {}
  void getEffect(Army *army) override;
};
class SpecialZone : public TerrainElement {
public:
  SpecialZone() : TerrainElement() {}
  SpecialZone(const Position &pos) : TerrainElement(pos) {}
  // Destructor
  ~SpecialZone() {}
  void getEffect(Army *army) override;
};

class BattleField {
private:
  int n_rows, n_cols;
  TerrainElement ***terrain;

public:
  BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
              vector<Position *> arrayRiver,
              vector<Position *> arrayFortification,
              vector<Position *> arrayUrban,
              vector<Position *> arraySpecialZone);
  ~BattleField();
  string str() const;
  TerrainElement *getTerrainAt(int row, int col) const;
  void applyTerrainEffects(Army *army);
};
class Configuration {
private:
  int num_rows, num_cols;
  vector<Position *> arrayForest, arrayRiver, arrayFortification, arrayUrban,
      arraySpecialZone;
  vector<Unit *> liberationUnits, ARVNUnits;
  int eventCode;
  VehicleType getVehicleTypeFromString(const string &type);
  InfantryType getInfantryTypeFromString(const string &type);
  void parsePositionList(const string &line, vector<Position *> &positions);
  void parseUnitList(const string &line);
  int normalizeEventCode(int code); // Hàm chuẩn hóa eventCode
public:
  Configuration(const string &filepath);
  ~Configuration();
  string str() const;
  // List hàm get
  int getNumRows() const;
  int getNumCols() const;
  vector<Position *> getArrayForest() const;
  vector<Position *> getArrayRiver() const;
  vector<Position *> getArrayFortification() const;
  vector<Position *> getArrayUrban() const;
  vector<Position *> getArraySpecialZone() const;
  vector<Unit *> getLiberationUnits() const;
  vector<Unit *> getARVNUnits() const;
  int getEventCode() const;
};
class HCMCampaign {
private:
  Configuration *config;
  BattleField *battleField;
  LiberationArmy *liberationArmy;
  ARVN *arvn;

public:
  HCMCampaign(const string &config_file_path);
  void NewFunction();
  ~HCMCampaign();
  void run();

  string printResult();
};

#endif
