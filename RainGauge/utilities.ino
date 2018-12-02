#include <ctype.h>

char * getCLOB(String fileName) {
  Serial.println("open"); delay(1000);
  File f = SPIFFS.open(fileName.c_str(), "r");
  if(!f) {
    // No file for some reason...
    return (char*) NULL;
  }

  uint16_t sizeCLOB = f.size();
    Serial.println("malloc"); delay(1000);
  char *p = (char*) malloc(sizeCLOB+1);
  if(!p) {
    // No memory probably. Make sure clobs are not to big...
    return (char*) NULL;
  }
    Serial.println("read"); delay(1000);

  char * pp = p;
  char c;
  while(c = f.read()>0) {
    if(isprint(c)) {
      *pp++ = c; *pp = 0;
    }
  }

  f.close();
  
 // *(p+sizeCLOB) = 0;  // Make sure 0 char is at the end...
  Serial.println("return"); delay(1000);
  return p;
}
