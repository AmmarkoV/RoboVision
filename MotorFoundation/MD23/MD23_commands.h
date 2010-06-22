#ifndef MD23_COMMANDS_H_INCLUDED
#define MD23_COMMANDS_H_INCLUDED


#include "MD23.h"
int MD23_ok(struct md23_device * dev);
int MD23_Zero_encoders(struct md23_device * dev,char nocheck);
int MD23_Stop_motors(struct md23_device * dev,char nocheck);
int MD23_search_on_I2C(struct md23_device * dev);
void MD23_MarkMotorShut(struct md23_device * dev,unsigned char motor_id);
int MD23_movemotor(struct md23_device * dev,unsigned char motor_id,unsigned char power1,unsigned char power2);
int MD23_ReadStatus(struct md23_device * dev,char nocheck);


#endif // MD23_COMMANDS_H_INCLUDED
