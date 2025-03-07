#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include "map.h"

struct Task {
    char identifier;
    unsigned int start_time;
    unsigned int duration;
    unsigned int vruntime;

    // Define comparison operator for sorting
    bool operator<(const Task& other) const {
        return start_time < other.start_time;
    }
};

class CFS_Scheduler {
public:
    explicit CFS_Scheduler(std::vector<Task> input)
        : tick(0), min_vruntime(0), timeline(), tasks(std::move(input)) {}

    void Scheduler();

private:
    unsigned int tick;
    unsigned int min_vruntime;
    Multimap<int, Task> timeline;
    std::vector<Task> tasks;
};


void CFS_Scheduler::Scheduler() {
    while (!tasks.empty() || timeline.Size() >= 0) {
        // Add tasks to the timeline based on start time
        if(tasks.front().start_time == tick) {
            Task task = tasks.front();
            task.vruntime = min_vruntime; // Initialize vruntime
            timeline.Insert(task.vruntime, task); // Insert into multimap
            tasks.erase(tasks.begin()); // Remove from task list
        }

        if (timeline.Size() == 0) {
            std::cout << tick << " [0]: _\n";
        } else {
            // Get task with the smallest vruntime
            int min_key = timeline.Min();
            Task current_task = timeline.Get(min_key);

            std::cout << tick << " [" << timeline.Size() << "]: " << current_task.identifier;

            // Process task execution
            current_task.duration--;

            timeline.Remove(min_key); // Remove the old instance

            if (current_task.duration == 0) {
                std::cout << "*"; // Mark as finished
            } else {
                current_task.vruntime++;
                timeline.Insert(current_task.vruntime, current_task); // Reinsert with updated vruntime
            }

            min_vruntime = current_task.vruntime; // Update min vruntime
        }

        std::cout << '\n';
        tick++;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <task_file.dat>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: cannot open file " << argv[1] << '\n';
        return 1;
    }

    std::vector<Task> tasks;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream task(line);
        char identifier;
        unsigned int start_time, duration_time;

        task >> identifier >> start_time >> duration_time;
        tasks.push_back({identifier, start_time, duration_time, 0}); // Initial vruntime = 0
    }

    // Sort tasks by start_time
    std::sort(tasks.begin(), tasks.end());

    CFS_Scheduler sched(tasks);
    sched.Scheduler();

    return 0;
}
