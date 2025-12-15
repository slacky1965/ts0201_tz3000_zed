const {identify, reporting, ota} = require('zigbee-herdsman-converters/lib/modernExtend');

const definition = {
    fingerprint: [{manufacturerName: '_TZ3000_xr3htd96'},
                  {manufacturerName: '_TZ3000_fllyghyj'},
                  {manufacturerName: '_TZ3000_0s1izerx'},
                  {manufacturerName: '_TZ3000_dowj6gyi'}
                 ],
    zigbeeModel: ['TS0201'],
    model: 'Original Tuya Temperature/Humidity sensor ready for update',
    vendor: 'Slacky-DIY',
    description: 'Original Tuya temperature/humidity sensor ready for custom Firmware update',
    extend: [
      identify(),
    ],
    meta: {},
    ota: true,
};

module.exports = definition;