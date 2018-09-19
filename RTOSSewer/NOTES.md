TODO:

- discuss LICENSE (maybe LGPL?)


===============================================================================
Payload examples:

version: 2018090700
payload: CC9E4978C901011119000000000000000000007F02000000000000660129014001
size   : 33 bytes

version: 2
payload: 0200C901011119000000000000006E0200000039014C013901
size   : 25 bytes


===============================================================================
Payload functions:

function Decoder(bytes, port) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.

  function decodeFloat(bytes, offset) {
    var bits = decodeUI32(bytes, offset)
    var e = bits >>> 23 & 0xff
    var m = (e === 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000
    return ((bits >>> 31 === 0) ? 1.0 : -1.0) * m * Math.pow(2, e - 150)
  }

  function decodeUI8(bytes, offset) {
    return bytes[offset + 0]
  }

  function decodeUI16(bytes, offset) {
    return bytes[offset + 1] << 8 | bytes[offset + 0]
  }

  function decodeUI32(bytes, offset) {
    return bytes[offset + 3] << 24 | bytes[offset + 2] << 16 | bytes[offset + 1] << 8 | bytes[offset + 0]
  }

  // it will be removed later
  if (bytes.length == 33 && decodeUI32(bytes, 0) == 2018090700) {
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
        Conductivity: (bytes[4] & 64) > 0,
        SEN0189: (bytes[4] & 128) > 0,
        VegetronixAquaPlumb: (bytes[5] & 1) > 0
      },
      board_voltage: decodeUI16(bytes, 6),
      lsm303agr_temperature: decodeUI8(bytes, 8),
      bmp280_temperature: decodeFloat(bytes, 9),
      bmp280_pressure: decodeFloat(bytes, 13),
      vl53l0x_distance: decodeUI16(bytes, 17),
      mb7092_distance: decodeUI16(bytes, 19),
      hcsr04_distance: decodeUI16(bytes, 21),
      ds18b20_temperature: decodeFloat(bytes, 23),
      conductivity_value: decodeUI16(bytes, 27),
      sen0189_value: decodeUI16(bytes, 29),
      vegetronixaquaplumb_value: decodeUI16(bytes, 31)
    }
  }

  var version = bytes.length > 2 ? decodeUI16(bytes, 0) : null

  if (version == 2) {
    return {
      version     : version,
      board_vo    : decodeUI16(bytes, 4),
      lsm303agr_te: (bytes[4] &   1) > 0 ? decodeUI8 (bytes,  6) : null,
      bmp280_te   : (bytes[4] &   2) > 0 ? decodeUI8 (bytes,  7) : null,
      bmp280_pr   : (bytes[4] &   2) > 0 ? decodeUI32(bytes,  8) : null,
      vl53l0x_di  : (bytes[4] &   4) > 0 ? decodeUI16(bytes, 12) : null,
      mb7092_di   : (bytes[4] &   8) > 0 ? decodeUI16(bytes, 14) : null,
      hcsr04_di   : (bytes[4] &  16) > 0 ? decodeUI16(bytes, 16) : null,
      ds18b20_te  : (bytes[4] &  32) > 0 ? decodeUI8 (bytes, 18) : null,
      cond_co     : (bytes[4] &  64) > 0 ? decodeUI16(bytes, 19) : null,
      sen0189_di  : (bytes[4] & 128) > 0 ? decodeUI16(bytes, 21) : null,
      aquap_di    : (bytes[5] &   1) > 0 ? decodeUI16(bytes, 23) : null
    }
  }

  return {
    TestValue: String.fromCharCode.apply(null, bytes)
  }
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
