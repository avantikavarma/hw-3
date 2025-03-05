#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <chrono>

#include "map.h"




struct Task {
    std::string identifier;
    int duration_time;
    int remaining_time;
    int vruntime;
};

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout << "Usage: ./cfs_sched <task_file.dat>";
    }
    std::ifstream file(argv[1]);

    if(!file){
        std::cout << "Error: cannot open file doesnotexist.dat";
    }
    Multimap<int, Task> task_map;

  
    std::string line;
    while(std::getline(file, line)){
        std::istringstream task(line);
        std::string identifier;
        unsigned int start_time;
        unsigned int duration_time;

        task >> identifier >> start_time >> duration_time;
        Task t = {identifier, duration_time, duration_time};
        task_map.Insert(start_time, t);
    }
    
// need smth for timeline
    unsigned int tick = 0;
    unsigned int min_vruntime = 0;
    while(task_map.Size() != 0){
        auto current_task = task_map.Get(task_map.Min()); // how to find other tasks with same key value??
        if(current_task.key == tick){
            unsigned int current_runtime = min_vruntime;
        }

        
    }

}


