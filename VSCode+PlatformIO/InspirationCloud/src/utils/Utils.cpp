#include "Utils.h"

String Utils::getNamedOptionValue(LinkedList<Option> *options, String optionName)
{
    for (int i = 0; i < options->size(); i++)
    {
        if (options->get(i).option.equals(optionName))
        {
            return options->get(i).value;
        }
    }

    return "";
}

unsigned int Utils::hexToDec(String hexString)
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