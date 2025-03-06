#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "map.h"

struct Task {
    char identifier;
    unsigned int start_time;
    unsigned int duration;
    unsigned int vruntime;
};

class CFS_Scheduler {
public:
    void Scheduler();

    CFS_Scheduler(std::vector<Task> input) {
        tasks = input;
    }

private:
    unsigned int tick = 0;
    unsigned int min_vruntime = 0;
    Multimap<int, Task> timeline;
    std::vector<Task> tasks;
};

void CFS_Scheduler::Scheduler() {
    while (tasks.size() != 0 || timeline.Size() != 0) {
        // Add tasks to timeline based on start time
        for (auto task = tasks.begin(); task != tasks.end(); ) {
            auto start_time = task->start_time;
            if (start_time == tick) {
                task->vruntime = min_vruntime;
                timeline.Insert(task->vruntime, *task);
                task = tasks.erase(task);  // Properly update iterator after erase
            } else {
                task++;
            }
        }

        // If there are no tasks to run, print idle time
        if (timeline.Size() == 0) {
            std::cout << tick << " [0]: _";
        } else {
            // There are tasks to run
            Task current_task = timeline.Get(timeline.Min());  // Copy the task
            std::cout << tick << " [" << timeline.Size() << "]: " << current_task.identifier;

            // Decrease the duration of the current task
            current_task.duration--;

            if (current_task.duration == 0) {
                // Task is finished, mark it with "*" and remove it from the timeline
                std::cout << "*";
                timeline.Remove(timeline.Min());
            } else {
                // Task is still running, increment vruntime and reinsert it into the timeline
                current_task.vruntime++;
                timeline.Remove(timeline.Min());  // Remove the task before reinserting
                timeline.Insert(current_task.vruntime, current_task);
            }
            min_vruntime = current_task.vruntime;  // Update the min_vruntime for the next task
        }
        std::cout << '\n';
        tick++;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[1] << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file) {
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        return 1;
    }

    // Use vector to store tasks
    std::vector<Task> tasks;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream task(line);
        char identifier;
        unsigned int start_time;
        unsigned int duration_time;

        task >> identifier >> start_time >> duration_time;

        // Create task and add it to the vector
        Task t = {identifier, start_time, duration_time, 0}; // Set vruntime to 0 initially
        tasks.push_back(t);
    }

    CFS_Scheduler sched(tasks);
    sched.Scheduler();

    return 0;
}
