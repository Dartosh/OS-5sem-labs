#include <iostream>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <string>
#include <ctime>
#include <memory>
#include "Point.h"

using namespace std;

condition_variable variable;
mutex cv_m;
bool notified_result_listener = false;
bool notified_logger = false;
bool done = false;
int current_function_value = 1;
int current_argument_value = 1;
fstream function_result;


void init_file(char filename[]) {
    function_result.open(filename,  fstream::in | fstream::out | fstream::trunc);
    function_result.close();
}

void write_function_result(char filename[], string result) {
    function_result.open(filename, fstream::in | fstream::out | fstream::app);
    function_result << result << "\n";
    function_result.close();
}

void result_listener() {
    const char filename[] = "../results";

    init_file(const_cast<char *>(filename));

    unique_lock<mutex> lock(cv_m);

    while (!done) {
        while (notified_result_listener) {
            write_function_result(const_cast<char *>(filename), to_string(current_function_value));

            notified_result_listener = false;
            notified_logger = true;
            variable.notify_all();
        }
        variable.wait(lock);
    }
}

void factorial_calculation(int n) {
    for (; current_argument_value <= n; current_argument_value++) {
        this_thread::sleep_for(chrono::seconds(1));
        unique_lock<mutex> lock(cv_m);

        current_function_value *= current_argument_value;

        notified_result_listener = true;
        variable.notify_all();
    }

    done = true;
    variable.notify_all();
}

void log_writer() {
    const char filename[] = "../log";
    init_file(const_cast<char *>(filename));

    unique_lock<mutex> lock(cv_m);

    while (!done) {
        while (notified_logger) {
            notified_logger = false;
            time_t now = time(0);
            string initialized_time = ctime(&now);
            shared_ptr<Point> current_point(new Point(current_argument_value - 1, current_function_value, initialized_time));
            write_function_result(const_cast<char *>(filename), current_point -> log_value());
        }
        variable.wait(lock);
    }
}

int main() {
    thread factorial(factorial_calculation, rand() % 10);
    thread result(result_listener);
    thread log(log_writer);

    factorial.join();
    result.join();
    log.join();

    cout << "completed";
    return 0;
}

