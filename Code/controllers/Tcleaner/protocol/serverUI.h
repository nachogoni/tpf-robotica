#ifndef _SERVERUI_H
#define _SERVERUI_H

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
void cmd_dsEnable(char * data);
void cmd_dsDisable(char * data);
void cmd_dsSetAll(char * data); 
void cmd_dsGetValue(char * data);
void cmd_dsGetAllValue(char * data);
void cmd_dsGetOneValue(char * data);
void cmd_dsGetOneValueAll(char * data);
// Commands for FloorSensor (fs)      
void cmd_fsEnable(char * data);
void cmd_fsDisable(char * data);
void cmd_fsSetAll(char * data);
void cmd_fsGetValue(char * data);
void cmd_fsGetAllValue(char * data);
void cmd_fsGetOneValue(char * data);
void cmd_fsGetOneValueAll(char * data);
// Commands for UltraSonicSensor (us)
void cmd_usEnable(char * data);
void cmd_usDisable(char * data);
void cmd_usSetAll(char * data);
void cmd_usGetValue(char * data);
void cmd_usGetAllValue(char * data);
void cmd_usGetOneValue(char * data);
void cmd_usGetOneValueAll(char * data);
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