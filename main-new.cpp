#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <string>
#include <limits>
#include <ctime>
#include <random>

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else
    #define CLEAR_COMMAND "clear"
#endif

using namespace std;

// Global time tracking
int hour = 0;
int dayCount = 1;  // Start at day 1

void clearScreen() {
    // You can replace this with printing many newlines if system call is undesirable.
    system(CLEAR_COMMAND);
}

class Cat {
private:
    int thirst;
    int hunger;
    int happiness;
    int energy;
    int cleanliness;
    int didntSleepLastFourHours;
    bool sick;
    int ageDays;

    // Random engine for events
    mt19937 rng;

    // Clamp all stats to [0,100]
    void clampStats() {
        thirst = max(0, min(100, thirst));
        hunger = max(0, min(100, hunger));
        happiness = max(0, min(100, happiness));
        energy = max(0, min(100, energy));
        cleanliness = max(0, min(100, cleanliness));
    }

    // Check if cat becomes sick based on thresholds
    void checkForSickness() {
        if (!sick) {
            // If hunger or thirst too high, or cleanliness too low, chance to get sick
            if (hunger >= 80 || thirst >= 80 || cleanliness <= 20) {
                uniform_int_distribution<int> dist(1, 100);
                int roll = dist(rng);
                // 30% chance if in bad condition
                if (roll <= 30) {
                    sick = true;
                    cout << name << " has become sick! You should take it to the vet soon.\n";
                }
            }
        }
    }

    // Random daily event
    void randomDailyEvent() {
        uniform_int_distribution<int> distEvent(1, 100);
        int roll = distEvent(rng);
        if (roll <= 20) {
            // 20% chance of something happening
            uniform_int_distribution<int> typeDist(1, 3);
            int type = typeDist(rng);
            switch (type) {
                case 1:
                    cout << name << " found a new toy and seems happier!\n";
                    happiness += 15;
                    break;
                case 2:
                    cout << name << " got a bit dirty playing outside.\n";
                    cleanliness -= 20;
                    break;
                case 3:
                    cout << name << " chased a bug and got slightly tired.\n";
                    energy -= 10;
                    break;
            }
            clampStats();
        }
    }

public:
    string name;

    Cat()
        : thirst(50), hunger(50), happiness(50), energy(50),
          cleanliness(80), didntSleepLastFourHours(0),
          sick(false), ageDays(0)
    {
        // Seed random engine with time + address
        rng.seed(static_cast<unsigned long>(time(nullptr)) ^ reinterpret_cast<uintptr_t>(this));
    }

    void hourlyUpdate() {
        // Called whenever an hour passes
        ageDays += (hour == 0 ? 1 : 0);  // increment day count at midnight
        didntSleepLastFourHours++;
        // Base decay/growth per hour
        thirst += 5;
        hunger += 5;
        energy -= 5;
        cleanliness -= 2;
        happiness -= 2;

        // If sick, stats worsen faster
        if (sick) {
            thirst += 5;
            hunger += 5;
            energy -= 5;
            happiness -= 5;
        }

        clampStats();
        checkForSickness();
    }

    void status() {
        cout << "Day: " << dayCount << ", Hour: " << hour << "\n";
        cout << "Name: " << name << "\n";
        cout << "Hunger: " << hunger << "/100\n";
        cout << "Thirst: " << thirst << "/100\n";
        cout << "Energy: " << energy << "/100\n";
        cout << "Happiness: " << happiness << "/100\n";
        cout << "Cleanliness: " << cleanliness << "/100\n";
        cout << "Age (days): " << ageDays << "\n";
        cout << "Sick: " << (sick ? "Yes" : "No") << "\n";
    }

    void asciiArt() {
        // Different art if low energy or unhappy
        if (energy < 20) {
            cout << "  /\\_/\\  \n";
            cout << " ( -.- ) zZz\n";
            cout << " /     \\\\ \n";
        }
        else if (happiness >= 50) {
            cout << "  /\\_/\\  \n";
            cout << " ( ^‿^ ) \n";
            cout << " /     \\\\ \n";
        } else {
            cout << "  /\\_/\\  \n";
            cout << " ( ; _ ; ) \n";
            cout << " /     \\\\ \n";
        }
    }

    void feed() {
        cout << "(=｀ω´=)っ🍗 The cat is eating...\n";
        this_thread::sleep_for(chrono::seconds(2));
        hunger -= 30;
        thirst += 10;
        cleanliness -= 5;
        cout << name << " enjoyed the meal.\n";
    }

    void drink() {
        cout << "(=^‥^=)っ💧 The cat is drinking...\n";
        this_thread::sleep_for(chrono::seconds(2));
        thirst -= 30;
        cout << name << " quenched its thirst.\n";
    }

    void sleepCat(bool manual = false) {
        // If manual: let user decide how long; if automatic (called from alive()), sleeps until energy restored somewhat.
        cout << name << " is going to sleep...\n";
        bool wakeEarly = false;
        while (energy < 80) {
            cout << "/\\_/\\\n =^.^= zZz\n";
            cout << "Sleeping...\n";
            this_thread::sleep_for(chrono::seconds(2));
            energy += 15;
            hunger += 10;
            thirst += 10;
            cleanliness -= 5;
            happiness += 5;
            didntSleepLastFourHours = 0;

            // advance hour, day if needed
            hour++;
            if (hour >= 24) {
                hour = 0;
                dayCount++;
                randomDailyEvent();
            }

            clampStats();

            if (manual) {
                cout << "Wake " << name << " early? (1 = yes, 0 = no): ";
                int ans;
                if (!(cin >> ans)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    ans = 0;
                }
                if (ans == 1) {
                    wakeEarly = true;
                }
            }
            if (wakeEarly) break;
        }
        cout << name << " woke up from sleep.\n";
    }

    void play() {
        cout << "(=ÒㅅÓ=)ノ🧶 The cat is playing...\n";
        this_thread::sleep_for(chrono::seconds(2));
        energy -= 15;
        hunger += 15;
        thirst += 10;
        happiness += 20;
        cleanliness -= 10;
        cout << name << " had fun playing with you!\n";
    }

    void pet() {
        cout << "(=^･ω･^=)♡ You pet the cat.\n";
        this_thread::sleep_for(chrono::seconds(1));
        happiness += 15;
        cout << name << " seems happier.\n";
    }

    void groom() {
        cout << "(=^･ｪ･^=)✂ Grooming the cat...\n";
        this_thread::sleep_for(chrono::seconds(2));
        cleanliness += 30;
        happiness += 10;
        cout << name << " is cleaner and looks refreshed.\n";
    }

    void vet() {
        if (!sick) {
            cout << "The vet says " << name << " is healthy.\n";
            return;
        }
        cout << "Taking " << name << " to the vet...\n";
        this_thread::sleep_for(chrono::seconds(3));
        // Recovery chance
        uniform_int_distribution<int> dist(1, 100);
        int roll = dist(rng);
        if (roll <= 80) {
            sick = false;
            energy -= 10;
            hunger += 10;
            thirst += 10;
            cleanliness -= 10;
            happiness += 10;
            cout << name << " has recovered from sickness!\n";
        } else {
            // Might remain sick
            energy -= 5;
            hunger += 5;
            thirst += 5;
            cleanliness -= 5;
            happiness -= 5;
            cout << "The treatment was not fully successful. " << name << " is still a bit unwell.\n";
        }
        clampStats();
    }

    void checkHealth() {
        cout << "Health check for " << name << ":\n";
        cout << " - Hunger: " << hunger << "/100\n";
        cout << " - Thirst: " << thirst << "/100\n";
        cout << " - Energy: " << energy << "/100\n";
        cout << " - Happiness: " << happiness << "/100\n";
        cout << " - Cleanliness: " << cleanliness << "/100\n";
        cout << " - Sick: " << (sick ? "Yes" : "No") << "\n";
        if (sick) {
            cout << "Consider taking " << name << " to the vet.\n";
        }
    }

    void changeName() {
        cout << "Enter new name for your cat: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string newName;
        getline(cin, newName);
        if (!newName.empty()) {
            name = newName;
            cout << "Cat's name changed to " << name << ".\n";
        } else {
            cout << "Name not changed.\n";
        }
    }

    bool alive() {
        // If extremely tired and hasn’t slept recently, force sleep
        if (energy <= 20 && didntSleepLastFourHours >= 4) {
            cout << name << " is extremely tired and falls asleep automatically.\n";
            sleepCat(false);
        }

        // Death conditions
        if (hunger >= 100) {
            cout << name << " died from hunger.\n";
            return false;
        }
        if (thirst >= 100) {
            cout << name << " died from thirst.\n";
            return false;
        }
        if (energy <= 0) {
            cout << name << " died from exhaustion.\n";
            return false;
        }
        if (happiness <= 0) {
            cout << name << " died from sadness.\n";
            return false;
        }
        if (cleanliness <= 0) {
            cout << name << " died from infection due to poor cleanliness.\n";
            return false;
        }
        // If sick and not treated for too long, gradually worsen:
        // Could be implemented by tracking days sick, but skipping for now.

        return true;
    }

    void nothing() {
        cout << name << " is idling...\n";
        this_thread::sleep_for(chrono::seconds(2));
        energy += 5;
        happiness -= 5;
        hunger += 10;
        thirst += 10;
        cleanliness -= 5;
    }
};

void waitForEnter() {
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
    // Seed for other parts if needed
    srand(static_cast<unsigned>(time(nullptr)));

    Cat cat;
    cout << "Name your cat: ";
    getline(cin, cat.name);

    while (true) {
        if (!cat.alive()) break;
        if (hour >= 24) {
            hour = 0;
            dayCount++;
        }

        clearScreen();
        cat.asciiArt();
        cat.status();

        cout << "\nWhat do you want to do?\n";
        cout << "1 - Feed the cat\n";
        cout << "2 - Give the cat some drink\n";
        cout << "3 - Play with the cat\n";
        cout << "4 - Pet the cat\n";
        cout << "5 - Groom the cat\n";
        cout << "6 - Put the cat to sleep\n";
        cout << "7 - Take the cat to the vet\n";
        cout << "8 - Check health/vitals\n";
        cout << "9 - Change cat's name\n";
        cout << "0 - Do nothing\n";
        cout << "q - Quit simulation\n";
        cout << "Choose an action: ";

        string input;
        cin >> input;
        if (!cin) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            input = "invalid";
        }

        bool advancedHour = true;
        if (input == "q" || input == "Q") {
            cout << "Exiting simulation. Goodbye!\n";
            break;
        }

        int action = -1;
        try {
            action = stoi(input);
        } catch (...) {
            action = -1;
        }

        switch (action) {
            case 1:
                cat.feed();
                break;
            case 2:
                cat.drink();
                break;
            case 3:
                cat.play();
                break;
            case 4:
                cat.pet();
                advancedHour = false; // petting is quick
                break;
            case 5:
                cat.groom();
                break;
            case 6:
                // Manual sleep
                cat.sleepCat(true);
                // sleepCat advances hours internally
                advancedHour = false;
                break;
            case 7:
                cat.vet();
                break;
            case 8:
                cat.checkHealth();
                advancedHour = false;
                break;
            case 9:
                cat.changeName();
                advancedHour = false;
                break;
            case 0:
                cat.nothing();
                break;
            default:
                cout << "Invalid input. Doing nothing this hour.\n";
                cat.nothing();
                break;
        }

        // Advance hour if applicable
        if (advancedHour) {
            hour++;
            // After advancing hour, apply hourlyUpdate in cat
            if (hour >= 24) {
                hour = 0;
                dayCount++;
                // On day rollover, trigger random event
                // randomDailyEvent is called in hourlyUpdate when hour==0
            }
            cat.hourlyUpdate();
        }

        // Let user see result before clearing
        waitForEnter();
    }

    cout << "Simulation ended. Thank you for playing.\n";
    return 0;
}
