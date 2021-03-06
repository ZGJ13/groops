/***********************************************/
/**
* @file loopFileGnssStationInfo.h
*
* @brief Loop over receivers or antennas in a GNSS station/transmitter info file.
*
* @author Sebastian Strasser
* @date 2018-01-29
*
*/
/***********************************************/

#ifndef __GROOPS_LOOPFILEGNSSSTATIONINFO__
#define __GROOPS_LOOPFILEGNSSSTATIONINFO__

// Latex documentation
#ifdef DOCSTRING_Loop
static const char *docstringLoopFileGnssStationInfo = R"(
\subsection{FileGnssStationInfo}
Loop over receivers or antennas in a GNSS station/transmitter info \file{file}{gnssStationInfo}.
)";
#endif

/***********************************************/

#include "base/import.h"
#include "classes/loop/loop.h"
#include "files/fileGnssStationInfo.h"

/***** CLASS ***********************************/

/** @brief Loop over receivers or antennas in a GNSS station/transmitter info file.
* @ingroup LoopGroup
* @see Loop */
class LoopFileGnssStationInfo : public Loop
{
  std::string     nameName, nameSerial, nameInfo, nameTimeStart, nameTimeEnd, nameIndex, nameCount;
  Bool            useAntennas;
  GnssStationInfo info;
  UInt            index;

public:
  LoopFileGnssStationInfo(Config &config);

  UInt count() const override {return (useAntennas ? info.antenna.size() : info.receiver.size());}
  Bool iteration(VariableList &varList) override;
};

/***********************************************/
/***** Inlines *********************************/
/***********************************************/

inline LoopFileGnssStationInfo::LoopFileGnssStationInfo(Config &config)
{
  try
  {
    FileName    fileName;
    std::string choice;

    readConfig(config, "inputfileGnssStationInfo", fileName, Config::MUSTSET, "", "station/transmitter info file");
    if(readConfigChoice(config, "infoType", choice, Config::MUSTSET, "", "info to loop over"))
    {
      if(readConfigChoiceElement(config, "antenna",  choice, "loop over antennas"))  useAntennas = TRUE;
      if(readConfigChoiceElement(config, "receiver", choice, "loop over receivers")) useAntennas = FALSE;
      endChoice(config);
    }
    readConfig(config, "variableLoopName",      nameName,            Config::OPTIONAL,  "loopName",      "variable with antenna/receiver name");
    readConfig(config, "variableLoopSerial",    nameSerial,          Config::OPTIONAL,  "loopSerial",    "variable with antenna/receiver serial");
    readConfig(config, "variableLoopInfo",      nameInfo,            Config::OPTIONAL,  "loopInfo",      "variable with radome (antenna) or version (receiver)");
    readConfig(config, "variableLoopTimeStart", nameTimeStart,       Config::OPTIONAL,  "loopTimeStart", "variable with antenna/receiver start time");
    readConfig(config, "variableLoopTimeEnd",   nameTimeEnd,         Config::OPTIONAL,  "loopTimeEnd",   "variable with antenna/receiver end time");
    readConfig(config, "variableLoopIndex",     nameIndex,           Config::OPTIONAL,  "",              "variable with index of current iteration (starts with zero)");
    readConfig(config, "variableLoopCount",     nameCount,           Config::OPTIONAL,  "",              "variable with total number of iterations");
    if(isCreateSchema(config)) return;

    readFileGnssStationInfo(fileName, info);
    index = 0;
  }
  catch(std::exception &e)
  {
    GROOPS_RETHROW(e)
  }
}

/***********************************************/

inline Bool LoopFileGnssStationInfo::iteration(VariableList &varList)
{
  if(index >= count())
    return FALSE;

  if(!nameName.empty())      addVariable(nameName,      useAntennas ? info.antenna.at(index).name            : info.receiver.at(index).name,            varList);
  if(!nameSerial.empty())    addVariable(nameSerial,    useAntennas ? info.antenna.at(index).serial          : info.receiver.at(index).serial,          varList);
  if(!nameInfo.empty())      addVariable(nameInfo,      useAntennas ? info.antenna.at(index).radome          : info.receiver.at(index).version,         varList);
  if(!nameTimeStart.empty()) addVariable(nameTimeStart, useAntennas ? info.antenna.at(index).timeStart.mjd() : info.receiver.at(index).timeStart.mjd(), varList);
  if(!nameTimeEnd.empty())   addVariable(nameTimeEnd,   useAntennas ? info.antenna.at(index).timeEnd.mjd()   : info.receiver.at(index).timeEnd.mjd(),   varList);
  if(!nameIndex.empty())     addVariable(nameIndex,     index,   varList);
  if(!nameCount.empty())     addVariable(nameCount,     count(), varList);

  index++;
  return TRUE;
}

/***********************************************/

#endif
