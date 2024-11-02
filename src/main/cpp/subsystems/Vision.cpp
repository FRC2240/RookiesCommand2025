#include "subsystems/Vision.h"
#include <frc/smartdashboard/SmartDashboard.h>

units::meter_t Vision::getDistanceToTarget() {
    //Is target valid?
    if ((m_left_table->GetNumber("tv", 0.0) > 0.5) and (m_left_table->GetNumber("tid", 0.0) == 8)) {
        //Get target data
        auto target = m_left_table->GetNumberArray("camerapose_targetspace", m_zero_vector);

        auto distance = units::meter_t(sqrt((target[0] * target[0]) + (target[1] * target[1]) + (target[2] * target[2])));

        return distance;
    }

    return 0.0_m;
}

void Vision::Periodic() {
    m_distance = getDistanceToTarget();
    frc::SmartDashboard::PutNumber("Target Distance", m_distance.value());
}

bool Vision::InRange(units::meter_t min, units::meter_t max) {
    if ((m_distance <= max) && (m_distance >= min)) {
        return true;
    }

    return false;
}
