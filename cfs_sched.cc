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

    private:
    unsigned int tick = 0;
    unsigned int min_vruntime = 0;
    Multimap<int, Task> timeline;
    std::vector<Task> tasks;


};

void CFS_Scheduler::Scheduler(){
    while(!tasks.empty()){
        for(auto task : tasks){
            if(task.start_time == tick){
                task.vruntime = min_vruntime;
                timeline.Insert(task.vruntime, task);
                //somehow erase task from vector
            }
        }

        if(timeline.Size() != 0){
            auto current_task = timeline.Get(timeline.Min());
            std::cout << tick <<"[" << timeline.Size() << "]:" << current_task.identifier;
            current_task.duration--;

            if(current_task.duration == 0){
                std::cout << "*";
                timeline.Remove(timeline.Min());
            } else {
                current_task.vruntime++;
                timeline.Remove(timeline.Min());
                timeline.Insert(current_task.vruntime, current_task);
            }
            min_vruntime = current_task.vruntime;
        }
        tick++;   
    }

}

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage: ./cfs_sched <task_file.dat>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);

    if(!file){
        std::cout << "Error: cannot open file " << argv[1] << std::endl;
        return 1;
    }

    // Use multimap to store tasks by start time
    std::vector<Task> tasks;
  
    std::string line;
    while(std::getline(file, line)){
        std::istringstream task(line);
        char identifier;
        unsigned int start_time;
        unsigned int duration_time;

        task >> identifier >> start_time >> duration_time;

        // Create task and add it to the map
        Task t = {identifier, start_time, duration_time, 0}; // Set vruntime to 0 initially
        tasks.push_back(t);
    }

    CFS_Scheduler sched;
    sched.Scheduler();


    return 0;
}
