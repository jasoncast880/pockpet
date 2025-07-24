#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/ip4_addr.h"
#include "lwip/prot/ip4.h"
#include "lwip/icmp.h"
#include "lwip/inet_chksum.h"
#include "lwip/raw.h"
#include "lwip/timeouts.h"

#include "FreeRTOS.h"
#include "task.h"

#include "secrets.h"

// Change this to your target IP
#define TARGET_IP "8.8.8.8"
#define PING_ID 0xAFAF
#define PING_DATA_SIZE 32

static struct raw_pcb *ping_pcb = NULL;
static ip_addr_t target_addr;

static uint16_t ping_seq_num = 0;

static err_t ping_recv(void *arg, struct raw_pcb *pcb, struct pbuf *p, const ip_addr_t *addr) {
    struct icmp_echo_hdr *iecho;

    if (p == NULL) return ERR_OK;

    if (p->len >= sizeof(struct icmp_echo_hdr)) {
        iecho = (struct icmp_echo_hdr *)p->payload;

        if ((iecho->id == lwip_htons(PING_ID)) && (iecho->seqno == lwip_htons(ping_seq_num))) {
            printf("Ping reply from %s: seq=%d\n", ipaddr_ntoa(addr), lwip_htons(iecho->seqno));
        }
    }

    pbuf_free(p);
    return ERR_OK;
}

static void send_ping(void) {
    struct pbuf *p;
    struct icmp_echo_hdr *iecho;
    size_t icmp_len = sizeof(struct icmp_echo_hdr) + PING_DATA_SIZE;

    p = pbuf_alloc(PBUF_IP, (u16_t)icmp_len, PBUF_RAM);
    if (!p) return;

    iecho = (struct icmp_echo_hdr *)p->payload;

    ICMPH_TYPE_SET(iecho, ICMP_ECHO);
    ICMPH_CODE_SET(iecho, 0);

    iecho->id = lwip_htons(PING_ID);
    iecho->seqno = lwip_htons(++ping_seq_num);

    memset((uint8_t *)iecho + sizeof(struct icmp_echo_hdr), 0xAA, PING_DATA_SIZE);

    iecho->chksum = 0;
    iecho->chksum = inet_chksum(iecho, icmp_len);

    raw_sendto(ping_pcb, p, &target_addr);
    pbuf_free(p);

    printf("Ping sent to %s: seq=%d\n", ipaddr_ntoa(&target_addr), ping_seq_num);
}

void ping_task(void *params) {
    ping_pcb = raw_new(IP_PROTO_ICMP);
    raw_recv(ping_pcb, (raw_recv_fn)ping_recv, NULL);
    raw_bind(ping_pcb, IP_ADDR_ANY);

    ip4addr_aton(TARGET_IP, &target_addr);

    while (1) {
        cyw43_arch_lwip_begin();
        send_ping();
        cyw43_arch_lwip_end();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void main_task(void *params) {
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed!\n");
        while (1);
    }

    printf("A\n");

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID,WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Wi-Fi connect failed\n");
        while (1);
    }

    printf("B\n");

    printf("Wi-Fi connected with IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));

    printf("C\n");

    xTaskCreate(ping_task, "ping", 1024, NULL, 1, NULL);

    printf("D\n");

    vTaskDelete(NULL);  // Done with main task
}

int main() {
    stdio_init_all();

    sleep_ms(5000);
    printf("GO\n");

    xTaskCreate(main_task, "main", 2048, NULL, 1, NULL);
    vTaskStartScheduler();

    while (1); // Should not reach
}

