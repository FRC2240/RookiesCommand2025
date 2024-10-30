#pragma once

#include <frc2/command/CommandPtr.h>
#include <frc2/command/Commands.h>
#include <frc2/command/RunCommand.h>
#include <frc2/command/ConditionalCommand.h>
#include <frc2/command/SubsystemBase.h>
#include <units/length.h>
#include <rev/CANSparkMax.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <vector>
#include <units/length.h>


class Vision : frc2::SubsystemBase
{
public:
  units::meter_t getDistanceToTarget();

private:
  std::shared_ptr<nt::NetworkTable> m_left_table =
      nt::NetworkTableInstance::GetDefault().GetTable("limelight-dev");
  std::vector<double> m_zero_vector = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
};