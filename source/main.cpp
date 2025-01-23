#include <notifications/notifications.h>
#include <sstream>
#include <string>
#include <wups.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include<memory>
#include <thread>
#include<coreinit/time.h>
#include<coreinit/thread.h>
#include<errno.h>
#include <arpa/inet.h>
#include <wups/config/WUPSConfigItemBoolean.h>
#include <wups/config/WUPSConfigItemMultipleValues.h>
#include <wups/config/WUPSConfigItemStub.h>
#include <nn/ac.h>
  #include <poll.h>
#include<coreinit/title.h>
#include<coreinit/dynload.h>
#include<coreinit/memorymap.h>
#include<lib.h>
#include"patching.h"
#include <chrono>
#include"kernel.h"
WUPS_PLUGIN_NAME("Splatoon Private Battles Alone");
WUPS_PLUGIN_DESCRIPTION("Enables playing private battles alone in splatoon");
//WUPS_PLUGIN_VERSION(VERSION_FULL);
WUPS_PLUGIN_AUTHOR("Patch: Shadow, Plugin: DJMrTV");
WUPS_PLUGIN_LICENSE("GPL");
WUPS_USE_WUT_DEVOPTAB ();


using namespace std::chrono_literals;

bool is_current_game_menu_app(){
  auto current_game = OSGetTitleID();

  if(
    current_game == 0x0005000010162B00 ||
    current_game == 0x0005000010176900 ||
    current_game == 0x0005000010176A00
  ){
    return true;
  }
  return false;
}


INITIALIZE_PLUGIN() {
  //NotificationModule_AddInfoNotification("HEHEHE(COLDBOOTSHIT)");
  if (NotificationModule_InitLibrary() != NOTIFICATION_MODULE_RESULT_SUCCESS) {
  }
}



ON_APPLICATION_START() {
  //await_connections();
  if(is_current_game_menu_app()){
    NotificationModule_AddInfoNotification("Splatoon Detected, applying pbs alone patch");
    
    auto rpl_count = OSDynLoad_GetNumberOfRPLs();

    std::vector<OSDynLoad_NotifyData> rpls(rpl_count);

    if(!OSDynLoad_GetRPLInfo(0, rpl_count, rpls.data())){
      NotificationModule_AddInfoNotification("failed to get rpls");  
    }

    for(auto rpl: rpls){
      if(rpl.name != 0){
        if(strstr(rpl.name, "Gambit") != NULL){
          game_text_region = (void*)rpl.textAddr;

          game_data_region = (void*)rpl.dataAddr;

          //this point in memory is still mapped to physical so im jusst gonna use it for myself lmao
          current_codecave_writer_location = (uint32_t*)(rpl.textAddr + rpl.textSize + 0x1000);
          
          patch_text_location(0x00175B90, NOP());
          patch_text_location(0x00092C54, NOP());
          patch_text_location(0x008294B8, NOP());
        }
      }
    }
  }
}
