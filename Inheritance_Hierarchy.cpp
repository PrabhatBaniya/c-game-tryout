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