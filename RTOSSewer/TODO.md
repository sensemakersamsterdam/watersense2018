TODO

- add measurements with sound speaker 8ohm 0.5W and microphone CJMCU-9812
- add BMP3290 sensor
- add Aquaplumb sensor
- add Turbidity sensor
- discuss LICENSE (maybe LGPL?)

===============================================================================
Payload functions:

function Decoder(bytes, port) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.

  function decodeFloat(bytes, offset) {
    var bits = decodeUI32(bytes, offset);
    var e = bits >>> 23 & 0xff;
    var m = (e === 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000;
    return ((bits >>> 31 === 0) ? 1.0 : -1.0) * m * Math.pow(2, e - 150);
  }

  function decodeUI8(bytes, offset) {
    return bytes[offset + 0];
  }

  function decodeUI16(bytes, offset) {
    return bytes[offset + 1] << 8 | bytes[offset + 0];
  }

  function decodeUI32(bytes, offset) {
    return bytes[offset + 3] << 24 | bytes[offset + 2] << 16 | bytes[offset + 1] << 8 | bytes[offset + 0];
  }

  if (!bytes || bytes.length != 30) {
    return {
      TestValue: String.fromCharCode.apply(null, bytes)
    }
  }

  return {
    result: 'OK',
    version: decodeUI32(bytes, 0),
    board_modules: {
      LSM303AGR: (bytes[4] & 1) > 0,
      BMP280: (bytes[4] & 2) > 0,
      VL53L0X: (bytes[4] & 4) > 0,
      MB7092: (bytes[4] & 8) > 0,
      HCSR04: (bytes[4] & 16) > 0,
      DS18B20: (bytes[4] & 32) > 0,
      conductivity: (bytes[4] & 64) > 0,
      SEN0189: (bytes[4] & 128) > 0
    },
    board_voltage: decodeUI16(bytes, 5),
    lsm303agr_temperature: decodeUI8(bytes, 7),
    bmp280_temperature: decodeFloat(bytes, 8),
    bmp280_pressure: decodeFloat(bytes, 12),
    vl53l0x_distance: decodeUI16(bytes, 16),
    mb7092_distance: decodeUI16(bytes, 18),
    hcsr04_distance: decodeUI16(bytes, 20),
    ds18b20_temperature: decodeFloat(bytes, 22),
    conductivity_value: decodeUI16(bytes, 26),
    sen0189_value: decodeUI16(bytes, 28)
  };
}

function Converter(decoded, port) {
  // Merge, split or otherwise
  // mutate decoded fields.

  var converted = decoded;

  // if (port === 1 && (converted.led === 0 || converted.led === 1)) {
  //   converted.led = Boolean(converted.led);
  // }

  return converted;
}

function Validator(converted, port) {
  // Return false if the decoded, converted
  // message is invalid and should be dropped.

  //return converted && converted.result === 'OK';
  return true;
}
