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