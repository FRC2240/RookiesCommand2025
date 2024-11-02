// if 6.5-7 ft to target
// spin 1250 +- 1% rpm
#pragma once

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableInstance.h>
#include <vector>
#include <units/length.h>
#include <frc2/command/SubsystemBase.h>

class Vision : public frc2::SubsystemBase
{
public:
    void Periodic() override;
    bool InRange(units::meter_t min, units::meter_t max);
    units::meter_t getDistanceToTarget();

private:
    units::meter_t m_distance;

    std::shared_ptr<nt::NetworkTable> m_left_table =
        nt::NetworkTableInstance::GetDefault().GetTable("limelight-dev");
    std::vector<double> m_zero_vector = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
};