#include "DiskStream.h"

int DiskStream::seek(int pos_to_seek_to)
{
    current_pos_in_stream = pos_to_seek_to;
    return 0;
}

int DiskStream::read(void *destination, int total_num_of_bytes_to_read)
{
    int sector = current_pos_in_stream / disk->get_sector_size();
    int offset_in_sector = current_pos_in_stream % disk->get_sector_size();

    int total_to_read_in_current_sector = total_num_of_bytes_to_read;
    bool overflow = ( offset_in_sector + total_num_of_bytes_to_read >= disk->get_sector_size() );
    if(overflow) total_to_read_in_current_sector = disk->get_sector_size() - current_pos_in_stream;

    char buff[disk->get_sector_size()];
    int read_result = disk->read_sector(sector, 1, buff);
    if(read_result < 0) return -1;

    for(int i=0; i<total_to_read_in_current_sector; i++){
        *(char*)destination = buff[offset_in_sector + i];
        destination = (char*)destination + 1;
    }

    current_pos_in_stream += total_to_read_in_current_sector;
    if(overflow) return read(destination, total_num_of_bytes_to_read - total_to_read_in_current_sector);

    return 0;
} 
