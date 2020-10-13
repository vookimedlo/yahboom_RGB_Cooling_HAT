#ifndef COOLING_HAT_OLED_H
#define COOLING_HAT_OLED_H

struct oled_information {
    const char m_cpu_load[10];
    const char m_cpu_temperature[10];
    const char m_ram_usage[30];
    const char m_disk_usage[30];
    const char m_network_address[30];
};

void oled_initialization();
void oled_clear();
void oled_refresh(const struct oled_information* values);

#endif //COOLING_HAT_OLED_H
