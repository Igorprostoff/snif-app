#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "ssd1306/ssd1306.h"
#include "ssd1306/ssd1306.c"
#include "ssd1306/font8x8_basic.h"
#include "ssd1306/ssd1306_spi.c"
#include "ssd1306/ssd1306_i2c.c"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"

#define WIFI_CHANNEL_SWITCH_INTERVAL  (500)
#define WIFI_CHANNEL_MAX               (13)
SSD1306_t dev;

uint8_t level = 0, channel = 1;

static wifi_country_t wifi_country = {.cc="CN", .schan = 1, .nchan = 13};

typedef struct {
  unsigned frame_ctrl:16;
  unsigned duration_id:16;
  uint8_t addr1[6]; 
  uint8_t addr2[6]; 
  uint8_t addr3[6]; 
  unsigned sequence_ctrl:16;
  uint8_t addr4[6]; 
} wifi_ieee80211_mac_hdr_t;

typedef struct {
  wifi_ieee80211_mac_hdr_t hdr;
  uint8_t payload[0]; 
} wifi_ieee80211_packet_t;

static esp_err_t event_handler(void *ctx, system_event_t *event);
static void wifi_sniffer_init(void);
static void wifi_sniffer_set_channel(uint8_t channel);
static const char *wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type);
static void wifi_sniffer_packet_handler(void *buff, wifi_promiscuous_pkt_type_t type);

esp_err_t event_handler(void *ctx, system_event_t *event)
{
  return ESP_OK;
}

void wifi_sniffer_init(void)
{
  nvs_flash_init();
  tcpip_adapter_init();
  ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
  ESP_ERROR_CHECK( esp_wifi_set_country(&wifi_country) ); 
  ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
  ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_NULL) );
  ESP_ERROR_CHECK( esp_wifi_start() );
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&wifi_sniffer_packet_handler);
}

void wifi_sniffer_set_channel(uint8_t channel)
{
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
}

const char * wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type)
{
  switch(type) {
  case WIFI_PKT_MGMT: return "MGMT";
  case WIFI_PKT_DATA: return "DATA";
  default:  
  case WIFI_PKT_MISC: return "MISC";
  }
}

void wifi_sniffer_packet_handler(void* buff, wifi_promiscuous_pkt_type_t type)
{
  
  //if (type != WIFI_PKT_MGMT)
  //  return;

  
 

  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buff;
  const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
  const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;

if (hdr->addr2[0]==hdr->addr3[0] && 
  hdr->addr2[1]==hdr->addr3[1] && 
  hdr->addr2[2]==hdr->addr3[2] && 
  hdr->addr2[3]==hdr->addr3[3] && 
  hdr->addr2[4]==hdr->addr3[4] && 
  hdr->addr2[5]==hdr->addr3[5]) {
    return;
  }

  ssd1306_clear_screen(&dev, false);
  char str[100];
  sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x", 
  hdr->addr1[0],hdr->addr1[1],hdr->addr1[2],
    hdr->addr1[3],hdr->addr1[4],hdr->addr1[5]);
  ssd1306_display_text(&dev, 0, str, strlen(str), false);

sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x", 
  hdr->addr2[0],hdr->addr2[1],hdr->addr2[2],
    hdr->addr2[3],hdr->addr2[4],hdr->addr2[5]);
  ssd1306_display_text(&dev, 1, str, strlen(str), false); 

  sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x", 
  hdr->addr3[0],hdr->addr3[1],hdr->addr3[2],
    hdr->addr3[3],hdr->addr3[4],hdr->addr3[5]);
  ssd1306_display_text(&dev, 2, str, strlen(str), false);

 sprintf(str, "RSSI: %02d", 
  ppkt->rx_ctrl.rssi);
  ssd1306_display_text(&dev, 3, str, strlen(str), false);

 sprintf(str, "Channel: %02d", 
  ppkt->rx_ctrl.channel);
  ssd1306_display_text(&dev, 4, str, strlen(str), false);

  printf("PACKET TYPE=%s, CHAN=%02d, RSSI=%02d,"
    " ADDR1=%02x:%02x:%02x:%02x:%02x:%02x,"
    " ADDR2=%02x:%02x:%02x:%02x:%02x:%02x,"
    " ADDR3=%02x:%02x:%02x:%02x:%02x:%02x\n",
    wifi_sniffer_packet_type2str(type),
    ppkt->rx_ctrl.channel,
    ppkt->rx_ctrl.rssi,
    /* ADDR1 */
    hdr->addr1[0],hdr->addr1[1],hdr->addr1[2],
    hdr->addr1[3],hdr->addr1[4],hdr->addr1[5],
    /* ADDR2 */
    hdr->addr2[0],hdr->addr2[1],hdr->addr2[2],
    hdr->addr2[3],hdr->addr2[4],hdr->addr2[5],
    /* ADDR3 */
    hdr->addr3[0],hdr->addr3[1],hdr->addr3[2],
    hdr->addr3[3],hdr->addr3[4],hdr->addr3[5]
  );
  vTaskDelay(200);
}

void button_check(void *p){
  for ( ; ; )
  {
  
  if (gpio_get_level(GPIO_NUM_0)==0){
      

    channel = (channel % WIFI_CHANNEL_MAX) + 1;
    printf("CHANGING CHANNEL TO %i \n", channel);
    wifi_sniffer_set_channel(channel);
    char str[100];
    sprintf(str, "SET CHANNEL #%i", channel);
    //ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 5, str, strlen(str), false);
    
  }
    vTaskDelay(300);

  }
}


void app_main(void) {

  spi_master_init(&dev, CONFIG_SCR_MOSI_GPIO, CONFIG_SCR_SCLK_GPIO, CONFIG_SCR_CS_GPIO, CONFIG_SCR_DC_GPIO, CONFIG_SCR_RESET_GPIO);

  ssd1306_init(&dev, 128, 64);
  
  ssd1306_contrast(&dev, 0xff);
  

  wifi_sniffer_init();
  
  xTaskCreate(&button_check, "button_check", 2048, NULL, 4, NULL);

  
}