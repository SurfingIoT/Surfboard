/**
* @file SensorSample.h
*
* This file declares the Sample class implementation for the sensor stream schema used in JOTB Hackathon.
*
* ITRS Group all rights reserved (c) 2017.
*/

// External dependencies
#include <stdio.h>

#ifndef IO_VALO_IOT_SENSOR_SAMPLE_H
#define IO_VALO_IOT_SENSOR_SAMPLE_H

namespace io {
namespace valo {
namespace iot {

static char g_date_time[] = "0000-00-00T00:00:00.000Z";

/**
 * This class represents a sample of the stream content for the IoT demonstration.
 * It encapsulates the construction of the sample payload for a given sensor.
 *
 * @code
 *
 * @endcode
 */
class SensorSample {

  // ---------------------------------------------------------------------------------------------------------------------
  // Public methods
  // ---------------------------------------------------------------------------------------------------------------------
 public:

  /**
   * Constructor.
   *
   * @param meas The measurement value (e.g. temperature).
   * @param unit The unit of the measurement.
   */
  SensorSample(const char *stream, const char *meas, const char *unit) : stream_(stream), meas_(meas), unit_(unit) {};

  /**
   * Destructor.
   */
  virtual ~SensorSample() {}

  /**
   * Sets the GPS position of the IoT device.
   *
   * @param lon GPS longitude.
   * @param lat GPS latitude.
   */
  inline void setPosition(const char *lon, const char *lat) {
    lon_ = lon;
    lat_ = lat;
  }

  /**
   * Sets the value for the sensor.
   * @param value
   */
  inline void setValue(const char *value) {
    refresh_date_time();
    value_ = value;
  }

  /**
   * Serialise the sample into a string for a given contributor identifier.
   *
   * @param id the contributor identifier.
   * @param output the output string containing the serialised data.
   * @return the length of the serialised string.
   */
  inline int toString(const char *id, char *output) const {
    sprintf(output, "{\"contributor\":\"%s\", \"position\":{\"latitude\":%s,\"longitude\":%s},"
        "\"timestamp\":\"%s\",\"%s\":%s,\"units\":\"%s\"}", id, lon_, lat_, g_date_time, meas_, value_, unit_);
  }

  // ---------------------------------------------------------------------------------------------------------------------
  // Protected methods
  // ---------------------------------------------------------------------------------------------------------------------
 protected:
  inline void refresh_date_time() const {
    sprintf(g_date_time, "%04d-%02d-%02dT%02d:%02d:%02d.000Z",year(),month(), day(), hour(),minute(),second());
  }

  // ---------------------------------------------------------------------------------------------------------------------
  // Public attributes
  // ---------------------------------------------------------------------------------------------------------------------
 public:
  const char *stream_;

  // ---------------------------------------------------------------------------------------------------------------------
  // Protected attributes
  // ---------------------------------------------------------------------------------------------------------------------
 protected:
  const char *lon_;
  const char *lat_;
  const char *meas_;
  const char *unit_;
  const char *value_;
};

} // iot
} // valo
} // io

#endif // IO_VALO_IOT_SENSOR_SAMPLE_H
