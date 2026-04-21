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