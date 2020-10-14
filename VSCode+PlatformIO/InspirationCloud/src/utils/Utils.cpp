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

void Utils::deleteParsedMessageList(LinkedList<ParsedMessage> *linkedList)
{
  if (linkedList != NULL)
  {
    if (linkedList->size() > 0)
    {
      for (int i = 0; i < linkedList->size(); i++)
      {
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