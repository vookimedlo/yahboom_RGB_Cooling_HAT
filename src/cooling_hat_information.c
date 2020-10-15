#include "cooling_hat_information.h"

#include <linux/kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/statvfs.h>
#include <unistd.h>

#define TEMPERATURE_PATH "/sys/class/thermal/thermal_zone0/temp"

void get_average_load(char *output_value, size_t output_value_size) {
    struct sysinfo sys_info;
    if (sysinfo(&sys_info)) {
        *output_value = '\0';
        return;
    }
    const float average_cpu_load = sys_info.loads[0] / ((float)(1<<SI_LOAD_SHIFT));
    snprintf(output_value, output_value_size, "Load:%.2f", average_cpu_load);
}

void get_ram_usage(char *output_value, size_t output_value_size) {
    struct sysinfo sys_info;
    if (sysinfo(&sys_info)) {
        *output_value = '\0';
        return;
    }
    const unsigned long total_ram = sys_info.totalram >> 20;
    const unsigned long free_ram = sys_info.freeram >> 20;
    snprintf(output_value, output_value_size, "RAM:%.2f%%", free_ram / (double)total_ram);
}

void get_disk_usage(char *output_value, size_t output_value_size) {
    struct statvfs disk_info;
    statvfs("/", &disk_info);
    const unsigned long long total_blocks = disk_info.f_bsize;
    const unsigned long long total_size = total_blocks * disk_info.f_blocks;
    const unsigned long long free_disk = disk_info.f_bfree * total_blocks;
    snprintf(output_value, output_value_size, "HDD:%.2f%%", (free_disk >> 20) / (double)(total_size >> 20));
}

void get_ip_address(char *output_value, size_t output_value_size) {
    struct ifaddrs *if_addrs = NULL;
    *output_value = '\0';

    if (getifaddrs(&if_addrs) == 0) {
        struct ifaddrs *if_addrs_head = if_addrs;
        while (if_addrs != NULL) {
            if (if_addrs->ifa_addr->sa_family == AF_INET) {
                const void *tmp_addr_ptr = &((struct sockaddr_in *) if_addrs->ifa_addr)->sin_addr;
                char address_buffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmp_addr_ptr, address_buffer, INET_ADDRSTRLEN);

                if (strcmp(if_addrs->ifa_name, "eth0") == 0) {
                    snprintf(output_value, output_value_size, "eth0:%s", address_buffer);
                    break;
                } else if (strcmp(if_addrs->ifa_name, "wlan0") == 0) {
                    snprintf(output_value, output_value_size, "wlan0:%s", address_buffer);
                    break;
                }
            }
            if_addrs = if_addrs->ifa_next;
        }
        freeifaddrs(if_addrs_head);
    }
}

void get_temperature(char *output_value, size_t output_value_size, double temperature) {
    snprintf(output_value, output_value_size, "Temp:%.1fC", temperature);
}

double get_temperature_double() {
    double temperature = 0;
    int fd_temp = open(TEMPERATURE_PATH, O_RDONLY);
    if (fd_temp >= 0) {
        char buf[32];
        if (read(fd_temp, buf, sizeof(buf)) > 0)
            temperature = atoi(buf) / 1000.0;
        close(fd_temp);
    }
    return temperature;
}
