#ifndef BMP180_h
#define BMP180_h

char debug(char *fmt, ... );
double calculate_sea_level(double p,double alt);
void readSensor();
void  getCalibrationData() ;

#endif
