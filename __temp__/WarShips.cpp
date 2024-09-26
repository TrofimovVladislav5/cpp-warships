// #include <iostream>
// #include <vector>
// #include <map>

// enum class ShipSize {
//     MIN = 1,
//     MAX = 4
// };

// enum class Direction {
//     VERTICAL = true,
//     HORIZONTAL = false
// };

// enum class SegmentState {
//     INT,
//     DAMAGED,
//     DESTROYED
// };

// enum class CellField {
//     UNKNOWN,
//     EMPTY,
//     SHIP
// };

// class Ship {
// public:
//     Ship(int length, Direction direction){
//         this->length = length;
//         this->direction = direction;
//         if (length < static_cast<int>(ShipSize::MIN) || length > static_cast<int>(ShipSize::MAX)){
//             std::cout << "Out of range" << std::endl;
//             exit(40);
//         }
//         segments = std::vector<SegmentState>(length, SegmentState::INT);
//     }

//     ~Ship() = default;

//     int getLength(){
//         return length;
//     }

//     Direction getDirection(){
//         return direction;
//     }
//     void getDamage(int segmentIndex){
//         if (segmentIndex <  static_cast<int>(ShipSize::MIN) - 1 || segmentIndex > length){
//             std::cout << "Invalid Index" << std::endl;
//             exit(40);
//         }
//         if (segments[segmentIndex] == SegmentState::INT){
//             segments[segmentIndex] = SegmentState::DAMAGED;
//         }

//         else if (segments[segmentIndex] == SegmentState::DAMAGED){
//             segments[segmentIndex] = SegmentState::DESTROYED;
//         }
//         else {
//             std::cout << "Segment destroyed" << std::endl;
//             return;
//         }
//     }

//     bool isDestroyed(){
//         for (auto segment : segments){
//             if (segment != SegmentState::DESTROYED){
//                 return false;
//             }
//         }
//         return true;
//     }

//     void printState(){
//         int count = 1;
//         std::string result = "Ship " + std::to_string(count) + " : ";
//         for (auto segment : segments){
//             switch (segment){
//                 case SegmentState::INT:
//                     result += "INT ";
//                     break;
//                 case SegmentState::DAMAGED:
//                     result += "DAMAGED ";
//                     break;
//                 case SegmentState::DESTROYED:
//                     result += "DESTROYED ";
//                     break;
//             }
//         }
//         std::cout << result << std::endl;
//         count++;
//     }
// private:
//     int length;
//     Direction direction;
//     std::vector<SegmentState> segments;
// };

// class ShipManager {
// public:
//     ShipManager(std::vector<int>shipSizes) {
//         for (auto size: shipSizes){
//             if (size < static_cast<int>(ShipSize::MIN) || size > static_cast<int>(ShipSize::MAX)){
//                 std::cout << "Invalid ship size: " << size << std::endl;
//                 continue;
//             }
//             Ship temp(size, Direction::VERTICAL);
//             ships.push_back(temp);
//         }
//     }

//     ~ShipManager() = default;

//     // void addShip(int size, Direction direction){
//     //     if (size )
//     // }
//     void printShipsInfo(){
//         int count = 1;
//         for (auto ship : ships){
//             std:: cout << "Ship " << count << " Length " << ship.getLength() << std::endl;
//             count++;
//         }
//     }

// private:
//     std::vector<Ship> ships;
// };

// class GameField {
// public:
//     GameField(int width, int height){
//         this->height = height;
//         this->width = width;
//         field = std::vector<std::vector<CellField>>(height, std::vector<CellField>(width));
//     }

//     ~GameField() = default;

//     void printGameField(){
//         for (auto& y : field){
//             for (auto& x : y){
//                 switch (x){
//                 case CellField::UNKNOWN:
//                     std::cout << "U ";
//                     break;
//                 case CellField::EMPTY:
//                     std::cout << "E ";
//                     break;
//                 case CellField::SHIP:
//                     std::cout << "S ";
//                 }
//             }
//             std::cout << std::endl;
//         }
//     }

//     bool placeShip(Ship ship, int x, int y){
//         int length = ship.getLength();
//         Direction direct = ship.getDirection();
//         if (!placeShipValidate(x,y,length, direct) || intersectionShips(x, y, length, direct)){
//             std::cout << "Can't place ship" << std::endl;
//             return false;
//         }
//         for (int i = 0;i < length;i++){
//             if (direct == Direction::VERTICAL){
//                 field[y + i][x] = CellField::SHIP;
//             }
//             else{
//                 field[y][x + i] = CellField::SHIP;
//             }
//         }
//         return true;
//     }

//     void attack(){
        
//     }


// private:
//     int width, height;
//     std::vector<std::vector<CellField>> field;

//     bool placeShipValidate(int x, int y, int length, Direction direct){
//         if (direct == Direction::VERTICAL) {
//             return y + length <= height;
//         }
//         else {
//             return x + length <= width;
//         }
//     }

//     bool intersectionShips(int x, int y, int length, Direction direct){
//         for (int i = 0; i < length;i++){
//             int newX = x;
//             int newY = y;
//             if (direct == Direction::VERTICAL){
//                 int newY = y + i;
//             }
//             if (direct == Direction::HORIZONTAL){
//                 int newX = x + i;
//             }
//             if (field[newY][newX] != CellField::EMPTY || neighborShips(newX, newY)){
//                 return true;
//             }
//         }
//         return false;
//     }

//     bool neighborShips(int x, int y){
//         for (int dy = -1;dy <= 1;dy++){
//             for (int dx = -1;dx <= 1;dx++){
//                 int newX = x + dx;
//                 int newY = y + dy;
//                 if (newX < width && newX >= 0 && newY < height && newY >= 0)
//                     if (field[newY][newX] == CellField::SHIP){
//                         return true;
//                 }
//             }
//         }
//         return false;
//     }
// };

// int main() {
//     Ship sh(2, Direction::HORIZONTAL);
//     GameField field(10, 20);
//     field.placeShip(sh, 5, 5);
//     field.placeShip(sh, 10, 10);
//     field.printGameField();
// }

#include <iostream>
#include <vector>
#include <map>

enum class ShipSize {
    MIN = 1,
    MAX = 4
};

enum class Direction {
    VERTICAL = true,
    HORIZONTAL = false
};

enum class SegmentState {
    INT,
    DAMAGED,
    DESTROYED
};

enum class CellField {
    UNKNOWN,
    EMPTY,
    SHIP
};

class Ship {
public:
    Ship(int length, Direction direction){
        this->length = length;
        this->direction = direction;
        if (length < static_cast<int>(ShipSize::MIN) || length > static_cast<int>(ShipSize::MAX)){
            std::cout << "Out of range" << std::endl;
            exit(40);
        }
        segments = std::vector<SegmentState>(length, SegmentState::INT);
    }

    ~Ship() = default;

    int getLength(){
        return length;
    }

    Direction getDirection(){
        return direction;
    }
    void getDamage(int segmentIndex){
        if (segmentIndex < static_cast<int>(ShipSize::MIN) - 1 || segmentIndex >= length){
            std::cout << "Invalid Index" << std::endl;
            exit(40);
        }
        if (segments[segmentIndex] == SegmentState::INT){
            segments[segmentIndex] = SegmentState::DAMAGED;
        }

        else if (segments[segmentIndex] == SegmentState::DAMAGED){
            segments[segmentIndex] = SegmentState::DESTROYED;
        }
        else {
            std::cout << "Segment destroyed" << std::endl;
            return;
        }
    }

    bool isDestroyed(){
        for (auto segment : segments){
            if (segment != SegmentState::DESTROYED){
                return false;
            }
        }
        return true;
    }

    void printState(){
        int count = 1;
        std::string result = "Ship " + std::to_string(count) + " : ";
        for (auto segment : segments){
            switch (segment){
                case SegmentState::INT:
                    result += "INT ";
                    break;
                case SegmentState::DAMAGED:
                    result += "DAMAGED ";
                    break;
                case SegmentState::DESTROYED:
                    result += "DESTROYED ";
                    break;
            }
        }
        std::cout << result << std::endl;
        count++;
    }
private:
    int length;
    Direction direction;
    std::vector<SegmentState> segments;
};

class GameField {
public:
    GameField(int width, int height){
        this->height = height;
        this->width = width;
        field = std::vector<std::vector<CellField>>(height, std::vector<CellField>(width, CellField::EMPTY)); // Инициализация поля значениями EMPTY
    }

    ~GameField() = default;

    void printGameField(){
        for (auto& y : field){
            for (auto& x : y){
                switch (x){
                    case CellField::UNKNOWN:
                        std::cout << "U ";
                        break;
                    case CellField::EMPTY:
                        std::cout << "- ";
                        break;
                    case CellField::SHIP:
                        std::cout << "S ";
                }
            }
            std::cout << std::endl;
        }
    }

    bool placeShip(Ship ship, int x, int y){
        int length = ship.getLength();
        Direction direct = ship.getDirection();
        if (!placeShipValidate(x, y, length, direct) || intersectionShips(x, y, length, direct)){
            std::cout << "Can't place ship" << std::endl;
            return false;
        }
        for (int i = 0; i < length; i++){
            if (direct == Direction::VERTICAL){
                field[y + i][x] = CellField::SHIP;
            } else {
                field[y][x + i] = CellField::SHIP;
            }
        }
        return true;
    }

private:
    int width, height;
    std::vector<std::vector<CellField>> field;

    bool placeShipValidate(int x, int y, int length, Direction direct){
        if (direct == Direction::VERTICAL) {
            return y + length <= height;
        } else {
            return x + length <= width;
        }
    }

    bool intersectionShips(int x, int y, int length, Direction direct){
        for (int i = 0; i < length; i++){
            int newX = x;
            int newY = y;
            if (direct == Direction::VERTICAL){
                newY = y + i;  // Исправлено: изменяем только координату y для вертикального направления
            }
            if (direct == Direction::HORIZONTAL){
                newX = x + i;  // Исправлено: изменяем только координату x для горизонтального направления
            }
            if (field[newY][newX] != CellField::EMPTY || neighborShips(newX, newY)){
                return true;
            }
        }
        return false;
    }

    bool neighborShips(int x, int y){
        for (int dy = -1; dy <= 1; dy++){
            for (int dx = -1; dx <= 1; dx++){
                int newX = x + dx;
                int newY = y + dy;
                if (newX >= 0 && newX < width && newY >= 0 && newY < height){  // Проверка выхода за границы
                    if (field[newY][newX] == CellField::SHIP){
                        return true;
                    }
                }
            }
        }
        return false;
    }
};

// int main() {
//     Ship sh(2, Direction::HORIZONTAL);
//     GameField field(10, 20);
//     field.placeShip(sh, 5, 5);   // Установка первого корабля
//     field.placeShip(sh, 0, 0); // Установка второго корабля
//     field.printGameField();      // Вывод игрового поля
// }
