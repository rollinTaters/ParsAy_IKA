#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstring>
#include "serial_arduino.hpp"
 //Arduino ile USB üzerinden haberleşme yapan sınıf
// Set serial port path according to your setup
#define SERIAL_PORT "/dev/ttyUSB0"  //it may be with  S for example S0 , S1 , S2 , S3 

SerialArduino::SerialArduino(const std::string& device, int baudrate) {
    if (!initializeSerialPort(device, baudrate)) {
        std::cerr << "SerialArduino: Initialization failed\n";
    }
}

SerialArduino::~SerialArduino() {
    if (serial_fd >= 0) {
        close(serial_fd);
    }
}

bool SerialArduino::initializeSerialPort(const std::string& device,[[maybe_unused]] int baudrate) {
    serial_fd = open(device.c_str(), O_RDWR | O_NOCTTY);
    if (serial_fd < 0) return false;

    termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(serial_fd, &tty) != 0) return false;

    speed_t baud = B9600;
    cfsetispeed(&tty, baud);
    cfsetospeed(&tty, baud);

    tty.c_cflag |= (CLOCAL | CREAD); // enable receiver
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_iflag = 0;

    tcflush(serial_fd, TCIFLUSH);
    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) return false;

    return true;
}

bool SerialArduino::sendCommand(const std::string& command) {
    std::string cmd_with_newline = command + "\n";
    return write(serial_fd, cmd_with_newline.c_str(), cmd_with_newline.length()) > 0;
}

std::string SerialArduino::readResponse() {
    char buf[256];
    ssize_t n = read(serial_fd, buf, sizeof(buf));
    if (n > 0) return std::string(buf, n);
    return "";
}