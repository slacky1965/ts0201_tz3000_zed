import * as m from 'zigbee-herdsman-converters/lib/modernExtend';
//import * as exposes from 'zigbee-herdsman-converters/lib/exposes';
//import * as utils from 'zigbee-herdsman-converters/lib/utils';
//import * as logger from 'zigbee-herdsman-converters/lib/logger';

const attrSensorReadPeriod = 0xF000;
const attrTemperatureOffset = 0xF001;
const attrTemperatureOnOff = 0xF002;
const attrTemperatureLow = 0xF003;
const attrTemperatureHigh = 0xF004;
const attrHumidityOffset = 0xF005;
const attrHumidityOnOff = 0xF006;
const attrHumidityLow = 0xF007;
const attrHumidityHigh = 0xF008;


export default {
    zigbeeModel: ['TS0201-z-SlD'],
    model: 'TS0201-z-SlD',
    vendor: 'Slacky-DIY',
    description: 'Temperature and humidity sensor',
    extend: [
      m.battery({
        "voltage": true, 
         "voltageReporting": true,
         "percentageReportingConfig": {"min": 3600, "max": 0, "change": 0}, 
         "voltageReportingConfig": {"min": 3600, "max": 0, "change": 0}}),
      m.temperature({reporting: {min: 10, max: 3600, change: 10}}), 
      m.humidity({reporting: {min: 10, max: 3600, change: 10}}), 
      m.numeric({
        name: "temperature_offset",
        cluster: "msTemperatureMeasurement",
        attribute: {ID: attrTemperatureOffset, type: 0x29},
        unit: "°C",
        valueMin: -5,
        valueMax: 5,
        valueStep: 0.1,
        scale: 100,
        description: "Offset to add/subtract to the inside temperature",
      }),
      m.numeric({
        name: "humidity_offset",
        cluster: "msRelativeHumidity",
        attribute: {ID: attrHumidityOffset, type: 0x29},
        unit: "%",
        valueMin: -10,
        valueMax: 10,
        valueStep: 1,
        scale: 100,
        description: "Offset to add/subtract to the inside temperature",
      }),
      m.numeric({
        name: "read_interval",
        cluster: "msTemperatureMeasurement",
        attribute: {ID: attrSensorReadPeriod, type: 0x21},
        unit: "Sec",
        valueMin: 5,
        valueMax: 60,
        valueStep: 1,
        description: "Sensors reading period",
      }),
      m.binary({
        name: "enabling_temperature_control",
        cluster: "msTemperatureMeasurement",
        attribute: {ID: attrTemperatureOnOff, type: 0x10},
        description: "Enables/disables Tempearure control",
        valueOn: ["ON", 0x01],
        valueOff: ["OFF", 0x00],
      }),
      m.numeric({
        name: "low_temperature",
        cluster: "msTemperatureMeasurement",
        attribute: {ID: attrTemperatureLow, type: 0x29},
        unit: "°C",
        valueMin: -40,
        valueMax: 125,
        valueStep: 0.1,
        scale: 100,
        description: "Temperature low turn-off limit",
      }),
      m.numeric({
        name: "high_temperature",
        cluster: "msTemperatureMeasurement",
        attribute: {ID: attrTemperatureHigh, type: 0x29},
        unit: "°C",
        valueMin: -40,
        valueMax: 125,
        valueStep: 0.1,
        scale: 100,
        description: "Temperature high turn-on limit",
      }),
      m.binary({
        name: "enabling_humidity_control",
        cluster: "msRelativeHumidity",
        attribute: {ID: attrHumidityOnOff, type: 0x10},
        description: "Enables/disables Humidity control",
        valueOn: ["ON", 0x01],
        valueOff: ["OFF", 0x00],
      }),
      m.numeric({
        name: "low_humidity",
        cluster: "msRelativeHumidity",
        attribute: {ID: attrHumidityLow, type: 0x29},
        unit: "%",
        valueMin: 1,
        valueMax: 100,
        valueStep: 1,
        scale: 100,
        description: "Humidity low turn-off limit",
      }),
      m.numeric({
        name: "high_humidity",
        cluster: "msRelativeHumidity",
        attribute: {ID: attrHumidityHigh, type: 0x29},
        unit: "%",
        valueMin: 1,
        valueMax: 100,
        valueStep: 1,
        scale: 100,
        description: "Humidity high turn-on limit",
      }),
      m.enumLookup({
        name: "switch_actions",
        lookup: {off: 0, on: 1},
        cluster: "genOnOffSwitchCfg",
        attribute: "switchActions",
        description: "Actions switch",
      }),
    ],
    meta: {},
    ota: true,
};
