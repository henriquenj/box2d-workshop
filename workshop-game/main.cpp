
#include <stdio.h>
#include <chrono>
#include <thread>
#include <random>
#include <map>

#include "imgui/imgui.h"
#include "imgui_impl_glfw_game.h"
#include "imgui_impl_opengl3_game.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "draw_game.h"

#include "box2d/box2d.h"
#include "game_context.h"
#include "game_object.h"
#include "text.h"
#include "character.h"
#include "block.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // code for keys here https://www.glfw.org/docs/3.3/group__keys.html
    // and modifiers https://www.glfw.org/docs/3.3/group__mods.html

    if (key == GLFW_KEY_ESCAPE)
    {
        // Quit
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Retrieve GameContext from the GLFW window
    GameContext* context = (GameContext*)glfwGetWindowUserPointer(window);

    // forward key press to all game objects
    for (auto& game_object : context->all_objects)
    {
        game_object->OnKeyPress(key, scancode, action, mods);
    }
}

void MouseMotionCallback(GLFWwindow* window, double xd, double yd)
{
    // get the position where the mouse was pressed
    b2Vec2 ps((float)xd, (float)yd);
    // now convert this position to Box2D world coordinates
    b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);

    // Retrieve GameContext from the GLFW window
    GameContext* context = (GameContext*)glfwGetWindowUserPointer(window);

    // update GameContext with mouse positions
    context->mouse_position_window = ps;
    context->mouse_position_box2d = pw;
}

void MouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods)
{
    // code for mouse button keys https://www.glfw.org/docs/3.3/group__buttons.html
    // and modifiers https://www.glfw.org/docs/3.3/group__buttons.html
    // action is either GLFW_PRESS or GLFW_RELEASE
    double xd, yd;
    // get the position where the mouse was pressed
    glfwGetCursorPos(window, &xd, &yd);
    b2Vec2 ps((float)xd, (float)yd);
    // now convert this position to Box2D world coordinates
    b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);

    // Retrieve GameContext from the GLFW window
    GameContext* context = (GameContext*)glfwGetWindowUserPointer(window);

    // Forward mouse press to all game objects
    for (auto& game_object : context->all_objects)
    {
        game_object->OnMousePress(button, action, mods);
    }
}

class GameContactListener : public b2ContactListener
{
public:
    GameContactListener(){};
    virtual ~GameContactListener() = default;

    void BeginContact(b2Contact* contact) override
    {
        // if any of the two colliding bodies have no userData (i.e. they are
        // not PhysicalGameObjects), stop processing
        if (contact->GetFixtureA()->GetBody()->GetUserData().pointer == NULL ||
            contact->GetFixtureB()->GetBody()->GetUserData().pointer == NULL
            ) {
            // stop processing
            return;
        }
        // retrieve our two PhysicalGameObject
        PhysicalGameObject* object1 = (PhysicalGameObject*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        PhysicalGameObject* object2 = (PhysicalGameObject*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
        // call our new function
        object1->OnCollision(object2, contact);
        object2->OnCollision(object1, contact);

    }
};


int main()
{
    GameContext context;

    // glfw initialization things
    if (glfwInit() == 0) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // get main monitor
    GLFWmonitor* mainMonitor = glfwGetPrimaryMonitor();
    // make this window fullscreen, based on the GLFW doc here
    // https://www.glfw.org/docs/3.3/window_guide.html#window_windowed_full_screen
    const GLFWvidmode* mode = glfwGetVideoMode(mainMonitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    context.mainWindow = glfwCreateWindow(mode->width, mode->height, "Shooting blocks!", mainMonitor, NULL);
    // update size on the camera object
    g_camera.m_width = mode->width;
    g_camera.m_height = mode->height;

    if (context.mainWindow == NULL) {
        fprintf(stderr, "Failed to open GLFW g_mainWindow.\n");
        glfwTerminate();
        return -1;
    }

    // Set callbacks using GLFW
    glfwSetMouseButtonCallback(context.mainWindow, MouseButtonCallback);
    glfwSetKeyCallback(context.mainWindow, KeyCallback);
    glfwSetCursorPosCallback(context.mainWindow, MouseMotionCallback);

    // send a pointer of the GameContext object to GLFW so we can retrieve it
    // during the callbacks
    glfwSetWindowUserPointer(context.mainWindow, &context);

    glfwMakeContextCurrent(context.mainWindow);

    // Load OpenGL functions using glad
    int version = gladLoadGL(glfwGetProcAddress);

    // Setup Box2D world and with some gravity
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    context.world = new b2World(gravity);

    // Create debug draw. We will be using the debugDraw visualization to create
    // our games. Debug draw calls all the OpenGL functions for us.
    g_debugDraw.Create();
    context.world->SetDebugDraw(&g_debugDraw);
    CreateUI(context.mainWindow, 40.0f /* font size in pixels */);

    // register contact listener
    GameContactListener contact_listener;
    context.world->SetContactListener(&contact_listener);


    // Some starter objects are created here, such as the ground
    b2Body* ground;
    b2EdgeShape ground_shape;
    ground_shape.SetTwoSided(b2Vec2(-4000.0f, 0.0f), b2Vec2(4000.0f, 0.0f));
    b2BodyDef ground_bd;
    ground = context.world->CreateBody(&ground_bd);
    ground->CreateFixture(&ground_shape, 0.0f);

    // Create Character object
    context.all_objects.push_back(std::make_unique<Character>(&context));
    // store raw pointer to Character as well, which is a bit ugly
    Character* character = dynamic_cast<Character*>(context.all_objects.back().get());


    // This is the color of our background in RGB components
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Control the frame rate. One draw per monitor refresh.
    std::chrono::duration<double> frameTime(0.0);
    std::chrono::duration<double> sleepAdjust(0.0);

    // to create timers, store a frame counter that goes from to 0, and then
    // back goes back to the starting amount (depending on level).
    int frame_counter = 80 /* spawn time at level 1 */;

    bool is_game_over = false;
    // Main application loop
    while (!glfwWindowShouldClose(context.mainWindow)) {
        // Use std::chrono to control frame rate. Objective here is to maintain
        // a steady 60 frames per second (no more, hopefully no less)
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

        glfwGetWindowSize(context.mainWindow, &g_camera.m_width, &g_camera.m_height);

        int bufferWidth, bufferHeight;
        glfwGetFramebufferSize(context.mainWindow, &bufferWidth, &bufferHeight);
        glViewport(0, 0, bufferWidth, bufferHeight);

        // Clear previous frame (avoid creates shadows)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Setup ImGui attributes so we can draw text on the screen. Basically
        // create a window of the size of our viewport.
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(float(g_camera.m_width), float(g_camera.m_height)));
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
        ImGui::End();

        // Enable objects to be draw
        uint32 flags = 0;
        flags += b2Draw::e_shapeBit;
        g_debugDraw.SetFlags(flags);

        frame_counter--;
        if (frame_counter <= 0 && is_game_over == false)
        {
            // timer has expired, reset it. The time it takes to create a block
            // is related to the current level. TODO: this search can be moved
            // to level change instead (it does not need to be done at every
            // timer expiration)
            static const std::map<int /* level */, int /* block creation interval*/> level_to_respawn_time = {
                {1, 80}, {2, 70}, {3, 60}, {4, 50}, {5, 40}, {6, 30}, {7, 20}, {8, 10}};
            auto respawn_time_it = level_to_respawn_time.find(context.level);
            if (respawn_time_it != level_to_respawn_time.end())
            {
                // use respawn time from the map
                frame_counter = respawn_time_it->second;
            }
            else
            {
                // respawn time is not defined, we are probably on a bigger
                // level than 8, so use 5 from now on
                frame_counter = 5;
            }
            // Create a block
            context.all_objects.push_back(std::make_unique<Block>(&context, character->GetPosition().x));
        }

        // draw points on the corner of the screen
        std::string lives_text = "Lives: " + std::to_string(context.lives);
        g_debugDraw.DrawString(10, 10, ImColor(0.35f, 0.73f, 0.87f), lives_text.c_str());
        std::string level = "Level: " + std::to_string(context.level);
        g_debugDraw.DrawString(10, 200, ImColor(0.35f, 0.73f, 0.87f), level.c_str());

        if (is_game_over == false)
        {
            // if lives is 20, reset it to 10 and then increase level
            if (context.lives >= 20)
            {
                context.lives = 10;
                context.level++;
                context.all_objects.push_back(std::make_unique<FloatingText>(&g_debugDraw,
                                                                             character->GetPosition().x,
                                                                             character->GetPosition().y,
                                                                             "Faster!",
                                                                             ImColor(0.00, 0.85, 0.00f),
                                                                             0.1f /* speed */,
                                                                             120 /* ttl */));
            }
            else if (context.lives <= 0)
            {
                std::string game_over_text = "Game over! You reached level " + std::to_string(context.level);
                // get Character position because we are about to delete it (I
                // shouldn't have used std::unique_ptr::get()
                b2Vec2 char_pos = character->GetPosition();
                // game over! remove all objects
                context.all_objects.clear();
                context.to_create.clear();
                // add game over text with score
                context.all_objects.push_back(std::make_unique<Text>(&g_debugDraw,
                                                                     char_pos.x - 10, 25,
                                                                     game_over_text,
                                                                     ImColor(0.00, 0.85, 0.5f)));
                is_game_over = true;
            }
        }

        // When we call Step(), we run the simulation for one frame
        float timeStep = 60 > 0.0f ? 1.0f / 60 : float(0.0f);
        context.world->Step(timeStep, 8, 3);

        // Call update on all game objects
        for (auto& object : context.all_objects)
        {
            object->Update();
        }

        // Check if objects should be deleted. Remove all objects from our
        // vector that have shouldDelete as on. This code is based on the
        // erase-remove idiom. References https://stackoverflow.com/q/347441 and
        // https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom
        context.all_objects.erase(std::remove_if(context.all_objects.begin(), context.all_objects.end(),
                                                 [](const std::unique_ptr<GameObject>& object) { return object->ShouldDelete(); }),
                                  context.all_objects.end());

        // Create all objects that were created during the update function
        for (auto& create : context.to_create)
        {
            context.all_objects.push_back(std::move(create));
        }
        context.to_create.clear();

        // Render everything on the screen
        context.world->DebugDraw();
        g_debugDraw.Flush();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(context.mainWindow);

        // Process events (mouse and keyboard) and call the functions we
        // registered before.
        glfwPollEvents();

        // Throttle to cap at 60 FPS. Which means if it's going to be past
        // 60FPS, sleeps a while instead of doing more frames.
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
        std::chrono::duration<double> target(1.0 / 60.0);
        std::chrono::duration<double> timeUsed = t2 - t1;
        std::chrono::duration<double> sleepTime = target - timeUsed + sleepAdjust;
        if (sleepTime > std::chrono::duration<double>(0)) {
            // Make the framerate not go over by sleeping a little.
            std::this_thread::sleep_for(sleepTime);
        }
        std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();
        frameTime = t3 - t1;

        // Compute the sleep adjustment using a low pass filter
        sleepAdjust = 0.9 * sleepAdjust + 0.1 * (target - frameTime);

    }

    // Terminate the program if it reaches here
    glfwTerminate();
    g_debugDraw.Destroy();
    // objcts need to be deleted before the world is deleted
    context.all_objects.clear();
    // destroy world
    delete context.world;

    return 0;
}
