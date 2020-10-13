#ifndef COOLING_HAT_INFORMATION_H
#define COOLING_HAT_INFORMATION_H

void get_cpu_load(char *output_value, size_t output_value_size);

void get_ram_usage(char *output_value, size_t output_value_size);

void get_disk_usage(char *output_value, size_t output_value_size);

void get_ip_address(char *output_value, size_t output_value_size);

void get_temperature(char *output_value, size_t output_value_size, double temperature);

double get_temperature_double();

#endif //COOLING_HAT_INFORMATION_H
