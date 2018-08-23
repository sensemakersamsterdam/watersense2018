TODO

- add Relay MOSFET DFR0457
- add Ultrasonic sensor (10 measurements and do average)
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

  if (!bytes || bytes.length != 26) {
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
      FDC1004: (bytes[4] & 8) > 0
    },
    board_voltage: decodeUI16(bytes, 5),
    lsm303agr_temperature: decodeUI8(bytes, 7),
    bmp280_temperature: decodeFloat(bytes, 8),
    bmp280_pressure: decodeFloat(bytes, 12),
    bmp280_altitude: decodeFloat(bytes, 16),
    vl53l0x_distance: decodeUI16(bytes, 20),
    fdc1004_capacity: decodeFloat(bytes, 22)
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
