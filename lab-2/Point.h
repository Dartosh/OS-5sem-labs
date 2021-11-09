#include <string>

using namespace std;
class Point {
public:
    int x;
    int y;
    bool deletable = false;
    string initialized_time;
    string writed_time;
    Point(int x_value, int y_value, string initialized_time) {
        this -> x = x_value;
        this -> y = y_value;
        this -> initialized_time = initialized_time;
    }

    ~Point() {
        if (!this -> deletable)
            cout << "The point didn't logged yet\n";
    }

    string log_value() {
        time_t now = time(0);
        this -> writed_time = ctime(&now);
        string record = to_string(this -> x) + " " + to_string(this -> y) + " init: " + this -> initialized_time + " writed:" + this -> writed_time + "\n";
        set_deletable();
        return record;
    }

private:
    void set_deletable() {
        this -> deletable = true;
    }
};

