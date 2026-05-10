# Aefera OS - Advanced ESP8266 Cyberpunk Operating System

![Aefera OS](https://img.shields.io/badge/Aefera_OS-Cyberpunk-blueviolet?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-ESP8266-blue?style=for-the-badge)
![Framework](https://img.shields.io/badge/Framework-RTOS_SDK-red?style=for-the-badge)
![Language](https://img.shields.io/badge/Language-C++17-green?style=for-the-badge)

**Aefera OS** adalah operating system super-advanced, cinematic, dan immersive yang kini di-rombak total untuk berjalan di microcontroller **ESP8266**. Dibangun secara native menggunakan **C++** dan **ESP8266 RTOS SDK** untuk menjamin performa maksimal pada keterbatasan *single-core* 80MHz/160MHz dan RAM (~50KB) ESP8266.

Project ini membawa experience "Cyberdeck / Linux Hacker Terminal" ke dalam sebuah MCU kecil, lengkap dengan *bootloader animation*, *cooperative/preemptive multitasking*, dan UI elemen dark futuristic.

---

## 🌌 Arsitektur Sistem (ESP8266 Single-Core Optimized)

Aefera OS dirancang ulang dengan sangat hati-hati untuk menjaga RAM usage tetap stabil:

```text
AeferaOS/
├── main/                   # Entry point (app_main)
├── components/
│   ├── kernel/             # Task Scheduler, Process Manager (Single-core xTaskCreate)
│   ├── ui/                 # UI Framework (LVGL wrapper with minimal buffer)
│   ├── apps/               # Built-in Apps (Terminal, SysMonitor, MatrixRain)
│   ├── network/            # WiFi Manager, Network Dashboard
│   ├── storage/            # SPIFFS/LittleFS Manager
│   └── drivers/            # Hardware Abstraction Layer (Display SPI)
├── partitions.csv          # Custom Partition Table untuk 4MB Flash
├── sdkconfig.defaults      # Low-memory & Performance OS Configuration
└── CMakeLists.txt          # Build System
```

### 1. KERNEL SYSTEM (The Heart)
- **Task Manager**: Karena ESP8266 adalah *single-core*, kernel menggunakan `xTaskCreate` (bukan pinned to core). Semua *thread* (UI, Terminal, Network) berjalan secara *time-sliced* oleh FreeRTOS.
- **Micro Stack Size**: Alokasi stack diperkecil drastis (hanya 1024-2048 bytes per task) agar tidak terjadi `Stack Overflow` di memori ESP8266 yang sangat sempit.

### 2. UI & VISUAL ENGINE
- **Minimal Buffer LVGL**: Menggunakan alokasi *Display Buffer* yang lebih kecil dibanding versi ESP32.
- **Cyberpunk Aesthetics**: Tetap mengusung tema Dark Hacker dengan warna `#0B0C10`, `#45F3FF`, dan `#00FF41`.

### 3. TERMINAL SHELL
Shell serial untuk menerima *command* hacker seperti:
- `ps` (Cek CPU & Memory per Process)
- `sysinfo` (Menampilkan spec Tensilica L106 ESP8266)
- `matrix`, `ascii`

---

## ⚡ Hardware & Wiring Diagram (ESP8266 NodeMCU)

### Target Hardware:
1. **ESP8266 Board** (NodeMCU / Wemos D1 Mini 4MB)
2. **Display**: TFT LCD ILI9341 2.8" SPI atau OLED SSD1306.

### Wiring SPI TFT (NodeMCU ESP8266):
| NodeMCU Pin | GPIO | TFT Pin   | Fungsi       |
|-------------|------|-----------|--------------|
| 3V3         | -    | VCC       | Power        |
| GND         | -    | GND       | Ground       |
| D7          | 13   | SDI/MOSI  | SPI MOSI     |
| D6          | 12   | SDO/MISO  | SPI MISO     |
| D5          | 14   | SCK       | SPI Clock    |
| D8          | 15   | CS        | Chip Select  |
| D3          | 0    | RESET     | Reset        |
| D4          | 2    | DC/RS     | Data/Command |
| 3V3         | -    | LED       | Backlight    |

*(Catatan: D4, D8, dan D3 berhubungan dengan boot mode ESP8266. Jika gagal *booting*, tambahkan *pull-up resistor*).*

---

## 🛠️ Langkah Setup (ESP8266 RTOS SDK)

Sistem *build* menggunakan **ESP8266 RTOS SDK v3.4+** yang sudah berbasis CMake (mirip ESP-IDF).

1. Buka terminal `ESP8266 RTOS SDK`.
2. Masuk ke direktori project:
   ```bash
   cd AeferaOS
   ```
3. Konfigurasi Sistem:
   ```bash
   idf.py menuconfig
   ```
4. Compile & Flash:
   ```bash
   idf.py -p COM3 flash monitor
   ```

---

## 📈 Trik Optimasi Khusus ESP8266

- **No PSRAM**: ESP8266 tidak memiliki eksternal RAM (PSRAM). Semua objek LVGL dan struktur data Kernel harus sangat *lightweight*.
- **Flash Strings**: Biasakan menyimpan string panjang di memori Flash menggunakan makro atau teknik PROGMEM (meskipun di RTOS SDK string literal diletakkan di iROM).
- **Yielding**: Animasi *looping* harus rutin memanggil `vTaskDelay` agar sistem WiFi (yang berjalan di background ESP8266) tidak terblokir dan *crash* (*Watchdog Reset*).
