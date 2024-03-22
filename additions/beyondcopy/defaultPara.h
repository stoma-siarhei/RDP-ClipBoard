
//the head file is used to define some default value
const int con_port = 3333;
const int con_max_port = 9999;
const int con_min_port = 2000;
const AnsiString con_tray_tip = "BeyondCopy";

const AnsiString con_connected = "Connected";//beyondcopy can connected
const AnsiString con_disconnected = "Not Connected";

const int con_max_infolines = 100;

const int con_max_clip = 2048;//the largest sendable clip text size
const int con_max_send = con_max_clip + 1;//the largest send buffer size

const int con_send_timer = 800;//0.8 second
const int con_connect_timer = 20 * 1000;//20 second
