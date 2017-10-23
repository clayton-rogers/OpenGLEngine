// This file includes all the random bits of implementation for the
// stock provided systems

#include "Stock/CameraComponent.h"
#include "Stock/GeneralDrawComponent.h"
#include "Stock/InertialComponents.h"
#include "Stock/RotationalComponents.h"

ComponentEnum CameraComponent::type = ComponentEnum(0);

ComponentEnum GeneralDrawComponent::type = ComponentEnum(0);

ComponentEnum PositionComponent::type = ComponentEnum(0);
ComponentEnum VelocityComponent::type = ComponentEnum(0);
ComponentEnum MassComponent::type = ComponentEnum(0);

ComponentEnum AngularPositionComponent::type = ComponentEnum(0);
ComponentEnum AngularVelocityComponent::type = ComponentEnum(0);
ComponentEnum MomentofInertiaComponent::type = ComponentEnum(0);
