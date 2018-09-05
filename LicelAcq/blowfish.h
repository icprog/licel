#ifndef __BLOWFISH
#define __BLOWFISH

void Blowfish_encipher(unsigned long *xl, unsigned long *xr);

void Blowfish_decipher(unsigned long *xl, unsigned long *xr);

short InitializeBlowfish(const char* key, short keybytes);
#endif
