#include "motionSensor.h"
#include <utils/tcpRobotCommunicator.h>
#include <qrutils/inFile.h>

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

MotionSensor::MotionSensor(DeviceInfo const &info, PortInfo const &port
	, utils::TcpRobotCommunicator &tcpRobotCommunicator)
	: robotModel::parts::TrikMotionSensor(info, port)
		, mRobotCommunicator(tcpRobotCommunicator)
{
}

void MotionSensor::read()
{
	QString const pathToCommand = ":/trik/templates/wait/motion.t";
	QString const directCommand = utils::InFile::readAll(pathToCommand) + "brick.run()";
	mRobotCommunicator.runDirectCommand(directCommand);

	/// @todo: feedback from trikRuntime. Now, the command is only sent there
	emit newData(0);
}
