//todo

//includes

//defines

//functions
struct TimerX
{
	char timerID;
	char prescaler;
	char periodRegister;
}

struct PWM_timer
{
	struct TimerX;
	char OCMbits;		//might be unnecessary
	char RS;			//future duty cycle
	char R;				//current duty cycle
}

struct TimerX TimerX_init(char ID, char PS, char N)
{
	
}

/*struct MyObj{
    double x, y;
};

struct MyObj foo(){
    struct MyObj a;

    a.x = 10;
    a.y = 10;

    return a;
}     */