#ifndef WEB_SERVER_H
#define WEB_SERVER_H

typedef void (*web_server_app_cb)(bool);

void web_server_init(web_server_app_cb);
void web_server_update_water_valve(bool new_state);

#endif //WEB_SERVER_H
