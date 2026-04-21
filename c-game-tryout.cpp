#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <string>
#include <algorithm>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define CLEAR "cls"
    bool kbhit_check() { return _kbhit(); }
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
    #define CLEAR "clear"

    bool kbhit_check() {
        struct termios oldt, newt;
        int ch, oldf;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        if (ch != EOF) { ungetc(ch, stdin); return true; }
        return false;
    }
#endif

using namespace std;

const int WIDTH    = 50;
const int HEIGHT   = 20;
const int PIPE_GAP = 6;



//  TOPIC 1 : ABSTRACT CLASS

class GameObject {
public:
    int x, y;

    GameObject(int x, int y) : x(x), y(y) {}

    virtual void update()           = 0;   // pure virtual — no body here
    virtual char getSymbol() const  = 0;   // pure virtual — no body here

    // ── TOPIC 3 : DESTRUCTOR
    
    virtual ~GameObject() {}               // virtual destructor
};



//  TOPIC 2 : BASIC INHERITANCE
//  TOPIC 5 : PARENT-CHILD CLASS

class Bird : public GameObject {           // ← basic inheritance

private:
    float velocity;
    static int totalFlaps;                 // shared across ALL Bird objects

public:
    // TOPIC 5 — parent-child constructor chaining
    Bird()
        : GameObject(5, HEIGHT / 2),       // ① call parent ctor first
          velocity(0)                      // ② then initialise child fields
    {}

    // ── TOPIC 3 : DESTRUCTOR 
    
    ~Bird() {}

    void flap() {
        velocity = -1.5f;
        totalFlaps++;
    }

    // ── TOPIC 8 : OVERRIDING 
    
    void update() override {
        velocity += 0.5f;                  // gravity pulls bird down
        y += (int)velocity;
    }

    // ── TOPIC 8 : OVERRIDING 
    char getSymbol() const override { return '>'; }

    bool isOutOfBounds() const {
        return y <= 0 || y >= HEIGHT - 1;
    }

    static int getTotalFlaps() { return totalFlaps; }
};
int Bird::totalFlaps = 0;



//  TOPIC 4 : INHERITANCE HIERARCHY

class Pipe : public GameObject {           // ← part of the hierarchy

private:
    int  gapTop;
    bool passed;

public:
    // TOPIC 5 — parent-child constructor chaining (same pattern as Bird)
    Pipe(int startX)
        : GameObject(startX, 0),           // ① parent ctor
          passed(false)                    // ② child field
    {
        gapTop = rand() % (HEIGHT - PIPE_GAP - 4) + 2;
    }

    // ── TOPIC 3 : DESTRUCTOR 
    ~Pipe() {}

    // ── TOPIC 8 : OVERRIDING 
    void update() override { x--; }
    char getSymbol() const override { return '|'; }

    bool hasPassed(int birdX) {
        if (!passed && x < birdX) { passed = true; return true; }
        return false;
    }

    bool collidesWith(int birdX, int birdY) const {
        if (birdX == x)
            return (birdY <= gapTop || birdY >= gapTop + PIPE_GAP);
        return false;
    }

    int  getGapTop()    const { return gapTop; }
    int  getGapBottom() const { return gapTop + PIPE_GAP; }
    bool isOffScreen()  const { return x < 0; }
};



//  TOPIC 6 : OVERLOADED FUNCTIONS

class ScoreTracker {
protected:
    int score;
    static int highScore;

public:
    ScoreTracker() : score(0) {}

    void addPoint() {
        score++;
        if (score > highScore) highScore = score;
    }

    // overload 1 — no arguments
    void showScore() const {
        cout << "Score: " << score;
    }

    // overload 2 — custom label
    void showScore(const string& label) const {
        cout << label << score;
    }

    // overload 3 — label + toggle high score display
    void showScore(const string& label, bool showHigh) const {
        cout << label << score;
        if (showHigh) cout << "   High Score: " << highScore;
    }

    static int getHighScore() { return highScore; }

    // ── TOPIC 3 : DESTRUCTOR 
    virtual ~ScoreTracker() {}
};
int ScoreTracker::highScore = 0;



//  TOPIC 7 : MULTIPLE INHERITANCE

class Game : public ScoreTracker {         // ← multiple inheritance

private:
    Bird         bird;
    vector<Pipe> pipes;
    bool         gameOver;
    int          frameCount;

    // ── TOPIC 10 : POINTER 
    
    GameObject* lastUpdated;

    void spawnPipe() {
        pipes.push_back(Pipe(WIDTH - 1));
    }

    void drawFrame() {
        vector<string> screen(HEIGHT, string(WIDTH, ' '));

        for (auto& pipe : pipes) {
            for (int row = 0; row < HEIGHT; row++) {
                bool inGap = (row > pipe.getGapTop() &&
                              row < pipe.getGapBottom());
                if (!inGap && pipe.x >= 0 && pipe.x < WIDTH)
                    screen[row][pipe.x] = '#';
            }
        }

        if (bird.y >= 0 && bird.y < HEIGHT)
            screen[bird.y][bird.x] = bird.getSymbol();

        system(CLEAR);
        cout << " FLAPPY BIRD  [SPACE = flap]\n";
        cout << "+" << string(WIDTH, '-') << "+\n";
        for (auto& row : screen) cout << "|" << row << "|\n";
        cout << "+" << string(WIDTH, '-') << "+\n";

        // TOPIC 6 — calling overload 3 of showScore()
        showScore(" Score: ", true);
        cout << "   Total Flaps: " << Bird::getTotalFlaps() << "\n";
    }

    void update() {
        frameCount++;
        if (frameCount % 25 == 0) spawnPipe();

        // ── TOPIC 9 : POLYMORPHISM 
        
        lastUpdated = &bird;               // TOPIC 10: pointer assignment
        lastUpdated->update();             // TOPIC 9: → Bird::update()

        for (auto& pipe : pipes) {
            // ── TOPIC 9 : POLYMORPHISM 
            
            lastUpdated = &pipe;           // TOPIC 10: pointer re-assigned
            lastUpdated->update();         // TOPIC 9: → Pipe::update()

            if (pipe.hasPassed(bird.x)) addPoint();
        }

        pipes.erase(
            remove_if(pipes.begin(), pipes.end(),
                [](const Pipe& p) { return p.isOffScreen(); }),
            pipes.end()
        );

        if (bird.isOutOfBounds()) { gameOver = true; return; }
        for (auto& pipe : pipes)
            if (pipe.collidesWith(bird.x, bird.y)) { gameOver = true; return; }
    }

public:
    // ── TOPIC 3 : DESTRUCTOR 
    
    ~Game() {}

    Game()
        : gameOver(false),
          frameCount(0), lastUpdated(nullptr)
    {
        score = 0;          // score belongs to parent ScoreTracker — assign in body
        srand(time(0));
        spawnPipe();
    }

    void run() {
        cout << "\n  === FLAPPY BIRD (Text Edition) ===\n";
        cout << "  Press SPACE to flap\n";
        cout << "  Press ENTER to start...\n";
        cin.get();

        while (!gameOver) {
            if (kbhit_check()) {
                char ch = getchar();
                if (ch == ' ') bird.flap();
            }
            update();
            drawFrame();
            this_thread::sleep_for(chrono::milliseconds(100));
        }

        system(CLEAR);
        cout << "\n  ╔══════════════════════════╗\n";
        cout << "  ║       GAME OVER!         ║\n";
        cout << "  ╠══════════════════════════╣\n";
        cout << "  ║  Your Score  : " << score                   << "\n";
        cout << "  ║  High Score  : " << ScoreTracker::getHighScore() << "\n";
        cout << "  ║  Total Flaps : " << Bird::getTotalFlaps()   << "\n";
        cout << "  ╚══════════════════════════╝\n";
    }
};


//  MAIN

int main() {
    char again = 'y';
    while (again == 'y' || again == 'Y') {
        Game game;                         // constructed here
        game.run();
        // ← game destroyed here; destructors chain automatically
        cout << "\n  Play again? (y/n): ";
        cin >> again;
        cin.ignore();
    }

    cout << "\n  Thanks for playing! Final High Score: "
         << ScoreTracker::getHighScore() << "\n";
    return 0;
}