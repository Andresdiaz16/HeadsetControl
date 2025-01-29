#include "../device.h"
#include "../utility.h"


#include <hidapi.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define  ID_AUDEZE_MAXWELL_WIRED       0x4b1a
#define  ID_AUDEZE_MAXWELL_WIRELESS    0x4b19

static struct device device_maxwell;
static BatteryInfo audeze_maxwell_get_battery(hid_device* device_handle);

static const uint16_t PRODUCT_IDS[] = { 
    ID_AUDEZE_MAXWELL_WIRED,
    ID_AUDEZE_MAXWELL_WIRELESS
};

void audeze_maxwell_init(struct device** device)
{
    device_maxwell.idVendor =  VENDOR_AUDEZE;
    device_maxwell.idProductsSupported = PRODUCT_IDS;
    device_maxwell.numIdProducts = sizeof(PRODUCT_IDS) / sizeof(PRODUCT_IDS[0]);
    strcpy(device_maxwell.device_name, "Audeze Maxwell");

    device_maxwell.capabilities =B(CAP_BATTERY_STATUS);
    device_maxwell.request_battery = &audeze_maxwell_get_battery;
    *device = &device_maxwell;
}



static BatteryInfo audeze_maxwell_get_battery(hid_device* device_handle)
{
    BatteryInfo info = { .status = BATTERY_HIDERROR, .level = -1 };

    uint8_t request_battery_status [63] = { 0x06, 0x08, 0x80, 0x05, 0x5a, 0x03, 0x00, 0xd6, 0x0c, 0x00};

    //request read report 7
    uint8_t data_response[63] = {0x07}; 

    int response_request_battery = hid_send_feature_report(device_handle, request_battery_status, sizeof(request_battery_status));
    hid_get_input_report(device_handle, data_response, 63);

    int response = hid_get_input_report(device_handle, data_response, 63);

    if(response < 0 ){
        info.status = BATTERY_HIDERROR;
        return info;
    }

    info.status = BATTERY_AVAILABLE;
    info.level = (int)data_response[18];

    /*if (response == 0){*/
    /*    info.status = BATTERY_TIMEOUT;*/
    /*    return info;*/
    /*}*/
    /**/
    /*if (response < 0) {*/
    /*    return  info;*/
    /*}*/
    /**/
    /*uint8_t  data_read[62];*/
    /*response = hid_read_timeout(device_handle, data_read, 62, TIMEOUT); */
    /**/
    /*if (response < 0) {*/
    /*    return  info;*/
    /*}*/
    /**/
    /*if (response == 0) {*/
    /*    return  info;*/
    /*}*/
    /**/
    /*printf("BATTERY INFO AUDEZE: ");*/
    /**/
    for(int i = 0; i < 63; i++) {
        printf("%d %hhx ", i,data_response[i]);
        printf("\n");
    } 
    /**/
    /**/
    return  info;
}
