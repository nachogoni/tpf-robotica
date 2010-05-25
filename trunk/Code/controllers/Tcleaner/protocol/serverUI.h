#ifndef _SERVERUI_H
#define _SERVERUI_H

void registerHandlers(protocol::PacketServer * ps);
void createPacketBoards();

// Commands for Main Controller (mc)
void cmd_init(char * data);
void cmd_reset(char * data);
void cmd_ping(char * data);
void cmd_error(char * data);
void cmd_setDC(char * data);
void cmd_setDS(char * data);
void cmd_debug(char * data);
// Commands for MotorDC (dc)                                                                      
void cmd_dcSetDirection(char * data);
void cmd_dcSetSpeed(char * data);    
void cmd_dcSetEncoder(char * data);  
void cmd_dcGetEncoder(char * data);  
void cmd_dcResetEncoder(char * data);
void cmd_dcSetEncoderToStop(char * data);
void cmd_dcGetEncoderToStop(char * data);
void cmd_dcDontStop(char * data);        
void cmd_dcConsumption(char * data);     
void cmd_dcStressAlarm(char * data);     
void cmd_dcShutDownAlarm(char * data);   
void cmd_dcGetSpeed(char * data);        
// Commands for ServoMotor (sm)
void cmd_smSetPos(char * data);
void cmd_smSetAllPos(char * data);
void cmd_smGetPos(char * data);   
void cmd_smGetAllPos(char * data);
void cmd_smSetSpeed(char * data); 
void cmd_smSetAllSpeed(char * data);
void cmd_smGetSpeed(char * data);   
void cmd_smGetAllSpeed(char * data);
void cmd_smFree(char * data);       
void cmd_smFreeAll(char * data);    
// Commands for DistanceSensor (ds)
void cmd_dsOn(char * data);
void cmd_dsOff(char * data);
void cmd_dsSetMask(char * data);
void cmd_dsGetMask(char * data);
void cmd_dsGetValue(char * data);
void cmd_dsGetOneValue(char * data);
void cmd_dsAlarmOn(char * data);
void cmd_dsAlarmCommand(char * data);
// Commands for BatteryController (bc)
void cmd_bcEnable(char * data);
void cmd_bcDisable(char * data);
void cmd_bcGetValue(char * data);
void cmd_bcFullAlarm(char * data);
void cmd_bcSetEmptyValue(char * data);
void cmd_bcEmptyAlarm(char * data);
void cmd_bcSetFullValue(char * data);
// Commands for TrashBin (tb)
void cmd_tbGetValue(char * data);
void cmd_tbFullAlarm(char * data);
void cmd_tbSetFullValue(char * data);

#endif
