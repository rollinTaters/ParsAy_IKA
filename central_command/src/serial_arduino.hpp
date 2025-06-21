#ifndef SERIAL_ARDUINO_HPP
#define SERIAL_ARDUINO_HPP

#include <string>

//Haberleşme arayüzü tanımı (header)

/**
 * @brief A class to handle serial communication with Arduino over USB.
 */
class SerialArduino
{
public:
    /**
     * @brief Constructor with serial device path (e.g., "/dev/ttyUSB0")
     */
    SerialArduino(const std::string& device, int baudrate = 9600);

    /**
     * @brief Destructor to clean up
     */
    ~SerialArduino();

    /**
     * @brief Sends a command string to the Arduino.
     */
    bool sendCommand(const std::string& command);

    /**
     * @brief Reads a line response from Arduino (if any).
     */
    std::string readResponse();

private:
    int serial_fd;
    bool initializeSerialPort(const std::string& device, int baudrate);
};

#endif // SERIAL_ARDUINO_HPP
