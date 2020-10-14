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

void Utils::deleteParsedMessageList(LinkedList<ParsedMessage> *linkedList)
{
  if (linkedList != NULL)
  {
    if (linkedList->size() > 0)
    {
      for (int i = 0; i < linkedList->size(); i++)
      {
        // for(int j = 0; j < linkedList->get(i).options->size(); j++){
        //   delete(&linkedList->get(i).options->get(j));
        // }
        linkedList->get(i).options->clear();
        delete (linkedList->get(i).options);
      }
      linkedList->clear();
    }
    delete (linkedList);
  }
}

void Utils::deleteParsedMessage(ParsedMessage *parsedMessage)
{
  if (parsedMessage != NULL)
  {
    if (parsedMessage->options != NULL)
    {
      if (parsedMessage->options->size() > 0)
      {
        parsedMessage->options->clear();
        delete(parsedMessage->options);
      }
      delete (parsedMessage);
    }
  }
}