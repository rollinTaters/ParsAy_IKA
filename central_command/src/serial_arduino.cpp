#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

//!!!!!!!!!!!!!!ATTENTION!!!!!!!
// THAT CODE IS JUST FOR TEST SO IT IS USELESS

// Arduino'ya giden portu ayarla
#define SERIAL_PORT "/dev/ttyS0"
#define BAUDRATE B9600

int main() {
    int serial_port = open(SERIAL_PORT, O_RDWR);

    if (serial_port < 0) {
        std::cerr << "Seri port açılamadı\n";
        return 1;
    }

    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(serial_port, &tty) != 0) {
        std::cerr << "Ayarlar alınamadı\n";
        return 1;
    }

    cfsetispeed(&tty, BAUDRATE);
    cfsetospeed(&tty, BAUDRATE);

    tty.c_cflag &= ~PARENB; // Parity yok
    tty.c_cflag &= ~CSTOPB; // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;     // 8 bit veri

    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ISIG;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(ICRNL | INLCR);

    tty.c_oflag &= ~OPOST;

    tcsetattr(serial_port, TCSANOW, &tty);

    // Arduino'ya veri gönder
    std::string msg = "Merhaba Arduino!\n";
    write(serial_port, msg.c_str(), msg.size());

    // Arduino'dan veri oku
    char read_buf[256];
    memset(&read_buf, '\0', sizeof(read_buf));
    int n = read(serial_port, &read_buf, sizeof(read_buf));

    if (n > 0) {
        std::cout << "Arduino'dan gelen: " << read_buf << "\n";
    } else {
        std::cout << "Arduino'dan veri alınamadı\n";
    }

    close(serial_port);
    return 0;
}
