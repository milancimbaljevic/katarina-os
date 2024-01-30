#pragma once

class FileSystem
{
}; // temporary

class Disk
{
private:
    static const int max_number_of_disks = 32;
    int sector_size = 512;
    int disk_id;
    FileSystem *fs = nullptr;

public:
    Disk(int disk_id, FileSystem *fs = nullptr, int sector_size = 512) : sector_size(sector_size), disk_id(disk_id), fs(fs) {}
    virtual int read_sector(unsigned long logical_block_address, unsigned long total_number_of_blocks_to_read, void *destination_to_write_in) = 0;
    static void init_disks();
    static Disk *get_disk(int disk_id) { return disks[disk_id]; }
    int get_sector_size() { return sector_size; }
    static Disk *disks[max_number_of_disks];
    static bool register_disk(Disk *disk)
    {
        if (disks[disk->disk_id])
            return false;
        if (disk->disk_id >= 32)
            return false;
        disks[disk->disk_id] = disk;

        return true;
    }
    ~Disk()
    {
        fs = nullptr;
        disks[disk_id] = nullptr;
    }
};

class MasterDisk : public Disk
{
public:
    MasterDisk(int disk_id, FileSystem *fs = nullptr, int sector_size = 512) : Disk(disk_id, fs, sector_size) {}
    virtual int read_sector(unsigned long logical_block_address, unsigned long total_number_of_blocks_to_read, void *destination_to_write_in) override;
};