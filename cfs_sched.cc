#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "map.h"

// min_vruntime should be updated to reflect the smallest vruntime among the currently running tasks. a
// values should be sorted by A,B,C

class Task {
public:
    char identifier;
    unsigned int start_time;
    unsigned int duration;
    unsigned int vruntime;

    Task(char identifier, unsigned int start_time, unsigned int duration){
        identifier = identifier;
        start_time = start_time;
        duration = duration;
        vruntime = 0;
    }
};

class CFS_Scheduler {
public:
    void Scheduler();

    CFS_Scheduler(std::vector<Task*> input) {
        tasks = input;
    }
private:
    unsigned int tick = 0;
    unsigned int min_vruntime = 0;
    unsigned int num_tasks = 0;
    Multimap<unsigned int, Task*> timeline;
    std::vector<Task*> tasks;

};

void CFS_Scheduler::Scheduler(){
    while(!tasks.empty() || timeline.Size() > 0){
        if(!tasks.empty()){
            for(auto task = tasks.begin(); task!= tasks.end(); ){
                auto current_task = *task;
                if(current_task->start_time == tick) {
                    timeline.Insert(current_task->vruntime, current_task);
                    num_tasks++;
                    task = tasks.erase(task);
                } else {
                    task++;
                }
            }
        }

        if(num_tasks == 0) {
            std::cout << tick << " [0]: _";
            tick++;
            continue;
        }
        auto current_task = timeline.Get(timeline.Min());

        while(current_task->vruntime  <= min_vruntime) {
            current_task->duration--;

            std::cout << tick <<" [" << num_tasks << "]: " << current_task->identifier; 
        
            if(current_task->duration == 0) {
                std::cout << "*";
                timeline.Remove(current_task->vruntime);
                delete current_task;
                num_tasks--;
                continue;
            }
            std::cout << "\n";

            current_task->vruntime++;
            timeline.Insert(current_task->vruntime, current_task);
            min_vruntime = timeline.Min();
            tick++;
        }
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Usage: " << argv[0]  << "<task_file.dat>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);

    if(!file){
        std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        return 1;
    }

    // Use multimap to store tasks by start tim e
    std::vector<Task*> tasks;
  
    std::string line;
    while(std::getline(file, line)){
        std::istringstream task(line);
        char identifier;
        unsigned int start_time;
        unsigned int duration_time;

        task >> identifier >> start_time >> duration_time;

        // Create task and add it to the map
        Task *t = new Task{identifier, start_time, duration_time};
        tasks.push_back(t);
    }

    CFS_Scheduler sched(tasks);
    sched.Scheduler();


    return 0;
}
