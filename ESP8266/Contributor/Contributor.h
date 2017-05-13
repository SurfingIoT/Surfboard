
/**
 * @file Contributor.h
 *
 * This file declares the Contributor class implementation
 *
 * @license MIT
 * @author David Torelli Rosendo <dtorelli@itrsgroup.com>
 */

#ifndef IO_VALO_IOT_CONTRIBUTOR_H
#define IO_VALO_IOT_CONTRIBUTOR_H

// External dependencies
#include <stdio.h>

// Internal dependencies
#include "SensorSample.h"

namespace io {
namespace valo {
namespace iot {

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// [constants]
// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

#define MAX_PAYLOAD_SIZE 256

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// [globals]
// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

char json_sample[MAX_PAYLOAD_SIZE];

/**
 * This class represents a contributor inside Valo system.
 * The user will configure it to point to a specific Valo host and port and will set the stream templates to send
 * data.
 *
 * @code
 * // One stream with temperature and humidity
 * Contributor con("3645634565", "localhost", 8888);
 * con.feed("/streams/iot/team1/temp_humidity", "{\"temp\":%d, \"humidity\":%d}", 30, 40);
 *
 * // One stream with temperature and another with humidity
 * con.feed("/streams/iot/team1/temp", "{\"temp\":%d}", 30);
 * con.feed("/streams/iot/team1/humidity", "{\"humidity\":%d}", 40);
 * @endcode
 */
template<typename T>
class Contributor {

  // ---------------------------------------------------------------------------------------------------------------------
  // Public methods
  // ---------------------------------------------------------------------------------------------------------------------
 public:

  /**
   * Constructor.
   *
   * @param p the printer used to send the information (e.g. Serial)
   * @param id the unique identifier of this contributor.
   * @param host the host where this contributor is pointing to.
   * @param port the port in the host where this contributor is pointing to.
   */
  Contributor(T &p, const char *id, const char *host, int port) : p_(p), id_(id), host_(host), port_(port) {
  }

  /**
   * Destructor.
   */
  virtual ~Contributor() {};

  /**
   * Feeds the stream with a given sample of data.
   *
   * @param uri the uri of the target stream where data will be feed.
   * @param sch stream format without considering the contributor schema.
   * @param ... Variable arguments ...
   */
  inline void feed(const char *uri, const char *sch, ...) {
    // Compose the payload from the given schema and values
    va_list arg_ptr;
    va_start(arg_ptr, sch);
    int count = sprintf(json_sample, sch, arg_ptr);
    // Send the value through the stream
    send(uri, json_sample, count);
    va_end(arg_ptr);
  }

  /**
   * Sends the sample information through the stream.
   *
   * @param uri the uri of the target stream where data will be feed.
   * @param sample the sample of data to be sent to the stream.
   */
  inline void feed(const SensorSample &sample) {
    // Stringify the sample and send it through the stream
    sample.toString(id_, json_sample);
    String sampleString= String(json_sample);
    int count = sampleString.length();
    send(sample.stream_, json_sample, count);
  }

  /**
   * Feeds the stream with a given sample of data.
   *
   * @param uri the uri of the target stream where data will be feed.
   * @param data data to send to the stream.
   */
  inline void send(const char *uri, const char *data, int length) {
    //p_.print("POST "); p_.print(uri); p_.println(" HTTP/1.1");            // POST URI
    p_.println(String("POST ")+ uri + " HTTP/1.1");            // POST URI
    p_.print("Host:"); p_.print(host_); p_.print(":"); p_.println(port_); // Host header
    p_.println("Content-Type: application/json");                         // JSON content type
    p_.print("Content-Length:"); p_.println(length);                      // Content length
    p_.println();                                                         // End of headers
    p_.println(data);                                                     // POST message body
  }

  // ---------------------------------------------------------------------------------------------------------------------
  // Public attributes
  // ---------------------------------------------------------------------------------------------------------------------
 public:
  T &p_;
  const char *id_;

  // ---------------------------------------------------------------------------------------------------------------------
  // Protected attributes
  // ---------------------------------------------------------------------------------------------------------------------
 protected:
  const char *host_;
  int port_;
};

} // iot
} // valo
} // io

#endif // IO_VALO_CONTRIBUTOR_H
