#include "hcmcampaign.h"
// Hồ Sỹ Thắng - 2213188

// Position
Position::Position(int r, int c) : r(r), c(c) {}
Position::Position(const string &str_pos) {
  char ignore;
  stringstream ss(str_pos);
  ss >> ignore >> r >> ignore >> c;
}
int Position::getRow() const { return this->r; }
int Position::getCol() const { return this->c; }
void Position::setRow(int r) { this->r = r; }
void Position::setCol(int c) { this->c = c; }
string Position::str() const {
  stringstream ss;
  ss << "(" << r << "," << c << ")";
  return ss.str();
}

// Unit::
Unit::Unit(int quantity, int weight, Position pos)
    : quantity(quantity), weight(weight), pos(pos), attackScore(-1) {}
Unit::~Unit() {}
Position Unit::getCurrentPosition() const { return pos; }
void Unit::setQuantity(int quantity) { this->quantity = quantity; }
int Unit::getQuantity() { return this->quantity; }
void Unit::setWeight(int weight) { this->weight = weight; }
int Unit::getWeight() { return this->weight; }
void Unit::setAttackScore(int score) { this->attackScore = score; }
// Vehicle::
Vehicle::Vehicle(int quantity, int weight, Position pos,
                 VehicleType vehicleType)
    : Unit(quantity, weight, pos), vehicleType(vehicleType) {}
int Vehicle::getAttackScore() {
  int score = ceil((double)(this->vehicleType * 304 + quantity * weight) / 30);
  return score;
}
string Vehicle::getVehicleTypeName(VehicleType type) {
  switch (type) {
  case TRUCK:
    return "TRUCK";
  case MORTAR:
    return "MORTAR";
  case ANTIAIRCRAFT:
    return "ANTIAIRCRAFT";
  case ARMOREDCAR:
    return "ARMOREDCAR";
  case APC:
    return "APC";
  case ARTILLERY:
    return "ARTILLERY";
  case TANK:
    return "TANK";
  default:
    return "UNKNOWN";
  }
}
string Vehicle::str() const {
  return "Vehicle[vehicleType=" + getVehicleTypeName(vehicleType) +
         ",quantity=" + to_string(quantity) + ",weight=" + to_string(weight) +
         ",position=" + pos.str() + "]";
}
VehicleType Vehicle::getVehicleType() const { return vehicleType; }

// Infantry::
// Private method
bool Infantry::isSquareNumber(int n) const {
  int root = sqrt(n);
  return (root * root == n);
}
int Infantry::digitalRoot(int n) const {
  while (n >= 10) {
    int sum = 0;
    while (n > 0) {
      sum += (n % 10);
      n /= 10;
    }
    n = sum;
  }
  return n;
}
// Public method
Infantry::Infantry(int quantity, int weight, Position pos,
                   InfantryType infantryType)
    : Unit(quantity, weight, pos), infantryType(infantryType) {}
int Infantry::getAttackScore2() {
  int score = infantryType * 56 + quantity * weight;
  if (infantryType == SPECIALFORCES && isSquareNumber(weight)) {
    score += 75;
  }
  return score;
}
void Infantry::applyBoostOnce() {
  if (boosted)
    return;
  int baseScore = infantryType * 56 + quantity * weight;
  if (infantryType == SPECIALFORCES && isSquareNumber(weight)) {
    baseScore += 75;
  }
  int root = digitalRoot(baseScore + 1975);
  if (root > 7) {
    quantity = ceil(quantity * 1.2);
  } else if (root < 3) {
    quantity = ceil(quantity * 0.9);
  }
  boosted = true;
}
void Infantry::resetBoosted() {
  boosted = false; // Đặt lại trạng thái boosted về false
  // Không cần reset quantity và weight vì chúng không thay đổi sau khi boost
}
int Infantry::getAttackScore() {
  applyBoostOnce();
  int score = infantryType * 56 + quantity * weight;
  if (infantryType == SPECIALFORCES && isSquareNumber(weight)) {
    score += 75;
  }
  return score;
}
string Infantry::getInfantryTypeName(InfantryType type) {
  switch (type) {
  case SNIPER:
    return "SNIPER";
  case ANTIAIRCRAFTSQUAD:
    return "ANTIAIRCRAFTSQUAD";
  case MORTARSQUAD:
    return "MORTARSQUAD";
  case ENGINEER:
    return "ENGINEER";
  case SPECIALFORCES:
    return "SPECIALFORCES";
  case REGULARINFANTRY:
    return "REGULARINFANTRY";
  default:
    return "UNKNOWN";
  }
}

string Infantry::str() const {
  return "Infantry[infantryType=" + getInfantryTypeName(infantryType) +
         ",quantity=" + to_string(quantity) + ",weight=" + to_string(weight) +
         ",position=" + pos.str() + "]";
}
InfantryType Infantry::getInfantryType() const { return infantryType; }

// UnitList::
// Hàm đệ quy để kiểm tra xem S có thể tạo thành từ tổng các lũy thừa riêng biệt
// của base không
bool UnitList::canBeExpressedAsSumOfDistinctPowers(int S, int base, int power,
                                                   int sum) {
  if (sum == S)
    return true;
  if (sum > S)
    return false;
  int nextPower = pow(base, power);
  if (nextPower > S)
    return false;
  // Chọn lũy thừa hiện tại vào tổng
  if (canBeExpressedAsSumOfDistinctPowers(S, base, power + 1, sum + nextPower))
    return true;
  // Không chọn lũy thừa hiện tại, thử với power tiếp theo
  return canBeExpressedAsSumOfDistinctPowers(S, base, power + 1, sum);
}
// Hàm kiểm tra số đặc biệt
bool UnitList::isSpecialNumber(int S) const {
  if (S <= 0)
    return false;
  vector<int> primeBases = {3, 5, 7}; // Các cơ số nguyên tố lẻ nhỏ hơn 10
  for (int base : primeBases) {
    if (canBeExpressedAsSumOfDistinctPowers(S, base, 0, 0)) {
      return true;
    }
  }
  return false;
}
UnitList::UnitList(int S) {
  head = nullptr;
  size = 0;
  setCapacity(S);
}
void UnitList::setCapacity(int S) { capacity = isSpecialNumber(S) ? 12 : 8; }
void UnitList::clear() {
  Node *current = head;
  while (current) {
    Node *temp = current;
    current = current->next;
    // Không delete unit vì unit được tạo từ bên ngoài
    delete temp;
  }
  head = nullptr;
  size = 0;
}
UnitList::~UnitList() { clear(); }

bool UnitList::insert(Unit *unit) {
  if (!unit || size >= capacity)
    return false;
  Node *current = head;
  while (current) {
    Vehicle *v = dynamic_cast<Vehicle *>(current->unit);
    Infantry *i = dynamic_cast<Infantry *>(current->unit);
    if (v && dynamic_cast<Vehicle *>(unit)) // Nếu đã có => Cộng quantity
    {
      if (v->getVehicleType() ==
          dynamic_cast<Vehicle *>(unit)->getVehicleType()) {
        v->setQuantity(v->getQuantity() + unit->getQuantity()); //! Not clear
        v->setWeight(max(v->getWeight(), unit->getWeight())); // Cập nhật weight
        // Cập nhật attack score
        return true;
      }
    }
    if (i && dynamic_cast<Infantry *>(unit)) // Nếu đã có => Cộng quantity
    {
      if (i->getInfantryType() ==
          dynamic_cast<Infantry *>(unit)->getInfantryType()) {
        i->setQuantity(i->getQuantity() + unit->getQuantity()); //! Not clear
        i->setWeight(max(i->getWeight(), unit->getWeight()));
        i->resetBoosted(); // Đặt boosted về false
        i->applyBoostOnce(); // Cập nhật attack score
        return true;
      }
    }
    current = current->next;
  }
  // Nếu không tìm thấy trùng lặp => Thêm vào danh sách
  Node *newNode = new Node(unit);
  if (dynamic_cast<Vehicle *>(unit)) // Nếu là Vehicle -> Thêm vào cuối
  {
    if (!head)
      head = newNode;
    else {
      Node *temp = head;
      while (temp->next)
        temp = temp->next;
      temp->next = newNode;
    }
    size++;
    return true;
  } else if (dynamic_cast<Infantry *>(unit)) // Nếu là Infantry -> Thêm vào đầu
  {
    newNode->next = head;
    head = newNode;
    size++;
    // Cập nhật attack score cho Infantry
    Infantry *infantryUnit = dynamic_cast<Infantry *>(unit);
    infantryUnit->applyBoostOnce(); // Cập nhật attack score
    return true;
  }
  delete newNode;
  return false;
}
/**
 * @brief Kiểm tra xem danh sách có chứa loại xe cụ thể không
 *
 * @param[in] vehicleType
 * @return true
 * @return false
 *
 * @author Hồ Sỹ Thắng
 */
bool UnitList::isContain(VehicleType vehicleType) // Có TRUCK, TANK chưa ....
{
  Node *current = head;
  while (current) {
    Vehicle *v = dynamic_cast<Vehicle *>(current->unit);
    if (v && v->getVehicleType() == vehicleType)
      return true;
    current = current->next;
  }
  return false;
}
bool UnitList::isContain(
    InfantryType infantryType) // Có SNIPER, ENGINEER chưa ...
{
  Node *current = head;
  while (current) {
    Infantry *v = dynamic_cast<Infantry *>(current->unit);
    if (v && v->getInfantryType() == infantryType)
      return true;
    current = current->next;
  }
  return false;
}
string UnitList::str() const {
  stringstream ss;
  int count_vehicle = 0, count_infantry = 0;
  Node *current = head;
  while (current) {
    if (dynamic_cast<Vehicle *>(current->unit))
      count_vehicle++;
    if (dynamic_cast<Infantry *>(current->unit))
      count_infantry++;
    current = current->next;
  }
  ss << "UnitList[count_vehicle=" << count_vehicle
     << ";count_infantry=" << count_infantry;
  current = head;
  if (current)
    ss << ";";
  while (current) {
    ss << current->unit->str();
    if (current->next) {
      ss << ",";
    }
    current = current->next;
  }
  ss << "]";
  return ss.str();
}
void UnitList::remove(Unit *unit) {
  if (!head)
    return;
  if (head->unit == unit) {
    Node *temp = head;
    head = head->next;
    delete temp;
    size--;
    return;
  }
  Node *current = head;
  while (current->next) {
    if (current->next->unit == unit) {
      Node *temp = current->next;
      current->next = temp->next; // Bỏ phần tử khỏi danh sách
      delete temp;                // Xóa phần tử
      size--;
      return;
    }
    current = current->next;
  }
}

// Army::
/**
 * @brief Construct a new Army:: Army object
 *
 * @param[in] unitArray
 * @param[in] size
 * @param[in] name
 * @param[in] battleField
 *
 * @author Hồ Sỹ Thắng
 */
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField)
    : name(name), battleField(battleField) {
  LF = 0;
  EXP = 0;
  for (int idx = 0; idx < size; idx++) {
    Unit *u = const_cast<Unit *>(unitArray[idx]);
    Vehicle *v = dynamic_cast<Vehicle *>(u);
    Infantry *i = dynamic_cast<Infantry *>(u);

    if (v) {
      LF += v->getAttackScore();
    }
    if (i) {
      EXP += i->getAttackScore2();
    }
  }
  LF = max(0, min(LF, 1000));
  EXP = max(0, min(EXP, 500));
  unitList = new UnitList(LF + EXP);
  for (int i = 0; i < size; i++) {
    unitList->insert(const_cast<Unit *>(unitArray[i]));
  }
  updatePowerStats(); // Cập nhật chỉ số
}
Army::~Army() {
  if (unitList) {
    delete unitList;
    unitList = nullptr;
  }
}
int Army::getLF() const { return this->LF; }
int Army::getEXP() const { return this->EXP; }
void Army::setLF(int LF) { this->LF = LF; }
void Army::setEXP(int EXP) { this->EXP = EXP; }

UnitList *Army::getUnitList() { return unitList; }
void Army::removeUnitFromList(vector<Unit *> unitsToRemove) {
  for (Unit *unit : unitsToRemove) {
    unitList->remove(unit);
  }
}
void Army::updatePowerStats() {
  this->LF = 0;
  this->EXP = 0;
  Node *current = unitList->getHead();

  while (current) {
    Vehicle *v = dynamic_cast<Vehicle *>(current->unit);
    Infantry *i = dynamic_cast<Infantry *>(current->unit);
    if (v)
      this->LF += v->getAttackScore();
    if (i)
      this->EXP += i->getAttackScore();
    current = current->next;
  }
  this->LF = min(1000, this->LF);
  this->EXP = min(500, this->EXP);
}
// LiberationArmy::
// private
int LiberationArmy::findNearestFibonacci(int num) {
  if (num <= 1)
    return 1;
  int a = 1, b = 1;
  while (b < num) {
    int temp = b;
    b = a + b;
    a = temp;
  }
  return b;
}
vector<Unit *> LiberationArmy::findOptimalCombination(vector<Unit *> units,
                                                      int targetScore) {
  vector<Unit *> bestCombination;
  int bestScore = INT_MAX;
  int n = units.size();
  int totalCombination = 1 << n; // 2^n tổ hợp có thể
  for (int mask = 1; mask < totalCombination; mask++) {
    vector<Unit *> combination;
    int totalScore = 0;
    for (int i = 0; i < n; i++) {
      if (mask & (1 << i)) {
        combination.push_back(units[i]);
        totalScore += units[i]->getAttackScore();
      }
    }
    if (totalScore > targetScore && totalScore < bestScore) {
      bestScore = totalScore;
      bestCombination = combination;
    }
  }
  return bestCombination;
}
void LiberationArmy::captureUnits(Army *enemy) {
  vector<Unit *> captured; // Chỉ xóa những cái này
  vector<Unit *> units;
  // 1. Lấy toàn bộ đơn vị từ ARVN theo thứ tự từ cuối lên đầu
  Node *current = enemy->getUnitList()->getHead();
  while (current) {
    units.push_back(current->unit);
    current = current->next;
  }
  // 2. Duyệt từ cuối lên, insert từng unit (nếu được)
  for (int i = units.size() - 1; i >= 0; i--) {
    Unit *unit = units[i];
    if (unitList->insert(unit)) // Nếu insert thành công => Lấy được
      captured.push_back(unit);
  }
  // 3. Chỉ xóa những cái đã bị lấy khỏi danh sách của enemy
  enemy->removeUnitFromList(captured);
}
// public

LiberationArmy::LiberationArmy(Unit **unitArray, int size, string name,
                               BattleField *battleField)
    : Army(unitArray, size, name, battleField) {}
void LiberationArmy::processBattleVictory(vector<Unit *> A, vector<Unit *> B,
                                          Army *enemy) {
  if (!A.empty())
    removeUnitFromList(A);
  if (!B.empty())
    removeUnitFromList(B);

  captureUnits(enemy); // Trưng dụng
  updatePowerStats();  // Cập nhật chỉ số
}
void LiberationArmy::fight(Army *enemy, bool defense) {
  if (!enemy)
    return;
  if (!defense) // Tấn công
  {
    LF = ceil(LF * 1.5);
    EXP = ceil(EXP * 1.5);
    vector<Unit *> vehicles, infantry;
    Node *current = unitList->getHead();
    while (current) {
      if (dynamic_cast<Vehicle *>(current->unit)) {
        vehicles.push_back(current->unit);
      } else if (dynamic_cast<Infantry *>(current->unit)) {
        infantry.push_back(current->unit);
      }
      current = current->next;
    }
    vector<Unit *> A = findOptimalCombination(infantry, enemy->getEXP());
    vector<Unit *> B = findOptimalCombination(vehicles, enemy->getLF());
    if (!A.empty() && !B.empty()) {
      processBattleVictory(A, B, enemy);
      return;
    } else if (!A.empty() && LF > enemy->getLF()) {
      processBattleVictory(A, vehicles, enemy);
      return;
    } else if (!B.empty() && EXP > enemy->getEXP()) {
      processBattleVictory(B, infantry, enemy);
      return;
    }
    current = unitList->getHead();
    while (current) {
      current->unit->setWeight(ceil(current->unit->getWeight() * 0.9));
      current = current->next;
    }
    updatePowerStats();
  } else // Phòng thủ
  {
    LF = ceil(LF * 1.3);
    EXP = ceil(EXP * 1.3);
    if (LF > enemy->getLF() && EXP > enemy->getEXP()) {
      return; // Phòng thủ thành công
    } else if ((LF < enemy->getLF() && EXP > enemy->getEXP()) ||
               (LF > enemy->getLF() && EXP < enemy->getEXP())) // Không thất bại
    {
      Node *current = unitList->getHead();
      while (current) {
        current->unit->setWeight(ceil(current->unit->getWeight() * 0.9));
        current = current->next;
      }
      updatePowerStats();
    } else {
      Node *current = unitList->getHead();
      while (current) {
        current->unit->setQuantity(this->findNearestFibonacci(
            current->unit->getQuantity())); // Chi viện xong đánh lại
        current = current->next;
      }
      updatePowerStats(); // Cập nhật chỉ số
    }
  }
}
string LiberationArmy::str() const {
  stringstream ss;
  ss << "LiberationArmy[LF=" << LF << ",EXP=" << EXP
     << ",unitList=" << unitList->str() << ",battleField=]";
  return ss.str();
}

// ARVN::
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField) {}
void ARVN::fight(Army *enemy, bool defense) {
  if (!defense) // Tấn công
  {
    // Khi đánh -> Ảnh hưởng của ARVN
    Node *current = unitList->getHead();
    vector<Unit *> toRemove;
    while (current) {
      current->unit->setQuantity(ceil(current->unit->getQuantity() * 0.8));
      if (current->unit->getQuantity() <= 1)
        toRemove.push_back(current->unit);
      current = current->next;
    }
    removeUnitFromList(toRemove);
    updatePowerStats();
  } else {
    // LiberationArmy tấn công trước => nếu thua sẽ bị trưng dụng (captureUnits)
    // Ở đây chỉ xử lý hậu quả nếu thua
    if (!unitList->getHead())
      return;
    Node *current = unitList->getHead();
    while (current) {
      current->unit->setWeight(ceil(current->unit->getWeight() * 0.8));
      current = current->next;
    }
    updatePowerStats();
  }
}
string ARVN::str() const {
  stringstream ss;
  ss << "ARVN[LF=" << LF << ",EXP=" << EXP << ",unitList=" << unitList->str()
     << ",battleField=]";
  return ss.str();
}

// TerrainElement::
double TerrainElement::calculateDistance(const Position &p1,
                                         const Position &p2) const {
  int dx = p1.getRow() - p2.getRow();
  int dy = p1.getCol() - p2.getCol();
  return sqrt(dx * dx + dy * dy);
}
TerrainElement::TerrainElement() : position(Position(0, 0)) {}
TerrainElement::TerrainElement(const Position &pos) : position(pos) {}
TerrainElement::~TerrainElement() {}
// 1. Road:: Không làm gì
// 2. Forest::
void Forest::getEffect(Army *army) {
  Node *current = army->getUnitList()->getHead();
  while (current) {
    Unit *unit = current->unit;
    Position unitPos = unit->getCurrentPosition();
    double distance = calculateDistance(unitPos, position);
    if (dynamic_cast<LiberationArmy *>(army)) {
      if (distance <= 2) {
        Infantry *infantry = dynamic_cast<Infantry *>(unit);
        if (infantry) {
          army->setEXP(ceil(army->getEXP() + unit->getAttackScore() * 0.3));
        }
        Vehicle *vehicle = dynamic_cast<Vehicle *>(unit);
        if (vehicle) {
          army->setLF(ceil(army->getLF() - unit->getAttackScore() * 0.1));
        }
      }
    } else if (dynamic_cast<ARVN *>(army)) {
      if (distance <= 4) {
        Infantry *infantry = dynamic_cast<Infantry *>(unit);
        if (infantry) {
          army->setEXP(ceil(army->getEXP() + unit->getAttackScore() * 0.2));
        }
        Vehicle *vehicle = dynamic_cast<Vehicle *>(unit);
        if (vehicle) {
          army->setLF(ceil(army->getLF() - unit->getAttackScore() * 0.05));
        }
      }
    }
    current = current->next;
  }
}

// 3. River::
void River::getEffect(Army *army) {
  Node *current = army->getUnitList()->getHead();
  while (current) {
    Unit *unit = current->unit;
    Position unitPos = unit->getCurrentPosition();
    double distance = calculateDistance(unitPos, position);
    if (distance <= 2) {
      Infantry *infantry = dynamic_cast<Infantry *>(unit);
      if (infantry) {
        infantry->setAttackScore(ceil(infantry->getAttackScore() * 0.9));
      }
    }
    current = current->next;
  }
}

// 4. Khu dân cư
void Urban::getEffect(Army *army) {
  Node *current = army->getUnitList()->getHead();
  while (current) {
    Unit *unit = current->unit;
    Position unitPos = unit->getCurrentPosition();
    double distance = calculateDistance(unitPos, position);
    if (dynamic_cast<LiberationArmy *>(army)) {
      if (distance <= 5) {
        Infantry *infantry = dynamic_cast<Infantry *>(unit);
        if (infantry) {
          InfantryType infantryType = infantry->getInfantryType();
          if (infantryType == SPECIALFORCES ||
              infantryType == REGULARINFANTRY) {
            int base = infantry->getAttackScore();
            infantry->setAttackScore(base + ceil(2 * base / distance));
          }
        }
      }
      if (distance <= 2) {
        Vehicle *vehicle = dynamic_cast<Vehicle *>(unit);
        if (vehicle && vehicle->getVehicleType() == ARTILLERY)
          vehicle->setAttackScore(ceil(vehicle->getAttackScore() * 0.5));
      }
    } else if (dynamic_cast<ARVN *>(army)) {
      if (distance <= 3) // Trong bán kính 3 đơn vị
      {
        Infantry *infantry = dynamic_cast<Infantry *>(unit);
        if (infantry &&
            infantry->getInfantryType() ==
                REGULARINFANTRY) // Chỉ ảnh hưởng đến bộ binh chủ lực
        {
          int base = infantry->getAttackScore();
          infantry->setAttackScore(base + ceil(3 * base / (2 * distance)));
        }
      }
    }
    current = current->next;
  }
}

// 5. Fortification::
void Fortification::getEffect(Army *army) {
  Node *current = army->getUnitList()->getHead();
  while (current) {
    Unit *unit = current->unit;
    Position unitPos = unit->getCurrentPosition();
    double distance = calculateDistance(unitPos, position);
    if (distance <= 2) {
      if (dynamic_cast<LiberationArmy *>(army)) {
        unit->setAttackScore(ceil(unit->getAttackScore() * 0.8));
      } else if (dynamic_cast<ARVN *>(army)) {
        unit->setAttackScore(ceil(unit->getAttackScore() * 1.2));
      }
    }
    current = current->next;
  }
}
// 6. SpecialZone::
void SpecialZone::getEffect(Army *army) {
  Node *current = army->getUnitList()->getHead();
  while (current) {
    Unit *unit = current->unit;
    Position unitPos = unit->getCurrentPosition();
    double distance = calculateDistance(unitPos, position);
    if (distance <= 1) {
      unit->setAttackScore(0);
    }
    current = current->next;
  }
}
// BattleField::
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                         vector<Position *> arrayRiver,
                         vector<Position *> arrayFortification,
                         vector<Position *> arrayUrban,
                         vector<Position *> arraySpecialZone)
    : n_rows(n_rows), n_cols(n_cols) {
  // Cấp phát động cho mảng 2 chiều terrain
  terrain = new TerrainElement **[n_rows];
  for (int i = 0; i < n_rows; i++) {
    terrain[i] = new TerrainElement *[n_cols];
    for (int j = 0; j < n_cols; j++) {
      terrain[i][j] = new Road(Position(i, j)); // Mặc định tất cả là đường mòn
    }
  }
  // Đặt các vị trí trận địa dựa trên input
  for (auto pos : arrayForest)
    terrain[pos->getRow()][pos->getCol()] = new Forest(*pos);
  for (auto pos : arrayRiver)
    terrain[pos->getRow()][pos->getCol()] = new River(*pos);

  for (auto pos : arrayFortification)
    terrain[pos->getRow()][pos->getCol()] = new Fortification(*pos);

  for (auto pos : arrayUrban)
    terrain[pos->getRow()][pos->getCol()] = new Urban(*pos);

  for (auto pos : arraySpecialZone)
    terrain[pos->getRow()][pos->getCol()] = new SpecialZone(*pos);
}
BattleField::~BattleField() {
  for (int i = 0; i < n_rows; i++) {
    for (int j = 0; j < n_cols; j++) {
      delete terrain[i][j];
    }
    delete[] terrain[i];
  }
  delete[] terrain;
}

string BattleField::str() const {
  stringstream ss;
  ss << "BattleField[n_rows=" << n_rows << ",n_cols=" << n_cols << "]";
  return ss.str();
}

TerrainElement *BattleField::getTerrainAt(int row, int col) const {
  if (row >= 0 && row < n_rows && col >= 0 && col < n_cols) {
    return terrain[row][col];
  }
  return nullptr; // Trả về null nếu vị trí không hợp lệ
}
void BattleField::applyTerrainEffects(Army *army) {
  // Duyệt qua toàn bộ bản đồ để tìm các yếu tố địa hình
  for (int i = 0; i < n_rows; i++) {
    for (int j = 0; j < n_cols; j++) {
      if (terrain[i][j]) {              // Nếu ô này có một yếu tố địa hình
        terrain[i][j]->getEffect(army); // Áp dụng ảnh hưởng lên quân đội
      }
    }
  }
}
// Configuration::
// Private
int Configuration::normalizeEventCode(int code) {
  if (code > 99)
    code %= 100;
  if (code < 0)
    code = 0;
  return code;
}
// Hàm phân tích danh sách vị trí địa hình
void Configuration::parsePositionList(const string &line,
                                      vector<Position *> &positions) {
  /*. [<pos1>,<pos2>,..] là vị trí của các yếu tố địa hình trên trận địa. Ví dụ:
  ARRAY_FOREST=[(1,2),(2,3),(3,4)] */
  size_t start = line.find('[') + 1;
  size_t end = line.find(']');
  if (start == string::npos || end == string::npos || start >= end)
    return;
  string content = line.substr(start, end - start); // Lấy nội dung bên trong []
  size_t pos = 0;
  while ((pos = content.find('(', pos)) != string::npos) {
    size_t close = content.find(')', pos);
    if (close == string::npos)
      break;
    string token = content.substr(pos, close - pos + 1);
    positions.push_back(new Position(token));
    pos = close + 1;
  }
}

// Ánh xạ từ string sang VehicleType
VehicleType Configuration::getVehicleTypeFromString(const string &type) {
  const pair<string, VehicleType> vehicleMap[] = {
      {"TRUCK", TRUCK},
      {"MORTAR", MORTAR},
      {"ANTIAIRCRAFT", ANTIAIRCRAFT},
      {"ARMOREDCAR", ARMOREDCAR},
      {"APC", APC},
      {"ARTILLERY", ARTILLERY},
      {"TANK", TANK}};

  for (const auto &pair : vehicleMap) {
    if (pair.first == type)
      return pair.second;
  }
  return vehicleMap[0].second;
}

// Ánh xạ từ string sang InfantryType
InfantryType Configuration::getInfantryTypeFromString(const string &type) {
  const pair<string, InfantryType> infantryMap[] = {
      {"SNIPER", SNIPER},
      {"ANTIAIRCRAFTSQUAD", ANTIAIRCRAFTSQUAD},
      {"MORTARSQUAD", MORTARSQUAD},
      {"ENGINEER", ENGINEER},
      {"SPECIALFORCES", SPECIALFORCES},
      {"REGULARINFANTRY", REGULARINFANTRY}};

  for (const auto &pair : infantryMap) {
    if (pair.first == type)
      return pair.second;
  }
  return infantryMap[0].second;
}

void Configuration::parseUnitList(const string &line) {
  size_t start = line.find('[');
  size_t end = line.rfind(']');
  if (start == string::npos || end == string::npos || start >= end)
    return;

  string content = line.substr(start + 1, end - start - 1);

  // Xử lý từng unit riêng biệt
  size_t pos = 0;
  while (pos < content.length()) {
    // Tìm tên loại đơn vị
    size_t typeEnd = content.find('(', pos);
    if (typeEnd == string::npos)
      break;

    string type = content.substr(pos, typeEnd - pos);
    // Xóa khoảng trắng và dấu phẩy
    while (!type.empty() && (type.back() == ',' || isspace(type.back())))
      type.pop_back();
    while (!type.empty() && isspace(type.front()))
      type.erase(type.begin());

    // Tìm vị trí kết thúc của phần tham số
    int parenCount = 1;
    size_t paramStart = typeEnd + 1;
    size_t paramEnd = paramStart;

    while (parenCount > 0 && paramEnd < content.length()) {
      if (content[paramEnd] == '(')
        parenCount++;
      else if (content[paramEnd] == ')')
        parenCount--;
      paramEnd++;
    }

    if (parenCount != 0)
      break; // Lỗi cú pháp

    string paramStr = content.substr(paramStart, paramEnd - paramStart - 1);

    // Tách các tham số
    vector<string> params;
    size_t pStart = 0;
    int nestedParenCount = 0;

    for (size_t i = 0; i <= paramStr.length(); i++) {
      if (i == paramStr.length() ||
          (paramStr[i] == ',' && nestedParenCount == 0)) {
        string param = paramStr.substr(pStart, i - pStart);
        // Xóa khoảng trắng
        while (!param.empty() && isspace(param.front()))
          param.erase(param.begin());
        while (!param.empty() && isspace(param.back()))
          param.pop_back();

        params.push_back(param);
        pStart = i + 1;
      } else if (paramStr[i] == '(') {
        nestedParenCount++;
      } else if (paramStr[i] == ')') {
        nestedParenCount--;
      }
    }

    // Kiểm tra số lượng tham số (nên là 4: quantity, weight, position,
    // armyBelong)
    if (params.size() != 4) {
      pos = paramEnd;
      continue;
    }

    try {
      int quantity = stoi(params[0]);
      int weight = stoi(params[1]);
      string posStr = params[2]; // Đã là chuỗi dạng (x,y)
      int armyBelong = stoi(params[3]);

      Position pos(posStr);
      Unit *unit = nullptr;

      if (type == "TANK" || type == "TRUCK" || type == "ANTIAIRCRAFT" ||
          type == "ARMOREDCAR" || type == "APC" || type == "ARTILLERY") {
        unit =
            new Vehicle(quantity, weight, pos, getVehicleTypeFromString(type));
      } else if (type == "SNIPER" || type == "ANTIAIRCRAFTSQUAD" ||
                 type == "MORTARSQUAD" || type == "ENGINEER" ||
                 type == "SPECIALFORCES" || type == "REGULARINFANTRY") {
        unit = new Infantry(quantity, weight, pos,
                            getInfantryTypeFromString(type));
      }

      if (unit) {
        if (armyBelong == 0)
          liberationUnits.push_back(unit);
        else
          ARVNUnits.push_back(unit);
      }
    } catch (const std::exception &e) {
      // Xử lý lỗi khi parse số
    }

    pos = paramEnd;
    // Bỏ qua dấu phẩy giữa các đơn vị
    if (pos < content.length() && content[pos] == ',')
      pos++;
  }
}
// Public
Configuration::Configuration(const string &filepath) {
  ifstream file(filepath);
  /*if (!file.is_open())
  {
      cerr << "Không thể mở file cấu hình: " << filepath << endl;
      exit(1);
  }*/
  string line;
  while (getline(file, line)) {
    if (line.find("NUM_ROWS=") == 0) {
      num_rows = stoi(line.substr(9));
    } else if (line.find("NUM_COLS=") == 0) {
      num_cols = stoi(line.substr(9));
    } else if (line.find("ARRAY_FOREST=") == 0) {
      parsePositionList(line, arrayForest);
    } else if (line.find("ARRAY_RIVER=") == 0) {
      parsePositionList(line, arrayRiver);
    } else if (line.find("ARRAY_FORTIFICATION=") == 0) {
      parsePositionList(line, arrayFortification);
    } else if (line.find("ARRAY_URBAN=") == 0) {
      parsePositionList(line, arrayUrban);
    } else if (line.find("ARRAY_SPECIAL_ZONE=") == 0) {
      parsePositionList(line, arraySpecialZone);
    } else if (line.find("UNIT_LIST=[") == 0) {
      parseUnitList(line);
    } else if (line.find("EVENT_CODE") == 0) {
      size_t eq = line.find('=');
      if (eq != string::npos) {
        string val = line.substr(eq + 1);
        while (!val.empty() && isspace(val.back()))
          val.pop_back(); // xóa space cuối
        while (!val.empty() && isspace(val.front()))
          val.erase(val.begin()); // xóa space đầu
        eventCode = normalizeEventCode(stoi(val));
      }
    }
  }
  file.close();
}
Configuration::~Configuration() {
  for (Position *pos : arrayForest)
    delete pos;
  for (Position *pos : arrayRiver)
    delete pos;
  for (Position *pos : arrayFortification)
    delete pos;
  for (Position *pos : arrayUrban)
    delete pos;
  for (Position *pos : arraySpecialZone)
    delete pos;
  for (Unit *unit : liberationUnits)
    delete unit;
  for (Unit *unit : ARVNUnits)
    delete unit;
}
string Configuration::str() const {
  stringstream ss;
  ss << "[num_rows=" << num_rows << ",num_cols=" << num_cols;

  // In arrayForest
  ss << ",arrayForest=[";
  for (size_t i = 0; i < arrayForest.size(); ++i) {
    ss << arrayForest[i]->str();
    if (i < arrayForest.size() - 1)
      ss << ",";
  }
  ss << "]";

  // In arrayRiver
  ss << ",arrayRiver=[";
  for (size_t i = 0; i < arrayRiver.size(); ++i) {
    ss << arrayRiver[i]->str();
    if (i < arrayRiver.size() - 1)
      ss << ",";
  }
  ss << "]";

  // In arrayFortification
  ss << ",arrayFortification=[";
  for (size_t i = 0; i < arrayFortification.size(); ++i) {
    ss << arrayFortification[i]->str();
    if (i < arrayFortification.size() - 1)
      ss << ",";
  }
  ss << "]";

  // In arrayUrban
  ss << ",arrayUrban=[";
  for (size_t i = 0; i < arrayUrban.size(); ++i) {
    ss << arrayUrban[i]->str();
    if (i < arrayUrban.size() - 1)
      ss << ",";
  }
  ss << "]";

  // In arraySpecialZone
  ss << ",arraySpecialZone=[";
  for (size_t i = 0; i < arraySpecialZone.size(); ++i) {
    ss << arraySpecialZone[i]->str();
    if (i < arraySpecialZone.size() - 1)
      ss << ",";
  }
  ss << "]";

  // In liberationUnits
  ss << ",liberationUnits=[";
  for (size_t i = 0; i < liberationUnits.size(); ++i) {
    ss << liberationUnits[i]->str();
    if (i < liberationUnits.size() - 1)
      ss << ",";
  }
  ss << "]";

  // In ARVNUnits
  ss << ",ARVNUnits=[";
  for (size_t i = 0; i < ARVNUnits.size(); ++i) {
    ss << ARVNUnits[i]->str();
    if (i < ARVNUnits.size() - 1)
      ss << ",";
  }
  ss << "]";

  // Cuối cùng: eventCode
  ss << ",eventCode=" << eventCode << "]";

  return ss.str();
}

// Hàm trả về số hàng của bản đồ
int Configuration::getNumRows() const { return num_rows; }
// Hàm trả về số cột của bản đồ
int Configuration::getNumCols() const { return num_cols; }
// Hàm trả về danh sách vị trí của địa hình rừng
vector<Position *> Configuration::getArrayForest() const { return arrayForest; }
// Hàm trả về danh sách vị trí của địa hình sông
vector<Position *> Configuration::getArrayRiver() const { return arrayRiver; }
// Hàm trả về danh sách vị trí của địa hình chiến hào
vector<Position *> Configuration::getArrayFortification() const {
  return arrayFortification;
}
// Hàm trả về danh sách vị trí của địa hình khu dân cư
vector<Position *> Configuration::getArrayUrban() const { return arrayUrban; }
// Hàm trả về danh sách vị trí của khu vực phi quân sự
vector<Position *> Configuration::getArraySpecialZone() const {
  return arraySpecialZone;
}
// Hàm trả về danh sách các đơn vị của Quân Giải phóng
vector<Unit *> Configuration::getLiberationUnits() const {
  return liberationUnits;
}
// Hàm trả về danh sách các đơn vị của Quân đội Sài Gòn
vector<Unit *> Configuration::getARVNUnits() const { return ARVNUnits; }
// Hàm trả về mã sự kiện (Event Code)
int Configuration::getEventCode() const { return eventCode; }
// HCMCampaign::
HCMCampaign::HCMCampaign(const std::string &config_file_path) {
  config = new Configuration(config_file_path);

  // Khởi tạo bản đồ trận địa
  battleField = new BattleField(
      config->getNumRows(), config->getNumCols(), config->getArrayForest(),
      config->getArrayRiver(), config->getArrayFortification(),
      config->getArrayUrban(), config->getArraySpecialZone());

  // Khởi tạo đối tượng ARVN
  arvn = new ARVN((Unit **)config->getARVNUnits().data(),
                  config->getARVNUnits().size(), "ARVN", battleField);

  // Khởi tạo đối tượng LiberationArmy
  liberationArmy = new LiberationArmy(
      (Unit **)config->getLiberationUnits().data(),
      config->getLiberationUnits().size(), "LiberationArmy", battleField);
}

// Destructor: Giải phóng bộ nhớ
HCMCampaign::~HCMCampaign() {
  if (config) {
    delete config;
    config = nullptr;
  }
  if (battleField) {
    delete battleField;
    battleField = nullptr;
  }
  if (liberationArmy) {
    delete liberationArmy;
    liberationArmy = nullptr;
  }
  if (arvn) {
    delete arvn;
    arvn = nullptr;
  }
}

// Phương thức chạy chiến dịch
void HCMCampaign::run() {
  // 1. Áp dụng ảnh hưởng địa hình lên các quân đội
  battleField->applyTerrainEffects(liberationArmy);
  battleField->applyTerrainEffects(arvn);

  // 2. Xác định mã sự kiện
  int eventCode = config->getEventCode();

  // 3. Tiến hành giao tranh dựa trên eventCode
  if (eventCode < 75) {
    // LiberationArmy tấn công trước
    liberationArmy->fight(arvn, false);
  } else {
    // ARVN tấn công trước, sau đó LiberationArmy phản công
    arvn->fight(liberationArmy, false);
    liberationArmy->fight(arvn, false);
  }

  // 4. Xóa các đơn vị quân sự có attackScore ≤ 5
  vector<Unit *> toRemoveLiberation;
  vector<Unit *> toRemoveARVN;

  Node *current = liberationArmy->getUnitList()->getHead();
  while (current) {
    if (current->unit->getAttackScore() <= 5) {
      toRemoveLiberation.push_back(current->unit);
    }
    current = current->next;
  }

  current = arvn->getUnitList()->getHead();
  while (current) {
    if (current->unit->getAttackScore() <= 5) {
      toRemoveARVN.push_back(current->unit);
    }
    current = current->next;
  }

  liberationArmy->removeUnitFromList(toRemoveLiberation);
  arvn->removeUnitFromList(toRemoveARVN);

  // 5. Cập nhật lại chỉ số sức mạnh
  liberationArmy->updatePowerStats();
  arvn->updatePowerStats();
}
// Phương thức in kết quả trận chiến
string HCMCampaign::printResult() {
  stringstream ss;
  ss << "LIBERATIONARMY[LF=" << liberationArmy->getLF()
     << ",EXP=" << liberationArmy->getEXP() << "]"
     << "-ARVN[LF=" << arvn->getLF() << ",EXP=" << arvn->getEXP() << "]";
  return ss.str();
}
