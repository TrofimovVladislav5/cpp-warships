#include <iostream>

class GameController{
private:
    GameState* currentState;
    
public:

    void proceedCommand(std::string command){
        
    }

    // void startFromSave(std::string savePath){

    // }

    // void saveCurrentGame(std::string savePath){

    // }
};

class GameState {
protected:
    GameController* gameController;
public:
    
    ~GameState() = default;

    virtual void openState() = 0;

    virtual void updateState() = 0;

    virtual void closeState() = 0;

    virtual bool tryTransitState(std::string command, GameState);
};

class OngoingGameState : public GameState{
public:
    OngoingGameState(GameController* GameController){
        this->gameController = gameController;
    }

    ~OngoingGameState() = default;
    void openState() override {
        std::cout << "Игра началась" << std::endl;
    }

    void updateState() override {
        std::cout << "Игра обновляется" << std::endl;
    }

    void closeState() override {
        std::cout << "Выход из игры" << std::endl;
    }
    
    bool tryTransitState(std::string command, GameState* newState){
        if (command == "pause"){
            newState = new PausedGameState(gameController);
            return true;
        }

        else if (command == "menu"){
            newState = new MenuGameState(gameController);
            return true;
        }
        return false;
    }
};

class PausedGameState : public GameState{
public:
    PausedGameState(GameController* gameController){
        this->gameController = gameController;
    }

    ~PausedGameState() = default;
    void openState() override {
        std::cout << "Игра на паузе " << std::endl;
    }

    void updateState() override {
        std::cout << "Ожидание команды resume для продолжения " << std::endl;
    }

    void closeState() override {
        std::cout << "Возвращение из паузы" << std::endl;
    }

    bool tryTransitState(std::string command, GameState* newState){
        if (command == "resume"){
            newState = new OngoingGameState(gameController);
            return true;
        }

        else if (command == "menu"){
            newState = new MenuGameState(gameController);
            return true;
        }
        return false;
    }
};

class MenuGameState : public GameState{
public:
    MenuGameState(GameController* gameController){
        this->gameController = gameController;
    }
    ~MenuGameState() = default;

    void openState(){
        std::cout << "Открыто главное меню игры " << std::endl;
    }

    void updateState(){
        std::cout << "Меню игры. Ожидание команды... " << std::endl;
    }

    void closeState(){
        std::cout << "Выход из меню игры " << std::endl;
    }

    bool tryTransitState(std::string command, GameState* newState){
        if (command == "run") {
            newState = new OngoingGameState(gameController);
            return true;
        }

        else if (command == "end"){
            std::cout << "Игра завершена." << std::endl;
            exit(40);
        }
        return false;
    }
};

class Command {
public:
    virtual void execute() {
        system("cls");
    }
};

class MenuCommand : public Command {
public:
    void execute() override {
        Command::execute();
        std::cout << "dlsjkfalfafasf;" << std::endl;
    }
};

class PauseCommand : public Command {
public:
    void execute() override {
        Command::execute();
        std::cout << "Игра приостановлена." << std::endl;
    }
};

class ExitCommand : public Command {
public:
    void execute() override{
        Command::execute();
        std::cout << "Игра завершена " << std::endl;
        exit(40);
    }
};

class RunCommand : public Command {
public:
    void execute() override {
        Command::execute();
        std::cout << "Игра началась" << std::endl;
    }
};

class CommandFactory {
public:
    Command* createCommand(std::string input){
        if (input == "menu"){
            return new MenuCommand();
        }
        else if (input == "pause"){
            return new PauseCommand();
        }
        else if (input == "exit"){
            return new ExitCommand();
        }
        else if (input == "run"){
            return new RunCommand();
        }
        return nullptr;
    }
};

int main(int argc, char** argv){
    while (1){
        std::string input;
        std::getline(std::cin, input);
        CommandFactory factory;
        Command* command = factory.createCommand(input);
        if (command != nullptr){
            command->execute();
        }
        else{
            std::cout << "Неизвестная команда" << input << std::endl;
        }
    }
}   