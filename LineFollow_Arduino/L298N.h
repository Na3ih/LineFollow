#ifndef L298N_h
#define L298N_h

class L298N
{
  private:
    int ENABLE;
    int IN1;
    int IN2;

  public:  
    L298N(int ENABLE_pin, int IN1_pin, int IN2_pin);     // constructor
    void GPIO_Config();
	void EnableMotor(void);
    void DisableMotor(void);
    void Forward(void);
    void Backward(void);
    void Brake(void);
    void Coast(void);
    void TurnLeft(int ms);
    void TurnRight(int ms);
    void SetSpeed(int pwm);
};


#endif