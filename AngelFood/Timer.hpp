
class Timer
{
public:
    Timer() : time_to_wait(1.0f), counter(0.0f), running(false) {}
    Timer(float time) : time_to_wait(time), counter(0.0f), running(false) {}

    void start() 
    {
        counter = 0.0f;
        running = true;
    }

    bool update(float delta) 
    {
        if (!running)
            return false;

        counter += delta;

        if (counter >= time_to_wait)
        {
            running = false;
            return true;
        }

        return false;
    }

    void stop()
    {
        running = false;
    }

    bool is_running()
    {
        return running;
    }

private:
    float time_to_wait;
    float counter;

    bool running;
};
