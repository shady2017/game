#include "camera_gen.h"

#include "core/InputEvent.h"
#include "core/Application.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "core/messages/messages.h"
#include "core/messages/messages_camera.h"

const float k_speed = 20.f;

glm::quat rotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
    using namespace glm;
    start = normalize(start);
    dest  = normalize(dest);

    float cosTheta = dot(start, dest);
    vec3  rotationAxis;

    if(cosTheta < -1 + 0.001f) {
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
        if(length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
            rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

        rotationAxis = normalize(rotationAxis);
        return angleAxis(180.0f, rotationAxis);
    }

    rotationAxis = cross(start, dest);

    float s    = sqrt((1 + cosTheta) * 2);
    float invs = 1 / s;

    return quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);
}

class HA_EMPTY_BASE camera : public camera_gen,
                             public InputEventListener,
                             public UpdatableMixin<camera>
{
    HA_MESSAGES_IN_MIXIN(camera)
public:
    camera() {
        //set_pos(ha_this, glm::vec3(0, 0, -40));
        set_pos(ha_this, glm::vec3(0, 50, 10));
        set_rot(ha_this, rotationBetweenVectors({0, 0, -1}, {0, -1, -0.2}));
    }

    void process_event(const InputEvent& ev) override {
        if(ev.type == InputEvent::MOTION) {
            cursor_x = float(ev.motion.x);
            cursor_y = float(ev.motion.y);
        }
    }

    void update(float dt) {
        uint32 w = Application::get().width();
        uint32 h = Application::get().height();
        if(cursor_x < 10)
            move(ha_this, glm::vec3(-k_speed * dt, 0, 0));
        if(cursor_x > w - 10)
            move(ha_this, glm::vec3(k_speed * dt, 0, 0));
        if(cursor_y < 10)
            move(ha_this, glm::vec3(0, 0, -k_speed * dt));
        if(cursor_y > h - 10)
            move(ha_this, glm::vec3(0, 0, k_speed * dt));
    }

    glm::mat4 get_view_matrix() {
        glm::mat4 t = glm::translate(glm::mat4(1.f), get_pos(ha_this));
        glm::mat4 r = glm::toMat4(get_rot(ha_this));
        return glm::inverse(t * r);
    }

    glm::mat4 get_projection_matrix() {
        uint32 w = Application::get().width();
        uint32 h = Application::get().height();
        return glm::perspective(glm::radians(45.0f), float(w) / float(h), 0.1f, 1000.0f);
    }
};

HA_MIXIN_DEFINE(camera, get_view_matrix_msg& get_projection_matrix_msg);
