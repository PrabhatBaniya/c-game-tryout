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