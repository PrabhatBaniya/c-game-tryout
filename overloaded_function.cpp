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