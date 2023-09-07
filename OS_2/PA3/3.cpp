#include <iostream>
#include <ctime>

int main() {
    std::time_t current_time = std::time(nullptr);
    std::tm* local_time = std::localtime(&current_time);
    char time_string[9]; // buffer to hold the formatted time string

    std::strftime(time_string, sizeof(time_string), "%H:%M:%S", local_time);
    std::cout << "Current time is: " << time_string << std::endl;

    return 0;
}
