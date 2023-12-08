#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <limits>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <algorithm> // Для преобразования регистра

#pragma region Переменные
bool detail = false;
std::string hotelName;
int numFloors, numElevators, elevatorCapacity, peopleWaitingPerFloor, numWaitingPeople;
std::vector<std::vector<int>> destinationFloors(numFloors);

#pragma endregion
#pragma region Класс Лифта
class Elevator {
private:
    int capacity;
    int countPass;
    std::vector<int> destinationFloors;

public:
    Elevator(int capacity, int countPass, std::vector<int> destinationFloors) : capacity(capacity), countPass(countPass), destinationFloors(destinationFloors) {}
    int getNextDestination() {//Переходим к следующей позиции лифта
        if (!destinationFloors.empty()) {
            int nextFloor = destinationFloors.back();
            destinationFloors.pop_back();
            return nextFloor;
        }
        return -1;
    }
    int getCurrentPosition() {//Возвращаем тукущую позицию лифта
        if (!destinationFloors.empty()) {
            int currentFloor = destinationFloors.back();
            return currentFloor;
        }
        return -1;
    }
    int addPass(int Count) {//Добавляем пассажиров
        countPass = countPass + Count;
        return countPass;
    }
    int minusPass(int Count) {//Вычетаем пассажиров
        countPass = countPass - Count;
        return countPass;
    }
    int getPass() {//Возвращаем тукущую позицию лифта
        return countPass;
    }
    int getVector() {//Возвращаем тукущую позицию лифта
        return destinationFloors.size();
    }
    int pushPass() {//Сброс пассажиров
        countPass = 0;
        return destinationFloors.size();
    }
    bool addNewDestination(int Floor) {
        destinationFloors.insert(destinationFloors.begin(), Floor);
        return true;
    }

};
#pragma endregion
#pragma region Метод отрисовки положения лифтов
void displayElevatorPositions(const std::vector<int>& elevatorPositions, std::vector<Elevator> lifts)
{
    std::string topBottomBorder = "++-----+";
    std::string middleBorder = "-----+";

    int fixedWidth = 21;
    // Определяем пустое пространство слева и справа от переменной hotelName
    int padding = (fixedWidth - hotelName.size()) / 2;
    // Верхняя рамка
    std::cout << "++-----------------++\n";
    std::cout << "++" << std::setw(padding + hotelName.size()) << std::right << hotelName << std::setw(padding) << "++\n";
    std::cout << "++-----------------++\n";

    // Отрисовываем содержимое вектора, представляющего позиции лифтов
    for (int floor = numFloors; floor >= 1; --floor) {
        int currentFloor = floor;
        std::cout << "||";
        for (int pos = 1; pos <= elevatorPositions.size(); ++pos) {

            if (elevatorPositions[pos - 1] == floor) {
                std::cout << "[" << lifts[pos - 1].getPass() << "]|";
            }
            else {
                std::cout << "   |";
            }
        }
        std::cout << "|";
        std::cout << "   (";
        int i = numFloors;
        while (true) {
            int y = 0;
            while (y < destinationFloors[floor - 1].size())
            {
                std::cout << destinationFloors[floor - 1][y];
                if (y < destinationFloors[floor - 1].size() - 1) {
                    std::cout << ",";
                }
                y++;
            }
            i--;
            break;
        }
        std::cout << ")";
        std::cout << "\n||-";
        int z = 0;
        while (z < numElevators) {
            if (z == numElevators - 1) {
                std::cout << " -||";
                break;
            }
            else {
                std::cout << " -+-";
            }
            z++;
        }
        std::cout << std::endl;
    }

    // Нижняя рамка
    std::cout << "++-----------------++\n";
}
#pragma endregion
#pragma region Вывод состояний
std::vector<Elevator> displayInitialState(char state, std::vector<Elevator> lifts)
{
#pragma region Создаем количество Лифтов
    for (int i = 0; i < numElevators; ++i) {
        Elevator lift(elevatorCapacity, 0, destinationFloors[rand() % numFloors]);//генерируем случайные векторы для каждого лифта в пределах допустимых этажей.
        lifts.push_back(lift);
    }
#pragma endregion
    if (state == 'y')
    {
#pragma region Определяем позиции лифтов
        std::vector<int> lifts_pos(numElevators);
        for (int i = 0; i < numElevators; ++i) {//Заполняем стартовые позиции лифтов
            lifts_pos[i] = (lifts[i].getCurrentPosition() + 1);
        }
#pragma endregion
        std::wcout << L"\n";
        std::wcout << L"==================\n";
        std::wcout << L"   INITIAL STATE  \n";
        std::wcout << L"==================\n";
        std::wcout << L"\n";
        displayElevatorPositions(lifts_pos, lifts);
    }
    return lifts;
}
std::vector<Elevator> processing(std::vector<Elevator> lifts) {
    bool validInput = false;
    std::string showInitialState;
    while (!validInput) {
        // Очистка буфера ввода
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // Чтение строки
        std::cout << "\nShow all steps of the simulation? (\"all steps\"/\"end result\"): ";
        std::getline(std::cin, showInitialState);

        // Преобразование в нижний регистр
        std::transform(showInitialState.begin(), showInitialState.end(), showInitialState.begin(), [](unsigned char c) { return std::tolower(c); });

        if (showInitialState == "all steps") {
            // Делать что-то для "all steps"
            detail = true;
            break;
        }
        else if (showInitialState == "end result") {
            // Делать что-то для "end result"
            detail = false;
            break;
        }
        else {
            std::cout << "Invalid input. Please enter \"all steps\" or \"end result\"." << std::endl;
        }
    }
    int empty = 0;//Счетчик пустых векторов
    int z = 1;
    while (true)
    {
        int i = 0;
        std::cout << "\n== = Simulation Step " << z << " == =\n";
#pragma region Определяем позиции лифтов
        std::vector<int> lifts_pos(numElevators);
        for (int i = 0; i < numElevators; ++i) {//Заполняем стартовые позиции лифтов
            lifts_pos[i] = (lifts[i].getCurrentPosition());
        }
#pragma endregion
        while (i < lifts.size()) {
            int w = 0;
            int vector = lifts[i].getVector();
            int currentFloor = lifts[i].getCurrentPosition();
            if (i < numFloors) {
                if (destinationFloors[i].size() > 1) {
                    int nextFloor = destinationFloors[i].back();
                    lifts[i].getNextDestination(); // Смена позиции лифта
                }
                if (destinationFloors[i].size() == 0)
                {
                    int v = 0;
                    while (destinationFloors[i].size() == 0 && v < destinationFloors.size())
                    {
                        if (destinationFloors[v].size() != 0) {
                            lifts[i].addNewDestination(destinationFloors[v].back());
                            destinationFloors[v].pop_back();
                            if (destinationFloors[v].size() == 1) {
                                destinationFloors[v].clear();
                            }
                            break;
                        }
                        v++;
                    }
                }
                else {
                    lifts[i].addNewDestination(destinationFloors[i].back());
                }
                if (destinationFloors[i].size() != 0) {
                    destinationFloors[i].pop_back();
                }
            }
            lifts_pos.clear();
            lifts_pos.resize(numElevators);
            for (int i = 0; i < numElevators; ++i) {//Заполняем стартовые позиции лифтов
                lifts_pos[i] = (lifts[i].getCurrentPosition() + 1);
            }

            lifts[i].addPass(rand() % elevatorCapacity);
            vector = lifts[i].getVector();
            empty = 0;
            for (int i = 0; i < destinationFloors.size(); ++i) {//Заполняем стартовые позиции лифтов
                if (destinationFloors[i].empty()) {
                    empty++;
                }
            }
            if (empty != destinationFloors.size()) {
                std::cout << "\nA person reached their destination floor " << lifts[i].getCurrentPosition() << " with elevator " << i << ".\n";
            }
            else {

            }
#pragma region Определяем позиции лифтов
            std::vector<int> lifts_pos(numElevators);
            for (int i = 0; i < numElevators; ++i) {//Заполняем стартовые позиции лифтов
                lifts_pos[i] = (lifts[i].getCurrentPosition());
            }
#pragma endregion
            i++;
        }
        empty = 0;
        for (int i = 0; i < destinationFloors.size(); ++i) {//Заполняем стартовые позиции лифтов
            if (destinationFloors[i].empty()) {
                empty++;
            }
        }
        if (empty == destinationFloors.size()) {
            if (detail == true) {
                displayElevatorPositions(lifts_pos, lifts);
                std::wcout << L"\nElevators:\n";
                for (int i = 0; i < numElevators; ++i) {//Заполняем стартовые позиции лифтов
                    lifts[i].pushPass();
                }
                for (int i = 0; i < numElevators; ++i) {
                    std::wcout << i << ": " << lifts[i].getPass() << "\n";
                }
            }
            for (int i = 0; i < numElevators; ++i) {//Заполняем стартовые позиции лифтов
                lifts[i].pushPass();
            }
            std::wcout << L"\n";
            std::wcout << L"\n";
            std::wcout << L"==================\n";
            std::wcout << L"    FINAL STATE   \n";
            std::wcout << L"==================\n";
            std::wcout << L"\n";
            for (int i = 0; i < numElevators; ++i) {//Заполняем стартовые позиции лифтов
                lifts[i].pushPass();
            }
            displayElevatorPositions(lifts_pos, lifts);
            std::wcout << L"Simulation done in " << z << " steps!";
            break;
        }
        else {
            if (detail == true) {
#pragma region Определяем позиции лифтов
                std::vector<int> lifts_pos(numElevators);
                for (int i = 0; i < numElevators; ++i) {//Заполняем стартовые позиции лифтов
                    lifts_pos[i] = (lifts[i].getNextDestination() + 1);
                }
#pragma endregion
                displayElevatorPositions(lifts_pos, lifts);
                std::wcout << L"\nElevators:\n";
                for (int i = 0; i < numElevators; ++i) {//Заполняем стартовые позиции лифтов
                    std::wcout << i << ": " << lifts[i].getPass() << "\n";
                }
            }
            for (int i = 0; i < numElevators; ++i) {//Заполняем стартовые позиции лифтов
                lifts[i].pushPass();
            }

        }
        z++;
    }
    return lifts;
}
#pragma endregion

int main() {
    char showInitialState[10]; // Переменная вывода Инициализации
    char showInitialStateSt[10]; // Переменная вывода Инициализации
    bool validInput = false;
    bool validInputSt = false;
    std::vector<Elevator> lifts;
#pragma region Приветствие
    //Приветствие
    std::wcout << L"____________________\n";
    std::wcout << L" ____   ____   ___  \n";
    std::wcout << L"|____  |____  |___| \n";
    std::wcout << L"|____   ____| |     \n";
    std::wcout << L"____________________\n";
    std::wcout << L"\n";
    std::wcout << L"Welcome to the\n";
    std::wcout << L"Elevator Simulation Program!\n";
    std::wcout << L"\n";
#pragma endregion
#pragma region Сбор Данных
    try {
#pragma region Получаем название отеля
        // Получаем название отеля
        std::cout << "Enter the name of the hotel:\n";
        std::getline(std::cin, hotelName);

        // Обрезаем длинные имена отеля для упрощения
        if (hotelName.length() > 20) {
            hotelName = hotelName.substr(0, 20);
        }

        // Преобразуем имя отеля в заглавные буквы
        for (char& c : hotelName) {
            c = std::toupper(c);
        }

#pragma endregion
#pragma region Запрос количества этажей
        // Запрос количества этажей с проверкой ограничений
        std::cout << "Enter the number of floors in hotel " << hotelName << "(between 3 and 10): ";
        while (true) {
            std::cin >> numFloors;
            if (numFloors >= 3 && numFloors <= 10) {
                break;
            }
            else {
                std::cout << "Invalid input, the number of Floors must be between 3 and 10!\nEnter number of Floors: ";
                std::cin.clear(); // Очистка флагов ошибок ввода
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка ввода до символа новой строки
            }
        }
#pragma endregion
#pragma region Запрос количества лифтов
        // Запрос количества лифтов с проверкой ограничений
        std::cout << "Enter the number of elevators (between 1 and 5): ";
        while (true) {
            std::cin >> numElevators;
            if (numElevators >= 1 && numElevators <= 5) {
                break;
            }
            else {
                std::cout << "Wrong input, the number of elevators must be between 1 and 5!\nEnter the number of elevators: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
#pragma endregion    
#pragma region Запрос вместимости лифтов
        // Запрос вместимости лифтов с проверкой ограничений
        std::cout << "Enter the capacity of elevators in hotel " << hotelName << " (The capacity must be between 1 and 9 people): ";
        while (true) {
            std::cin >> elevatorCapacity;
            if (elevatorCapacity >= 1 && elevatorCapacity <= 9) {
                break;
            }
            else {
                std::cout << "Wrong input, the capacity of elevators must be between 1 and 9 person(s)!\nEnter the capacity of elevators: ";
                std::cin.clear();
                //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
#pragma endregion
#pragma region Получаем количество людей, ожидающих лифт на каждом этаже
        // Запрос вместимости лифтов с проверкой ограничений
        std::cout << "Enter the number of people waiting on each floor: ";
        while (true) {
            std::cin >> numWaitingPeople;
            if (numWaitingPeople >= 2 && numWaitingPeople <= 20) {
                break;
            }
            else {
                std::cout << "Wrong input, the number of people waiting on each floor must be between 2 and 20!\n ";
                std::cin.clear();
                //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
#pragma endregion
#pragma region  Запрос направления людей на каждом этаже
        destinationFloors.resize(numFloors);
        // Запрос этажей назначения для каждого человека на каждом этаже в виде списка
        for (int i = 0; i < numFloors; ++i) {
            std::string input;
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max()); // Clear input buffer
            std::cout << "Enter the destination floors of the people [floor: " << i << "]:\n> ";
            std::cin >> input;

            std::stringstream ss(input);
            std::string floor;
            bool validInput = true;

            while (std::getline(ss, floor, ',')) {
                int destinationFloor = std::stoi(floor);

                if (destinationFloor < 0 || destinationFloor >= numFloors) {
                    std::cout << "Wrong input, the destination floor " << destinationFloor << " is out of range (0 to " << numFloors - 1 << ")!\n";
                    validInput = false;
                    break;
                }
                else if (destinationFloor == i) {
                    std::cout << "Wrong input, the destination floor cannot be the current floor!\n";
                    validInput = false;
                    break;
                }
                else {
                    destinationFloors[i].push_back(destinationFloor);
                }
            }

            if (!validInput) {
                destinationFloors[i].clear();
                --i; // Repeat input for this floor
            }
        }
#pragma endregion
#pragma region Запрос отображения первичной иллюстрации
        while (!validInput) {
            std::cout << "Do you want to see the initial state illustration? (\"yes\"/\"no\"): ";
            std::cin >> showInitialState;

            for (int i = 0; i < strlen(showInitialState); ++i) {
                showInitialState[i] = tolower(showInitialState[i]);
            }

            if (strcmp(showInitialState, "yes") == 0) {
                lifts = displayInitialState('y', lifts);
                break;
            }
            else if (strcmp(showInitialState, "no") == 0) {
                lifts = displayInitialState('n', lifts);
                break;
            }
            else {
                std::cout << "Invalid input. Please enter \"yes\" or \"no\"." << std::endl;
            }
        }
#pragma endregion
#pragma region Запрос отображения
        while (!validInputSt) {
            //int numeric = numFloors;
            std::cout << "\nStart the simulation? (\"start\"/\"skip\"): ";
            std::cin >> showInitialStateSt;
            for (int i = 0; i < strlen(showInitialStateSt); ++i) {
                showInitialStateSt[i] = tolower(showInitialStateSt[i]);
            }
            //numFloors = numeric;//FIX
            if (strcmp(showInitialStateSt, "start") == 0) {
                lifts = processing(lifts);
                system("pause");
                return 0;
                break;
            }
            else if (strcmp(showInitialStateSt, "skip") == 0) {
                exit(0);
            }
            else {
                std::cout << "Invalid input. Please enter \"start\" or \"skip\"." << std::endl;
            }
        }
#pragma endregion
    }
    catch (const std::bad_alloc & e) {
        // Обработка исключения в случае неудачного выделения памяти
        std::cerr << "Out of memory! Progran terminated!\n" << e.what() << std::endl;
        return -1;
    }
#pragma endregion
}