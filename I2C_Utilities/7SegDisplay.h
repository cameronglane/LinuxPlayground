#ifndef I2CPLAYGROUND_12CUTILITIES_7SEGDISPLAY_H_
#define I2CPLAYGROUND_12CUTILITIES_7SEGDISPLAY_H_

int Change7SegAddress(int old_address, const char new_address);
int Clear7Seg(int address);
int WriteTo7Seg(const int address, const char buffer);
#endif // I2CPLAYGROUND_12CUTILITIES_7SEGDISPLAY_H_
