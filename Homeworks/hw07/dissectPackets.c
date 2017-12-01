/// file: dissectPackets.c
/// description: Implementation of packet dissector - homework 7
/// author: Duc Phan - ddp3945@rit.edu

#define _BSD_SOURCE
#include <arpa/inet.h>
#include <endian.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Macros for bit manipulation
#define nibH(num) ((num) >> 4)
#define nibL(num) ((num) & 0xf)
#define byteHH(num) ((num) >> 24)
#define byteH(num) (((num) >> 16) & 0xff)
#define byteL(num) (((num) >> 8) & 0xff)
#define byteLL(num) ((num) & 0xff)
#define wordH(num) ((num) >> 16)
#define wordL(num) ((num) & 0xffff)

/// Read a complete packet from the file stream
/// @param file: the file stream to read from
void readPacket(FILE *file) {
    uint32_t packetLen = 0;
    fread((void *) &packetLen, sizeof(uint32_t), 1, file);
    char *packet = (char *) malloc(packetLen * sizeof(char));

    uint32_t num = 0;
    fread((void *) &num, sizeof(uint32_t), 1, file);
    num = htobe32(num);
    printf("Version:\t\t0x%X (%d)\n", nibH(byteHH(num)), nibH(byteHH(num)));
    printf("IHL (Header Length):\t\t0x%X (%d)\n",  nibL(byteHH(num)), nibL(byteHH(num)));
    printf("Type of Service (TOS):\t\t0x%X (%d)\n", byteH(num), byteH(num));
    printf("Total Length:\t\t0x%X (%d)\n", wordL(num), wordL(num));
    
    fread((void *) &num, sizeof(uint32_t), 1, file);
    num = htobe32(num);
    printf("Identification:\t\t0x%X (%d)\n", wordH(num), wordH(num));
    printf("IP Flags:\t\t0x%X (%d)\n", byteL(num) & ( ((1 << 3) - 1) << 5), byteL(num) & ( ((1 << 3) - 1) << 5));
    printf("Fragment Offset:\t\t0x%X (%d)\n", num & ( (1 << 13) - 1), num & ( (1 << 13) - 1));

    fread((void *) &num, sizeof(uint32_t), 1, file);
    num = htobe32(num);
    printf("Time To Live (TTL):\t\t0x%X (%d)\n", byteHH(num), byteHH(num));
    printf("Protocol:\t\t");
    switch (byteH(num)) {
        case 1:
            printf("ICMP");
            break;
        case 2:
            printf("IGMP");
            break;
        case 6:
            printf("TCP");
            break;
        case 9:
            printf("IGRP");
            break;
        case 17:
            printf("UDP");
            break;
        case 47:
            printf("GRE");
            break;
        case 50:
            printf("ESP");
            break;
        case 51:
            printf("AH");
            break;
        case 57:
            printf("SKIP");
            break;
        case 88:
            printf("GRE");
            break;
        case 89:
            printf("ESP");
            break;
        case 115:
            printf("L2TP");
            break;
        default:
            break;
    }
    printf(" 0x%X (%d)\n", byteH(num), byteH(num));
    printf("Header Checksum:\t\t0x%X (%d)\n", wordL(num), wordL(num));

    fread((void *) &num, sizeof(uint32_t), 1, file);
    num = htobe32(num);
    printf("Source Address:\t\t%d.%d.%d.%d\n", byteHH(num), byteH(num), byteL(num), byteLL(num));

    fread((void *) &num, sizeof(uint32_t), 1, file);
    num = htobe32(num);
    printf("Destination Address:\t\t%d.%d.%d.%d\n", byteHH(num), byteH(num), byteL(num), byteLL(num));

    fread((void *) packet, sizeof(char), packetLen - 20, file);
    free(packet);
}

/// Implementation for the packet dissector
/// @param argc: number of command line arguments
/// @param argv: array of command line arguments
/// @return: EXIT_SUCCESS if no errors occurred. Otherwise EXIT_FAILURE
int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "usage: dissectPackets inputFile\n");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    int nPackets = 0;
    if (fread((void *) &nPackets, sizeof(int), 1, file) != 1) {
        fprintf(stderr, "Failed to read count of packets.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    printf("==== File %s contains %d Packets.\n", argv[1], nPackets);

    for (int i = 1; i <= nPackets; i++) {
        printf("==>Packet %d\n", i);
        readPacket(file);
    }

    fclose(file);
    return 0;
}