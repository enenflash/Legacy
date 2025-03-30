#include <iostream>
#include <pigpio.h>
#include <unistd.h>  // For sleep()

char* UART_DEVICE = "/dev/serial0";  // UART0 TX/RX (GPIO 14 & 15)
#define BAUD_RATE 115200                   // Standard baud rate

int main() {
    u_int8_t number = 240;
    // Initialize pigpio
    if (gpioInitialise() < 0) {
        std::cerr << "Failed to initialize pigpio!" << std::endl;
        return 1;
    }

    // Open UART port
    int uartHandle = serOpen(UART_DEVICE, BAUD_RATE, 0);
    if (uartHandle < 0) {
        std::cerr << "Failed to open UART!" << std::endl;
        gpioTerminate();
        return 1;
    }

    // Send data every second
    std::string message = "Hello from Raspberry Pi!\r\n";
    while (true) {
        //serWrite(uartHandle, const_cast<char*>(message.c_str()), message.length());  // Fixed conversion
        //int number = 1234;  // Example number
        serWrite(uartHandle, reinterpret_cast<char*>(&number), sizeof(number));
        number++;
        std::cout << "Sent: " << static_cast<int>(number) << std::endl;
        sleep(1);  // Wait 1 second
    }
