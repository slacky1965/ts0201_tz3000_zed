# <a id="Top">Tuya Temperature and Relative Humidity Sensor Zigbee on CHT8305 with custom firmware</a>

### Custom firmware for Tuya sensor models

- _TZ3000_xr3htd96

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/ts0201.jpg"/>

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zrd/refs/heads/main/doc/images/ts0201_pcb.jpg"/>

**Автор не несет никакой отвественности, если вы, воспользовавшись этим проектом, превратите свой умный датчик в полоумный.**

Если у вас другая сигнатура, лучше не заливать, не проверив на совпадение сенсора и GPIO.

Проверялся только в zigbee2mqtt.

## Зачем. 

Понадобился датчик температуры и влажности, который умел бы отдавать свои значения не координатору, а напрямую другому устройству, например монитору с экраном. Ну а так, как прошивку все равно делать с нуля, то заодно сделать управление другим устройством через OnOff напрямую, например при достижении определенной температуры включить кондиционер, или при достижении определенной влажности включить вытяжку. Ну и выключить потом соответственно :))

## Что получилось. 

**About**

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/about.jpg"/>

**Exposes**

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/exposes.jpg"/>

**Reporting**

<img src="https://raw.githubusercontent.com/slacky1965/ts0201_tz3000_zed/refs/heads/main/doc/images/reporting.jpg"/>

