#include "sdc_handler.h"
#include "ff.h"

FATFS fs;
FRESULT fr;

void sdc_setup() {
//    xFileOps_Queue = xQueueCreate((UBaseType_t)10, sizeof());
    
//    x_sdcSemphr = xSemaphoreCreateMutex();

    FRESULT fr = f_mount(&fs, "0:", 1); //mounting sdc
    if (fr!=FR_OK) {
        printf("sdc messed up");
    } else {
        printf("sdc OK");
    }
    
    xTaskCreate( sdc_access_file_task, "filetask", 1000, NULL, 1, NULL );
}

void sdc_access_file_task(void* pvParameters) {
    for(;;) {
        FIL fil;
        fr = f_open( &fil, "0:/data.txt", FA_READ);
    }
}

void sdc_write_file_task(void* pvParameters) {
//    f_write(FIL *fp, const void *buff, UINT btw, UINT *bw)
}
