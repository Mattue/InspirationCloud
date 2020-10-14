#include <utils\LedUtils.h>

unsigned int LedUtils::hexToDec(String hexString)
{

  if (hexString.equals(""))
    return -1;

  unsigned int decValue = 0;
  int nextInt;

  for (int i = 0; i < hexString.length(); i++)
  {

    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57)
      nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70)
      nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102)
      nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);

    decValue = (decValue * 16) + nextInt;
  }

  return decValue;
}

CRGB::HTMLColorCode LedUtils::colorToHex(String color)
{
  color.toUpperCase();

  if (color.equals("RED"))
  {
    return CRGB::Red;
  }
  else if (color.equals("BLUE"))
  {
    return CRGB::Blue;
  }
  else if (color.equals("GREEN"))
  {
    return CRGB::Green;
  }
  else
  {
    return CRGB::Black;
  }
}