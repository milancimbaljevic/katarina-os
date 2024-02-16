#pragma once

#include "disk/DiskStream.h"
#include "kernel.h"
#include "print.h"

struct MBR_Parition
{
    unsigned short LBABegin;
    unsigned short NumberOfSectors;
    unsigned char TypeCode;
};

class MBR_Reader
{
    static DiskStream *mbr_stream;
    static bool init;

public:
    static MBR_Parition partitions[4];

    static void Init()
    {
        if (!init)
        {
            MBR_Reader::mbr_stream = DiskStream::create_disk_stream(0); // boot disk

            if (!MBR_Reader::mbr_stream)
            {
                kernel_panic("Can't load partition info!!!");
            }
            int current_pos;

            for (int i = 0; i < 4; i++)
            {
                current_pos = 446 + 16 * i;

                MBR_Reader::mbr_stream->seek(current_pos + 4);
                MBR_Reader::mbr_stream->read(&partitions[i].TypeCode, 1);

                MBR_Reader::mbr_stream->seek(current_pos + 8);
                MBR_Reader::mbr_stream->read(&partitions[i].LBABegin, 2);

                MBR_Reader::mbr_stream->seek(current_pos + 12);
                MBR_Reader::mbr_stream->read(&partitions[i].NumberOfSectors, 2);

                //     println("Partition {} TypeCode {h}", i, partitions[i].TypeCode);
                //     println("Partition {} LBABegin {h}", i, partitions[i].LBABegin);
                //     println("Partition {} NumberOfSectors {h}", i, partitions[i].NumberOfSectors);
            }

            init = true;
        }
    }
};

bool MBR_Reader::init = false;
DiskStream *MBR_Reader::mbr_stream = nullptr;
MBR_Parition MBR_Reader::partitions[4] = {};