#include <iostream>
#include <condition_variable>
#include <memory>
#include <thread>
#include <fstream>
#include <string>
#include <ctime>
#include "Point.h"
#include <vector>

using namespace std;

condition_variable variable;
condition_variable variable_log;
condition_variable variable_res;
mutex cv_m;
bool done = false;
int current_function_value = 1;
int current_argument_value = 1;
const char filename_results[] = "../results";
shared_ptr<Point> current_point;
fstream function_result;

void init_file(char filename[]) {
    function_result.open(filename,  fstream::in | fstream::out | fstream::trunc);
    function_result.close();
}

void write_result(char filename[], string result) {
    function_result.open(filename, fstream::in | fstream::out | fstream::app);
    function_result << result << "\n";
    function_result.close();
}

void result_listener() {
    init_file(const_cast<char *>(filename_results));

    unique_lock<mutex> lock(cv_m);

    while (!done) {
        write_result(const_cast<char *>(filename_results), to_string(current_function_value));
        variable_res.notify_all();
        variable.wait(lock);
    }
}

void fact_calc(int n) {
    for (; current_argument_value <= n; current_argument_value++) {
        this_thread::sleep_for(chrono::seconds(1));
        unique_lock<mutex> lock(cv_m);

        current_function_value *= current_argument_value;

        variable.notify_all();
    }

    done = true;
    variable.notify_all();
}

void logger() {
    const char filename[] = "../log";
    init_file(const_cast<char *>(filename));

    unique_lock<mutex> lock(cv_m);

    while (!done) {
        time_t now = time(0);
        string initialized_time = ctime(&now);
        this_thread::sleep_for(chrono::seconds(1));

        current_point = std::make_shared<Point>(current_argument_value - 1, current_function_value,
                                                initialized_time);
        write_result(const_cast<char *>(filename), current_point->log_value());
        variable_log.notify_all();
        variable.wait(lock);
    }
}

int main() {
    thread factorial(fact_calc, 7);
    thread result(result_listener);
    thread log(logger);

    factorial.join();
    result.join();
    log.join();

    cout << "completed";
    return 0;
}

