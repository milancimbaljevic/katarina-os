#pragma once
#include "Disk.h"

class DiskStream{
    int current_pos_in_stream;
    Disk* disk = nullptr;
    DiskStream(Disk* disk) : disk(disk){}
public:
    static DiskStream* create_disk_stream(int disk_id){
        if(!Disk::disks[disk_id]) return nullptr;
        return new DiskStream(Disk::disks[disk_id]);
    }

    int seek(int pos_to_seek_to);
    int read(void* destination, int total_num_of_bytes_to_read);

    ~DiskStream(){
        disk = nullptr;
    }
};