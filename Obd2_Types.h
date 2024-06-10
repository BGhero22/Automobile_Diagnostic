#ifndef OBD2_TYPES_H
#define OBD2_TYPES_H
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define MAX_UDS_RESPONSE_PAYLOAD_LENGTH 127
#define MAX_UDS_REQUEST_PAYLOAD_LENGTH 7

/***********************************************************************************************************************
Type definitions
***********************************************************************************************************************/
typedef struct{
    uint32_t can_id;
    uint8_t mode;
    bool has_pid;
    uint16_t pid;
    uint8_t pid_length;
    uint8_t payload[MAX_UDS_REQUEST_PAYLOAD_LENGTH];
    uint8_t payload_length;
   
    
} DiagnosticRequest;

typedef enum{
    NRC_SUCCESS = 0x0,
    NRC_SERVICE_NOT_SUPPORTED = 0x11,
    NRC_SUB_FUNCTION_NOT_SUPPORTED = 0x12,
    NRC_INCORRECT_LENGTH_OR_FORMAT = 0x13,
    NRC_CONDITIONS_NOT_CORRECT = 0x22,
    NRC_REQUEST_OUT_OF_RANGE = 0x31,
    NRC_SECURITY_ACCESS_DENIED = 0x33,
    NRC_INVALID_KEY = 0x35,
    NRC_TOO_MANY_ATTEMPS = 0x36,
    NRC_TIME_DELAY_NOT_EXPIRED = 0x37,
    NRC_RESPONSE_PENDING = 0x78
} DiagnosticNegativeResponseCode;


typedef struct  {
    uint32_t can_id;
    uint8_t mode;
    bool has_pid;
    uint16_t pid;
    DiagnosticNegativeResponseCode negative_response_code;
    uint8_t payload[MAX_UDS_RESPONSE_PAYLOAD_LENGTH];
    uint8_t payload_length;
}DiagnosticResponse;


typedef enum {
    OBD2_MODE_POWERTRAIN_DIAGNOSTIC_REQUEST = 0x1,
    OBD2_MODE_EMISSIONS_DTC_REQUEST = 0x3,
    OBD2_MODE_EMISSIONS_DTC_CLEAR = 0x4
    
}DiagnosticModeEnum;

#endif // OBD2_TYPES_H
