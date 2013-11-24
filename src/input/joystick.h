#pragma once

 
 
struct  Joystick {
public:
    Joystick();

    virtual ~Joystick();

    bool load(char*);
    bool is_ready();

    int get_num_buttons();
    int get_num_axes();
    char* get_name();

private: 


    int fd;
    bool ready;
};
