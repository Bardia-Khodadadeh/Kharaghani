#include "shareddata.h"

int SharedData::_encoder = 0;
int SharedData::_encode = 0;

Output* SharedData::_output = new Output();
GraderStatus SharedData::m_graderStatus = GraderStatus::Stopped;
CommunicationType SharedData::communicationType = CommunicationType::TCP;

SharedData::SharedData() {}
