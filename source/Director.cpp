#include "Director.h"

template<> Director* Ogre::Singleton<Director>::ms_Singleton = 0;



Director::Director(){}
Director::~Director(){}
int Director::playNextScenario(){}
int Director::playScenario(int ss){}
int Director::addScenario( const std::string& scenarioFileName ){}
int Director::addScenario( const std::string& scenarioFileName,int ss ){}

int Director::removeScenario(){}
int Director::removeScenario(int ss ){}
int Director::clearScenarios(){}
  

