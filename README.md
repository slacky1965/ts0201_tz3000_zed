# <a id="Top">Tuya Temperature and Relative Humidity Sensor Zigbee on CHT8305 with custom firmware</a>

### [Описание на русском](README_rus.md)

### Custom firmware for Tuya sensor models

- _TZ3000_xr3htd96
- _TZ3000_fllyghyj
- _TZ3000_0s1izerx

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/ts0201.jpg"/>

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/ts0201_pcb.jpg"/>

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/ts0201_pcb2.jpg"/>

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/ts0201_pcb3.jpg"/>

**The author assumes no responsibility if you turn your smart sensor into a half-witted sensor by using this project.**

If you have a different signature, it is better not to flash in without checking for a coincidence of the sensor and GPIO.

Only tested in `zigbee2mqtt`.

## Why. 

We needed a temperature and humidity sensor that could transmit its values ​ ​ not only to the coordinator, but also directly to another device, such as a monitor with a screen. Well, so, as the firmware is still done from scratch, then at the same time make control of another device via OnOff directly, for example, when a certain temperature is reached, turn on the air conditioner, or, when a certain humidity is reached, turn on the hood. Well, turn it off later, respectively :))

And more. The original firmware cannot show the temperature below -20 degrees. And the sensor itself can CHT8305 up to -40.

## Result. 

**About**

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/about.jpg"/>

**Exposes**

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/exposes.jpg"/>

**Reporting**

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/reporting.jpg"/>

## How to update.

First, add external [converter](https://github.com/slacky1965/ts0201_tz3000_zed/tree/main/zigbee2mqtt) `ts0201 _ tz3000 _ orig.js` to z2m. It activates OTA in z2m for a sensor with firmware from Tuya.

Next, you need to add a local update store.

Create a directory` images` in the z2m directory and put the file [1141-d3a3-1111114b-ts0201_tz3000_zrd.zigbee](https://github.com/slacky1965/ts0201_tz3000_zed/raw/refs/heads/main/bin/1141-d3a3-1111114b-ts0201_tz3000_zrd.zigbee).

Copy the file [local_ota_index.json](https://github.com/slacky1965/ts0201_tz3000_zed/blob/main/zigbee2mqtt/local_ota_index.json) to the z2m directory.

And add local storage to the z2m config (configuration.yaml)

```
ota:
  zigbee_ota_override_index_location: local_ota_index.json
```

And we put the converter `ts0201 _ tz3000 _ orig.js` in the `external _ converters` directory, which needs to be created in the z2m root.

Then reboot z2m and see a new device

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/tuya_ready.jpg"/>

Then go to the OTA section. And see your device there. Click check for updates.

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/update.jpg"/>
	
Hit the red button. And update.

If the result is not as described, it means you did something wrong (did not put the file where it should be, did not reboot z2m) or your thermostat signature is not in the list of supported devices.

> [!WARNING]
> Attention!!! If in the process you find a new updateon other Tuya devices that you may still have in your system, you do not need to update anything!!!! Otherwise you will flash into this device firmware from the sensor and get a brick!!!! If the update process has already started by mistake, just turn off the device!!!!

Then wait for it to finish.

After the update, you need to remove the device from z2m. Reboot z2m. Allow pairing. Hold the button for 5 seconds until the LED starts to blink and release. The pairing begins.

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/joined.jpg"/>

That's it, the sensor is ready to go.

> [!WARNING]
> Attention!!! The firmware flashed for the first time is intermediate. It contains code for checking `bootloader`, which carries a potential threat of damage to this `bootloader` in the future. Therefore, you need to update again to the final version, where this code is not used. You can't do it right away without this check (hello Tuya). For understanding - the intermediate version has the number 1.0.00.

## Device settings.

- **Battery** - battery capacity in %.
- **Voltage** - battery voltage in mV.
- **Temperature** - range from -40 to + 125 ° C.
- **Humidity** - range 0 to 100%.
- **Temperature offset** - temperature calibration from -5 to + 5 ° C in increments of 0.1.
- **Humidity offset** - humidity calibration from -10 to + 10% with step 1.
- **Read interval** - sensor read interval. 5 to 600 seconds in increments of 1. This parameter should be given special attention. This parameter also affects the time that the module "sleeps," i.e. is in battery saving mode. By and large, the module "sleeps" this entire period. By default, this period is set to 10 seconds. The module wakes up every 10 seconds, reads data from the sensor (temperature and humidity) and falls asleep again for 10 seconds. Choose 20 seconds, will "sleep" for 20 seconds. This must be understood and remembered that before any change in parameters through the network, you need to briefly press a button on the sensor so that it "wakes up." Also, for longer battery life, you need to consider the `MinIntrerval` setting for temperature and humidity in the `Reporting` section. By default, the same time is set there as the sensor reading interval. Sensor sleep also depends on this parameter. Therefore, if the sensor reading interval is changed upward, then `MinInterval` should also be brought to this value.
- **Enabling temperature control** - Activate remote device control by temperature.
- **High temperature** - high temperature limit - when this limit goes up, a command will be sent to turn on the remote device.
- **Low temperature** - low temperature limit - when this limit is moved down, a command will be sent to turn off the remote device.
- **Enabling humidity control** - Activate remote device control by humidity.
- **High humidity** - high humidity boundary - when this boundary is crossed in a larger direction, a command will be sent to turn on the remote device.
- **Low humidity** - low humidity limit - when this limit is moved down, a command will be sent to turn off the remote device.
- **Switch actions** - configures what command we send when controlling a remote device. If this parameter is set to `On` (this is the default), then the `On` command is sent to turn on the remote device, and `Off` to turn it off. If this parameter is set to `Off`, then the `Off` command is sent to turn on the remote device, and `On` to turn it off.

## A little about setting up direct communication to control a remote device.

To control an external device directly, you must first set up binding. This is done in the `Bind` tab of z2m. Select Endpoint 1, then select the device you want to control, mark the `OnOff` cluster. Then press the button on the sensor itself to wake it up and then press `Bind` in the z2m interface. The system will notify you of a successful connection or an error.

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/binding.jpg"/>

The sensor can be configured to control only temperature, only humidity, or both values ​​simultaneously.

## Energy consumption

With the default setting of `Read interval`, 10 seconds of two AAA batteries should last at least 2 years.

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/read_period_10sec.jpg"/>

With a `Read interval` setting of 60 seconds (and the same values ​​in `MinInterval` in the temperature and humidity reports), two AAA batteries should last about 8 years.

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/read_period_60sec.jpg"/>

You can contact me at **[Telegram](https://t.me/slacky1965)**.

### If you would like to thank the author, you can do so via [YouMoney](https://yoomoney.ru/to/4100118300223495).

## История версий
- 1.0.01
	- Initial version.
- 1.0.02
	- Bugs with the transmission of humidity and high temperature have been identified and fixed.
- 1.0.03
	- Corrected forced send of reports when pressing the button.
	- Fixed bugs with reports (SDK is not perfect).
	- Added temperature, humidity and battery clusters to `Output Cluster List`.
	
[Top](#Top)


