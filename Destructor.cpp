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