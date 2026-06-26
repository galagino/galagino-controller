#include "dump-nvs.h"

#include <nvs_flash.h>
#include <nvs.h>

extern "C" {
#include "btstack_tlv.h"
#include "classic/btstack_link_key_db.h"
#include "classic/btstack_link_key_db_tlv.h"
#include "ble/le_device_db.h"
#include "hci.h"

#include "classic/btstack_link_key_db.h"
#include "classic/btstack_link_key_db_memory.h"
#include "ble/le_device_db.h"
}

#define p Console

void dumpNVS() {

  p.printf("NVS dump ----------------\n");

  nvs_iterator_t it = nvs_entry_find("nvs", NULL, NVS_TYPE_ANY);
  if (it) {
    while (it != NULL) {
      nvs_entry_info_t info;
      nvs_entry_info(it, &info);
      p.printf("Namespace: [%-15s] | Key: %-15s | Type: 0x%02X\n", 
               info.namespace_name, info.key, info.type);
      it = nvs_entry_next(it);
    }
  }
  p.printf("NVS dump ----------------\n");
}

static void dump_classic_key_callback(bd_addr_t bd_addr, link_key_t link_key, link_key_type_t type, void * context) {
    p.print("Device Addr: ");
    for (int i = 0; i < 6; i++) {
        p.printf("%02X", bd_addr[i]);
        if (i < 5) p.print(":");
    }
    
    p.print(" | Link Key: ");
    for (int i = 0; i < 16; i++) {
        p.printf("%02X ", link_key[i]);
    }
    
    p.printf("| Type: %d\n", type);
}

void dumpBT() {
  p.printf("BT dump ----------------\n");

  const btstack_tlv_t * tlv_impl = NULL;
  void * tlv_context = NULL;
  btstack_tlv_get_instance(&tlv_impl, &tlv_context);

  if (tlv_impl) {

    const btstack_link_key_db_t * link_key_db = btstack_link_key_db_tlv_get_instance(tlv_impl, tlv_context);
        
        if (link_key_db && link_key_db->iterator_init && link_key_db->iterator_get_next) {
            btstack_link_key_iterator_t it;
            link_key_db->iterator_init(&it);
            
            bd_addr_t addr;
            link_key_t key;
            link_key_type_t type;
            int count = 0;
            
            // Loop until iterator_get_next evaluates to false (0)
            while (link_key_db->iterator_get_next(&it, addr, key, &type)) {
                dump_classic_key_callback(addr, key, type, NULL);
                count++;
            }
            
            if (count == 0) {
                Serial.println("No Classic Link Keys found inside the active TLV Database configuration.");
            }

    }
  }


  p.printf("BT dump ----------------\n");

}
