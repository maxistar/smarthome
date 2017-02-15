// button debouncer, how many ms to debounce, 5+ ms is usually plenty
#define DEBOUNCE 50  

#define ANIMATION_STEP 10

//how long to go into setup mode
#define LONG_PRESS 5000  

/**
 *
 *
 */
class BaseDimmer {
    protected:
    int buttonPin;

    //debouncing staff
    char isDebouncing;
    long debounceLastTime;  

    // we will track if a button is just pressed, just released, or 'currently pressed' 
    char pressed; 

    long buttonPressedTime;

    void checkSwitches();
    
    virtual void onButtonPressed();
    virtual void onButtonReleased();
    virtual void onButtonKeepsPressed();
    
    void setButton(int buttonPin);

    public:
    void loop();
    void setup();
    //BaseDimmer(int buttonPin);
};
