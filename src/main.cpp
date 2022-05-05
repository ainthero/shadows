#include "Camera.h"
#include "Object.h"
#include "Shader.h"
#include "Light.h"
#include <memory>

std::shared_ptr<Camera> camera;
std::vector<Light> lights;

void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

void glfw_cursor_position_callback(GLFWwindow *window, double xPos, double yPos);

GLFWwindow *initGLWindow(GLuint width, GLuint height, const char *title) {
    if (!glfwInit()) {
        throw std::runtime_error("Error while initializing GLFW");
    }
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Error while creating window with GLFW");
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        glfwTerminate();
        throw std::runtime_error((const char *) glewGetErrorString(err));
    }
    return window;
}

void initGLFW(GLFWwindow *window) {
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
    glfwSetCursorPosCallback(window, glfw_cursor_position_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

int main() {
    const GLuint window_width = 1024;
    const GLuint window_height = 768;
    const GLuint shadow_res = 1024;
    const glm::mat4 identity_m = glm::mat4(1.0f);
    const GLfloat near_plane = 1.f;
    const GLfloat far_plane = 80.f;
    const GLfloat aspect = (GLfloat) window_width / (GLfloat) window_height;
    glm::mat4 projection = glm::perspective(45.f, aspect, near_plane, far_plane);
    lights = {
            {{10,  15, 15}, {1, 0, 0}},
            {{-10, 15, 15}, {1, 1, 1}},
    };
    camera = std::make_shared<Camera>(Camera{{-15, 15, -5}, {0, 0, 0}});


    GLFWwindow *window = initGLWindow(window_width, window_height, "shadows");
    initGLFW(window);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    Shader stnrd_shader("../shaders/standard_vert.glsl", "../shaders/standard_frag.glsl");
    Shader depth_shader("../shaders/depth_vert.glsl", "../shaders/depth_frag.glsl");
    Shader light_shader("../shaders/standard_vert.glsl", "../shaders/light_frag.glsl");
    Object room = Object("../objects/myroom2.obj");
    Object cube = Object("../objects/cube.obj");

    GLuint depthMapFBO = 0;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    for (auto &light: lights) {
        light.genDepthMap(shadow_res);
        light.genLightSpace(projection);
    }

    stnrd_shader.setInt("numLights", (int) lights.size());

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glViewport(0, 0, shadow_res, shadow_res);

        for (auto &light: lights) {
            depth_shader.setMat4("model", identity_m);
            depth_shader.setMat4("lightSpace", light.getLightSpace());


            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light.getDepthMap(), 0);
            glClear(GL_DEPTH_BUFFER_BIT);
            room.render(depth_shader);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        glViewport(0, 0, window_width, window_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = camera->lookAt();

        stnrd_shader.setMat4("projection", projection);
        stnrd_shader.setMat4("view", view);
        stnrd_shader.setMat4("model", identity_m);
        light_shader.setMat4("projection", projection);
        light_shader.setMat4("view", view);

        for (int i = 0; i < lights.size(); ++i) {
            Light light = lights[i];

            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, light.getDepthMap());

            const std::string prefix = "lights[" + std::to_string(i) + "].";
            stnrd_shader.setInt(prefix + "enabled", light.is_enabled);
            stnrd_shader.setVec3(prefix + "lightPos", light.getPosition());
            stnrd_shader.setVec3(prefix + "lightColor", light.getColor());
            stnrd_shader.setMat4(prefix + "lightSpace", light.getLightSpace());
            stnrd_shader.setInt(prefix + "shadowMap", i);

            light_shader.setMat4("model", light.getModel());
            light_shader.setVec3("lightColor", light.getColor());
            cube.render(light_shader);
        }

        room.render(stnrd_shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

bool isDragging = false;
double dragX = -1, dragY = -1;

void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        camera->translate(Camera::Direction::FORWARD);
    } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        camera->translate(Camera::Direction::BACKWARD);
    } else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        camera->translate(Camera::Direction::LEFT);
    } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        camera->translate(Camera::Direction::RIGHT);
    }
}

void glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        isDragging = action == GLFW_PRESS;
        if (not isDragging)
            dragX = dragY = -1;
    }
}

void glfw_cursor_position_callback(GLFWwindow *window, double xPos, double yPos) {
    if (isDragging) {
        if (dragX != -1 && dragY != -1) {
            camera->rotate(xPos - dragX, yPos - dragY);
        }
        dragX = xPos;
        dragY = yPos;
    }
}
