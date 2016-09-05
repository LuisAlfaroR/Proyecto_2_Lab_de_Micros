
#ifndef DISPLAY_H
#define DISPLAY_H
#include <string.h>

using namespace std;

class Display
{
  public: 
      Display(int width, int height, const string& title);
      virtual ~Display();

  protected:
  private:
      Display(const Display& other) {}
      Display& operator=(const Display& other) {}
};

#endif // DISPLAY_H
