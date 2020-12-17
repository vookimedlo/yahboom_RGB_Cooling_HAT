# yahboom_RGB_Cooling_HAT

[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fvookimedlo%2Fyahboom_RGB_Cooling_HAT.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Fvookimedlo%2Fyahboom_RGB_Cooling_HAT?ref=badge_shield)
[![Code Quality Score](https://www.code-inspector.com/project/15242/score/svg)](https://frontend.code-inspector.com/public/project/15242/yahboom_RGB_Cooling_HAT/dashboard)
[![Code Grade](https://www.code-inspector.com/project/15242/status/svg)](https://frontend.code-inspector.com/public/project/15242/yahboom_RGB_Cooling_HAT/dashboard)

The main purpose of this software was to provide a functional and configurable controlling SW for the *Yahboom RGB Cooling HAT*. Such SW is currently missing and Yahboom provides only programming [samples][1].

This software is dependant on the [wiringPi][2] library.

## Build Instructions

```
cd src
mkdir build
cd build
cmake ..
make CFLAGS=-DNDEBUG
```

The resulting binary is called `cooling_hat`.

## Usage
```
./cooling_hat [-d] [-q] [-r fan_range:...] | [-h] | [-f fan_settings] | [-l led_settings] | [-e effect_settings]

        The following options are available:
                -h                  Shows usage.
                -q                  Suppress logging.
                -d                  Runs as a daemon.
                -f fan_settings     Sets the fan speed to the specified value and quits.
                                    fan_settings:
                                       0: Fan Off
                                       1: 100% speed
                                       2:  20% speed
                                       3:  30% speed
                                       4:  40% speed
                                       5:  50% speed
                                       6:  60% speed
                                       7:  70% speed
                                       8:  80% speed
                                       9:  90% speed
                -l led_settings     Sets the given LED to the specified R,G,B values and quits.
                                    led_settings: LED_NUMBER,RED,GREEN,BLUE
                                       LED_NUMBER: 0-2
                                       LED_NUMBER: 3 ... all LEDs
                                              RED: 0-255
                                            GREEN: 0-255
                                             BLUE: 0-255
                -e effect_settings  Sets the LEDs effect to the specified values and quits.
                                    effect_settings: EFFECT,SPEED,COLOR
                                       EFFECT: 0 ... water light
                                       EFFECT: 1 ... breathing light
                                       EFFECT: 2 ... marquee
                                       EFFECT: 3 ... rainbow lights
                                       EFFECT: 4 ... colorful lights
                                        SPEED: 0 ... low
                                        SPEED: 1 ... medium
                                        SPEED: 2 ... high
                                        COLOR: 0 ... red
                                        COLOR: 1 ... green
                                        COLOR: 2 ... blue
                                        COLOR: 3 ... yellow
                                        COLOR: 4 ... purple
                                        COLOR: 5 ... cyan
                                        COLOR: 6 ... white
                -r fan_range        Sets the fan ranges. If multiple fan_range are specified they need to be ordered from the lowest temperature!
                                    fan_range: UPPER_RANGE,fan_settings
                                       UPPER_RANGE: 0-255
```
## Run upon a system start

The `systemd` service script was implemented to have the starting and stopping under the control.

To enable it, just copy the provided `systemd/cooling_hat.service` to the  `/etc/systemd/system/`.
Do not forget to build the `cooling_hat` binary and copy it over to the `/opt/bin/`.

Fan speed and temperature ranges may be adjusted per your needs. Simply change the line `ExecStart=/opt/bin/cooling_hat -r 30,0:40,3:60,5:70,9 -d` and provide your ranges (see the `-r` argument).

Then, the service needs to be enabled.

`sudo systemctl enable cooling_hat.service`

Restart your RPi and check that the service is running.

`systemctl list-units --type service --all`

```
UNIT                                                        LOAD      ACTIVE   SUB     DESCRIPTION                                                       
...                              
cooling_hat.service                                         loaded    active   running Yahboom's Cooling Hat control service  
```

All logging is redirected to the `/var/log/syslog` which might be useful for debugging purposes. The output can be suppressed by the `-q` argument, which is specified by default in the `systemd` service.

Many thanks to [@abecko][3] who provided his yahboom HW and got the idea for implementing this!

## License
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fvookimedlo%2Fyahboom_RGB_Cooling_HAT.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2Fvookimedlo%2Fyahboom_RGB_Cooling_HAT?ref=badge_large)


[1]: https://github.com/YahboomTechnology/Raspberry-Pi-RGB-Cooling-HAT/
[2]: http://wiringpi.com/
[3]: https://github.com/abecko
