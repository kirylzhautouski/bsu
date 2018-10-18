
/*************************************************************************
   LAB 1                                                                

    Edit this file ONLY!

*************************************************************************/

#include <stdio.h>
#include "dns.h"
#include "../hash_table/hash_table.h"

// example: "127.0.0.1" -> 0x7F000001
static IPADDRESS ToIPAddress(const unsigned int *ip)
{
    IPADDRESS transformedIP = *ip << 24 | *(ip + 1) << 16 | *(ip + 2) << 8  |
                              *(ip + 3);

    return transformedIP;
}

DNSHandle InitDNS()
{
    HashTable* hashTable = InitHashTable();

    if (hashTable != INVALID_HASH_TABLE)
        return (DNSHandle)hashTable;

    return INVALID_DNS_HANDLE;
}

void LoadHostsFile(DNSHandle hDNS, const char* hostsFilePath)
{
    FILE* file = fopen(hostsFilePath, "r");
    if (file != NULL)
    {
        char domain[256];
        unsigned int ip[4];

        while (fscanf(file, "%u.%u.%u.%u    %s\n", ip, ip + 1, ip + 2, ip + 3, domain) == 5)
        {
            IPADDRESS ipAddress = ToIPAddress(ip);

            HashTableInsert((HashTable*)hDNS, domain, ipAddress);
        }

        fclose(file);
    }
}

IPADDRESS DnsLookUp(DNSHandle hDNS, const char* hostName)
{
    IPADDRESS ipAddress = HashTableSearch((HashTable*)hDNS, hostName);

    if (ipAddress != INVALID_VALUE)
        return ipAddress;

    return INVALID_IP_ADDRESS;
}

void ShutdownDNS(DNSHandle hDNS)
{
    DeleteHashTable((HashTable*)hDNS);
}
