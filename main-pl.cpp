#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else
    #define CLEAR_COMMAND "clear"
#endif

using namespace std;

int hour = 0;

void clearScreen() {
    system(CLEAR_COMMAND);
}

class Cat {
private:
    int thirst;
    int hunger;
    int happiness;
    int energy;
    int didntSleepLastFourHours = 0;

public:
    string name;

    Cat() {
        thirst = 50;
        hunger = 50;
        happiness = 50;
        energy = 50;
    }

    void status() {
        didntSleepLastFourHours++;

        thirst = max(0, min(100, thirst));
        hunger = max(0, min(100, hunger));
        happiness = max(0, min(100, happiness));
        energy = max(0, min(100, energy));

        cout << "Godzina: " << hour << endl;
        cout << "Imię: " << name << endl;
        cout << "Głód: " << hunger << endl;
        cout << "Pragnienie: " << thirst << endl;
        cout << "Energia: " << energy << endl;
        cout << "Szczęście: " << happiness << endl;
    }

    void asciiArt() {
        if (happiness >= 50) {
            cout << "  /\\_/\\  " << endl;
            cout << " ( ^‿^ ) " << endl;
            cout << " /     \\\\" << endl;
        } else {
            cout << "  /\\_/\\  " << endl;
            cout << " ( ; _ ; ) " << endl;
            cout << " /     \\\\" << endl;
        }
    }

    void eat() {
        cout << "(=｀ω´=)っ🍗" << endl;
        cout << "Kot je..." << endl;
        this_thread::sleep_for(chrono::seconds(3));
        hunger -= 25;
        thirst += 10;
    }

    void drink() {
        cout << "(=^‥^=)っ🍺" << endl;
        cout << "Kot pije..." << endl;
        this_thread::sleep_for(chrono::seconds(3));
        thirst -= 25;
    }

    void sleep() {
        bool instantWakeUp;

        while (energy < 70) {
            cout << "/\\_/\\\n =^.^= zZz" << endl;
            cout << "Kot śpi..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            energy += 10;
            hunger -= 5;
            thirst -= 5;
            happiness += 5;

            hour++;
            clearScreen();

            cout << "Obudzić go? (1 = tak, 0 = nie): ";
            cin >> instantWakeUp;
            if (instantWakeUp) {
                break;
            }
        }

        happiness -= 10;
        energy -= 5;
        didntSleepLastFourHours = 0;
    }

    void play() {
        cout << "(=ÒㅅÓ=)ノ🧶" << endl;
        cout << "Kot się bawi..." << endl;
        this_thread::sleep_for(chrono::seconds(3));
        energy -= 5;
        hunger += 5;
        happiness += 10;
    }

    bool alive() {
        if (energy <= 30 && didntSleepLastFourHours >= 4) {
            sleep();
        }

        if (hunger >= 100) {
            cout << name << " umarł z głodu." << endl;
            return false;
        }
        if (thirst >= 100) {
            cout << name << " umarł z pragnienia." << endl;
            return false;
        }
        if (energy <= 0) {
            cout << name << " umarł z wyczerpania." << endl;
            return false;
        }
        if (happiness <= 0) {
            cout << name << " umarł z depresji." << endl;
            return false;
        }

        return true;
    }

    void nothing() {
        cout << "Nic nie robi..." << endl;
        this_thread::sleep_for(chrono::seconds(3));
        energy += 5;
        happiness -= 10;
        hunger += 10;
        thirst += 10;
    }
};

int main() {
    Cat cat;
    int action;

    cout << "Podaj imię kota: ";
    getline(cin, cat.name);

    while (cat.alive()) {
        if (hour >= 24) hour = 0;

        cat.asciiArt();
        cat.status();

        cout << "\nCo chcesz zrobić?\n";
        cout << "1 - Nakarm kota" << endl;
        cout << "2 - Daj kotu coś do picia" << endl;
        cout << "3 - Pobaw się z kotem" << endl;
        cout << "4 - Nic nie rób" << endl;
        cout << "Wybierz akcję: ";
        cin >> action;

        switch (action) {
            case 1:
                cat.eat();
                hour++;
                break;
            case 2:
                cat.drink();
                hour++;
                break;
            case 3:
                cat.play();
                hour++;
                break;
            case 4:
                cat.nothing();
                hour++;
                break;
            default:
                cout << "Nieprawidłowy wybór. Kot nic nie robi." << endl;
                cat.nothing();
                hour++;
                break;
        }

        clearScreen();
    }

    return 0;
}
