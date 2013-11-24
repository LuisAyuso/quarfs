
#include "joystick.h"

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

namespace {
    bool get_event(int fd, js_event* event) {
        int bytes = read(fd, event, sizeof(*event));

        if(bytes != -1 && bytes == sizeof(*event)) {
            return true;
        }

        return false;
    }
}

Joystick::Joystick() {
    this->fd = open("/dev/input/js0", O_RDONLY | O_NONBLOCK);
}
 
Joystick::~Joystick() {
}
 
int Joystick::get_num_buttons() {
    char number_of_buttons;
    ioctl(this->fd, JSIOCGBUTTONS, &number_of_buttons);
    return (int)number_of_buttons;
}

int Joystick::get_num_axes() {
    char number_of_axes;
    ioctl(this->fd, JSIOCGAXES, &number_of_axes);
    return (int)number_of_axes;
}

char* Joystick::get_name() {
    char* name = new char[128];
    ioctl(this->fd, JSIOCGNAME(128), name);
    return name;
}

