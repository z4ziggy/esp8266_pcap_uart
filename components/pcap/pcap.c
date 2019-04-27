/* pcap encoder.
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "driver/uart.h"
#include "pcap.h"

/**
 * @brief Pcap File Header Type Definition
 *
 */
typedef struct {
    uint32_t magic;     /*!< Magic Number */
    uint16_t major;     /*!< Major Version */
    uint16_t minor;     /*!< Minor Version */
    uint32_t zone;      /*!< Time Zone Offset */
    uint32_t sigfigs;   /*!< Timestamp Accuracy */
    uint32_t snaplen;   /*!< Max Length to Capture */
    uint32_t link_type; /*!< Link Layer Type */
} pcap_file_header_t;

/**
 * @brief Pcap Packet Header Type Definition
 *
 */
typedef struct {
    uint32_t seconds;        /*!< Number of seconds since January 1st, 1970, 00:00:00 GMT */
    uint32_t microseconds;   /*!< Number of microseconds when the packet was captured(offset from seconds) */
    uint32_t capture_length; /*!< Number of bytes of captured data, not longer than packet_length */
    uint32_t packet_length;  /*!< Actual length of current packet */
} pcap_packet_header_t;

esp_err_t pcap_capture_packet(void *payload, uint32_t length, uint32_t seconds, uint32_t microseconds)
{
    pcap_packet_header_t header = {
        .seconds = seconds,
        .microseconds = microseconds,
        .capture_length = length,
        .packet_length = length
    };
    uart_write_bytes(UART_NUM_0, (const char *) &header, sizeof(header));
    uart_write_bytes(UART_NUM_0, (const char *) payload, length);
    uart_flush(UART_NUM_0);
    return ESP_OK;
}

esp_err_t pcap_start(void)
{
    /* Write Pcap File header */
    pcap_file_header_t header = {
        .magic = PCAP_MAGIC_BIG_ENDIAN,
        .major = PCAP_VERSION_MAJOR,
        .minor = PCAP_VERSION_MINOR,
        .zone = PCAP_TIME_ZONE_GMT,
        .sigfigs = 0,
        .snaplen = sizeof(uint32_t), //0x40000,
        .link_type = PCAP_LINK_TYPE_802_11
    };
    uart_write_bytes(UART_NUM_0, (const char *) &header, sizeof(header));
    uart_flush(UART_NUM_0);
    return ESP_OK;
}
