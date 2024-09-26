#include <iostream>
#include <vector>
enum class Direction{
    HORIZONTAL,
    VERTICAL
};

enum class SegmentState{
    INT,
    DAMAGED,
    DESTROYED
};

class Ship {
private:
    int length;
    Direction direction;
    std::vector<SegmentState> segments;
public:
    Ship(int length, Direction direction){
        this->length = length;
        this->direction = direction;
        segments = std::vector<SegmentState>(length, SegmentState::INT);
    }

    ~Ship(){
        segments.clear();
    }

    void takeDamage(int number){
        if (number <  0 || number >= length){
            throw std::out_of_range("Invalid length ship");
        }
        switch(segments[number]){
            case SegmentState::INT:{
                segments[number] = SegmentState::DAMAGED;
                break;
            }
            case SegmentState::DAMAGED:{
                segments[number] = SegmentState::DESTROYED;
                break;
            }
            default:
                std::cout << "C" << std::endl;
                break;
        }
    }

    bool destroyed(){
        for (int i = 0;i < length;i++){
            if (segments[i] != SegmentState::DESTROYED){
                return false;
            }
        }
        return true;
    }
};

// int main(){
//     Ship ship(4,Direction::HORIZONTAL);
// }