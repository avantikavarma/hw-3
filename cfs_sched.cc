#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

class Task {
public:
    char identifier;
    unsigned int start_time;
    unsigned int duration;
    unsigned int vruntime;

    Task(char identifier, unsigned int start_time, unsigned int duration)
        : identifier(identifier), start_time(start_time), duration(duration), vruntime(0) {}
};

class CFS_Scheduler {
public:
    CFS_Scheduler(const std::vector<Task*>& input) : tasks(input) {}
    void Scheduler();

private:
    unsigned int tick = 0;
    unsigned int min_vruntime = 0;
    unsigned int num_tasks = 0;
    std::multimap<unsigned int, Task*> timeline;
    std::vector<Task*> tasks;
};

void CFS_Scheduler::Scheduler() {
    while (!tasks.empty() || !timeline.empty()) {
        if (!tasks.empty()) {
            for (auto task = tasks.begin(); task != tasks.end(); ) {
                Task* current_task = *task;
                if (current_task->start_time == tick) {
                    timeline.insert({current_task->vruntime, current_task});
                    num_tasks++;
                    task = tasks.erase(task);
                } else {
                    ++task;
                }
            }
        }

        if (num_tasks == 0) {
            std::cout << tick << " [0]: _\n";
            tick++;
            continue;
        }

        auto it = timeline.begin();
        Task* current_task = it->second;

        std::cout << tick << " [" << num_tasks << "]: " << current_task->identifier;

        current_task->duration--;
        tick++;

        if (current_task->duration == 0) {
            std::cout << "*\n";
            timeline.erase(it);
            delete current_task;
            num_tasks--;
        } else {
            std::cout << "\n";
            timeline.erase(it);
            current_task->vruntime++;
            timeline.insert({current_task->vruntime, current_task});
        }

        if (!timeline.empty()) {
            min_vruntime = timeline.begin()->first;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <task_file.dat>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        return 1;
    }

    std::vector<Task*> tasks;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream task_stream(line);
        char identifier;
        unsigned int start_time;
        unsigned int duration;

        if (!(task_stream >> identifier >> start_time >> duration)) {
            std::cerr << "Error: Invalid task format in file." << std::endl;
            continue;
        }

        tasks.push_back(new Task(identifier, start_time, duration));
    }

    CFS_Scheduler sched(tasks);
    sched.Scheduler();

    return 0;
}
