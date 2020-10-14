#include "cooling_hat_oled.h"
#include "ssd1306_i2c.h"

void oled_initialization() {
    ssd1306_begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
    oled_clear();
}

void oled_clear() {
    ssd1306_clearDisplay();
}

void oled_refresh(const struct oled_information *values) {
    // Clear display
    oled_clear();

    if (!values) return;

    // Draw all information
    ssd1306_drawText(0, 0, values->m_cpu_load);
    ssd1306_drawText(56, 0, values->m_cpu_temperature);
    ssd1306_drawText(0, 8, values->m_ram_usage);
    ssd1306_drawText(0, 16, values->m_disk_usage);
    ssd1306_drawText(0, 24, values->m_network_address);

    // Refresh display
    ssd1306_display();
}
