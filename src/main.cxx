#include <iostream>
#include <memory>
#include <chrono>
#include <math.h>


#define GLM_FORCE_RADIANS

//OPENGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "linmath.h"

// #include "ImGuiUtils.h"

#include "baseline/ShaderProgram.h"
// #include "baseline/Gui.h"
// #include "baseline/Renderer.h"
// #include "baseline/Scene.h"
// #include "baseline/Simulation.h"

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include "curve.hpp"
#include "IconsFontAwesome.h"

#define ARCBALL_CAMERA_IMPLEMENTATION
#include "baseline/arcball_camera.h"

#include "baseline/plyloader.h"
#include "baseline/tinyply.h"

#define BUFFER_OFFSET(i) ((char*)NULL +(i))




using namespace tinyply;

typedef std::chrono::time_point<std::chrono::high_resolution_clock> timepoint;
std::chrono::high_resolution_clock c;

inline std::chrono::time_point<std::chrono::high_resolution_clock> now()
{
	return c.now();
}
inline double difference_micros(timepoint start, timepoint end)
{
	return (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}





int mAccumulatedMouseWheel=0;

// static struct
// {
//     float x, y;
//     float r, g, b;
// } vertices[3] =
// {
//     { -0.6f, -0.4f, 1.f, 0.f, 0.f },
//     {  0.6f, -0.4f, 0.f, 1.f, 0.f },
//     {   0.f,  0.6f, 0.f, 0.f, 1.f }
// };
//
//
//






void imgui_init(){
  ImGuiStyle * style = &ImGui::GetStyle();

    style->WindowPadding = ImVec2(15, 15);
    style->WindowRounding = 0.0f;
    style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 8.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 0.85f);
    style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 0.35f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.63f, 0.6f, 0.6f, 0.94f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.63f, 0.6f, 0.6f, 0.94f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
  ImGui::GetIO().Fonts->AddFontFromFileTTF("./deps/imgui/extra_fonts/Roboto-Regular.ttf", 14.0f);
  ImFontConfig config;
  config.MergeMode = true;
  const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
  ImGui::GetIO().Fonts->AddFontFromFileTTF("./deps/imgui/extra_fonts/fontawesome-webfont.ttf", 13.0f, &config, icon_ranges);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
  // std::cout << "scroll " << xoffset << " " << yoffset << '\n';
  mAccumulatedMouseWheel+=yoffset;
}

static void error_callback(int error, const char* description){
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int, char**){


    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Viewer", NULL, NULL);
    GLuint vbo,ibo,vao;
    GLint mvp_location, vpos_location, vcol_location;
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);




    //Imgui
    ImGui_ImplGlfwGL3_Init(window, true);
    imgui_init();



    // //Dummy data
    // std::vector<glm::vec3> positions;
    // std::vector<glm::vec3> colors;
    // std::vector<glm::ivec3> faces;
    //
    // positions.push_back(glm::vec3(-0.6f, -0.4f, 0.f));
    // positions.push_back(glm::vec3( 0.6f, -0.4f, 0.f));
    // positions.push_back(glm::vec3( 0.f,  0.6f, 0.f));
    //
    // colors.push_back(glm::vec3(1.f, 0.f, 0.f ));
    // colors.push_back(glm::vec3( 0.f, 1.f, 0.f ));
    // colors.push_back(glm::vec3(0.f, 0.f, 1.f ));
    //
    // faces.push_back(glm::ivec3(0,1,2));


    //Load ply
    // std::string file_path = "/media/alex/Data/Master/SHK/Data/euroc/mve_scene_inpainted/surface-L2-clean.ply";
    // std::string file_path = "/media/alex/Data/Master/SHK/Data/euroc/mesh.ply"; //The one that first failed to load
		std::string file_path = "/media/alex/Data/Master/SHK/Data/euroc/surface-L1-clean2.ply"; //The one wit 500mb
    // std::string file_path = "/media/alex/Data/Master/SHK/Data/Custom/custom_cyl_1.ply"; //doesnt correctly open since it doesnt have color
    // std::string file_path = "/media/alex/Data/Master/SHK/Data/New_data/ply_3/optim_colored_o4.ply";
    // PLYModel mesh (file_path.data(),false,true);
    // PLYModel mesh (file_path.data(),false,true);
    // // std::cout << "color  "<< mesh.colors[0][0] << '\n';
    // std::cout << "bounds is "  << mesh.bvWidth << " "  << mesh.bvHeight << " " <<mesh.bvDepth << '\n';
    // std::cout << "it is a mesh " << mesh.isMesh << '\n';
    //
    // for (size_t i = 0; i < 10; i++) {
    //   std::cout << " positions is " << mesh.positions[i][0] << " " << mesh.positions[i][1] << " " << mesh.positions[i][2] << '\n';
    // }



    std::vector<float> verts;
    std::vector<uint8_t> colors;
    std::vector<uint32_t> faces;
    //second way of reading ply file
    // Tinyply can and will throw exceptions at you!
  	try{
  		std::ifstream ss(file_path, std::ios::binary);
  		PlyFile file(ss);



  		uint32_t vertexCount, normalCount, colorCount, faceCount, faceTexcoordCount, faceColorCount;
  		vertexCount = normalCount = colorCount = faceCount = faceTexcoordCount = faceColorCount = 0;


  		vertexCount = file.request_properties_from_element("vertex", { "x", "y", "z" }, verts);
  		colorCount = file.request_properties_from_element("vertex", { "red", "green", "blue", "alpha" }, colors);
  		faceCount = file.request_properties_from_element("face", { "vertex_indices" }, faces, 3);

  		// Now populate the vectors...
  		timepoint before = now();
  		file.read(ss);
  		timepoint after = now();

      // for (size_t i = 0; i < 10; i++) {
      //   std::cout << "verts is" << verts[i] << '\n';
      // }

  		// Good place to put a breakpoint!
  		std::cout << "Parsing took " << difference_micros(before, after) << "μs: " << std::endl;
  		std::cout << "\tRead " << verts.size() << " total vertices (" << vertexCount << " properties)." << std::endl;
  		std::cout << "\tRead " << colors.size() << " total vertex colors (" << colorCount << " properties)." << std::endl;
  		std::cout << "\tRead " << faces.size() << " total faces (triangles) (" << faceCount << " properties)." << std::endl;
    }
    catch (const std::exception & e)
    {
      std::cerr << "Caught exception: " << e.what() << std::endl;
    }

    std::vector <float> colors_gl(colors.size());
    for (size_t i = 0; i < colors.size(); i++) {
      colors_gl[i]=colors[i]/255.0f;
    }
    colors.clear();


    // for (size_t i = 0; i < verts.size(); i++) {
    //   if (std::fabs(verts[i])>100){
		// 		std::cout << "deleting points" << '\n';
    //     verts[i]=0.0;
    //   }
    // }
		// return 1;













    //buffers
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    unsigned int size_positions=verts.size()*sizeof(float);
    unsigned int size_colors=colors_gl.size()*sizeof(float);
    unsigned int size_vbo= size_positions + size_colors;
    unsigned int size_ibo= faces.size() *sizeof(uint32_t);
    // unsigned int size_ibo= 1000 *sizeof(glm::ivec3);
    std::cout << "size positions= " <<size_positions << '\n';
    std::cout << "size colors= " <<size_colors << '\n';
    std::cout << "size vbo= " << size_vbo << '\n';
    std::cout << "size ibo= " << size_ibo << '\n';
    glBufferData(GL_ARRAY_BUFFER, size_vbo  , nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size_positions, verts.data());
    verts.clear();
    glBufferSubData(GL_ARRAY_BUFFER, size_positions,
                size_colors, colors_gl.data());
    colors_gl.clear();




    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_ibo, faces.data(), GL_STATIC_DRAW);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces_2), faces_2, GL_DYNAMIC_DRAW);


    //Shaders
    std::shared_ptr<ShaderProgram> shader_program (new ShaderProgram);
    shader_program->initFromFiles("../shaders/vert_shader.glsl", "../shaders/frag_shader.glsl");
    shader_program->addUniform("MVP");
    shader_program->addAttribute("vCol");
    shader_program->addAttribute("vPos");
    mvp_location = shader_program->uniform ("MVP");
    vpos_location = shader_program->attribute ( "vPos");
    vcol_location = shader_program->attribute ( "vCol");
    glEnableVertexAttribArray(vpos_location);
    // glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) 0);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vcol_location);
    // glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,sizeof(float) * 5, (void*) (sizeof(float) * 2));
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,0 , BUFFER_OFFSET(size_positions)  );


    glm::vec4 m_clear_color;
    bool mFirstUpdate=true;
    glm::vec3 pos= glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 target = glm::vec3(0.0f);
    glm::vec3 across = glm::cross(target - pos, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 up = glm::normalize(glm::cross(across, target - pos));
    float fov_y = glm::radians(70.0f);
    unsigned int mLastUpdateTick=0;
    double mLastMouseX=0.0, mouseX=0.0;
    double mLastMouseY=0.0, mouseY=0.0;
    glfwSetScrollCallback(window, scroll_callback);








    //-------------------LOOP-------------------------
    std::cout << "entering loop" << '\n';


    while(true){
      glfwPollEvents();
      ImGui_ImplGlfwGL3_NewFrame();
      int width, height;
      float ratio;
      mat4x4 m, p, mvp;
      glfwGetFramebufferSize(window, &width, &height);
      ratio = width / (float) height;
      glViewport(0, 0, width, height);

      //If you don't clear it will keep appending to the framebuffer so maybe this is a way to draw each triangle individually
      glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w);

			// glClearDepth(1.0f);
			// glDepthFunc(GL_LESS);
			// glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);

      glCullFace(GL_BACK  /* or GL_BACK or even GL_FRONT_AND_BACK */);




      unsigned int currentTick = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      glfwGetCursorPos(window,&mouseX, &mouseY);
      if (mFirstUpdate){
        mLastUpdateTick = currentTick;
        mLastMouseX = mouseX;
        mLastMouseY = mouseY;
      }
      float dtSec = (currentTick - mLastUpdateTick) / 1000.0f;




      //update camera
      arcball_camera_update(
                  glm::value_ptr(pos),
                  glm::value_ptr(target),
                  glm::value_ptr(up),
                  nullptr,
                  dtSec,
                  0.2f, // zoom per tick
                  0.5f, // pan speed
                  3.0f, // rotation multiplier
                  width, height, // screen (window) size
                  mLastMouseX, mouseX,
                  mLastMouseY, mouseY,
                  glfwGetMouseButton(window,1),
                  glfwGetMouseButton(window,0),
                  mAccumulatedMouseWheel,
                  0);



      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::ColorEdit3("clear color", (float*)&m_clear_color);
      // ImGui::SliderIng("float", &mAccumulatedMouseWheel, 0, 10);



      shader_program->use();

      mat4x4_identity(m);
      mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
      glm::mat4 P;
         {
             float f = 1.0f / tanf(fov_y / 2.0f);
             P = glm::mat4(
                 f / ratio, 0.0f, 0.0f, 0.0f,
                 0.0f, f, 0.0f, 0.0f,
                 0.0f, 0.0f, 0.0f, -1.0f,
                 0.0f, 0.0f,  0.01f, 0.0f);
        }

      glm::mat4 V = glm::lookAt(pos,target, up);
      glm::mat4 VP = P * V;
      glm::mat4 MVP = VP ;  //don't have yet a M matrix

      mat4x4_mul(mvp, p, m);



      // //second matrix
      // const float *pSource = (const float*)(p);
      // for (int i = 0; i < 16; ++i){
      //   if (i%4==0){
      //     std::cout  << '\n';
      //   }
      //   std::cout <<  " " <<  pSource[i];
      // }
      // std::cout << '\n';
      // std::cout << '\n';
      // std::cout << '\n';



      glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr (MVP));
      // glDrawArrays(GL_TRIANGLES, 0, 3);

      glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0 );
      // glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, 0 );








      ImGui::Render();
      glfwSwapBuffers(window);



      mFirstUpdate = false;
      mLastUpdateTick = currentTick;
      mAccumulatedMouseWheel = 0;
      mLastMouseX = mouseX;
      mLastMouseY = mouseY;
    }



    // GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    // GLint mvp_location, vpos_location, vcol_location;
    //
    // // glfwSwapInterval(0);
    //
    //
    //
    //
    //
    //
    //
    // GLuint VertexArrayID;
    // glGenVertexArrays(1, &VertexArrayID);
    // glBindVertexArray(VertexArrayID);
    //
    //
    // glGenBuffers(1, &vertex_buffer);
    // glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    //
    //
    // glEnableVertexAttribArray(shader_program->attribute("vPos"));
    // glVertexAttribPointer(shader_program->attribute("vPos"), 2, GL_FLOAT, GL_FALSE,
    //                      sizeof(float) * 5, (void*) 0);
    // glEnableVertexAttribArray(shader_program->attribute("vCol"));
    // glVertexAttribPointer(shader_program->attribute("vCol"), 3, GL_FLOAT, GL_FALSE,
    //                      sizeof(float) * 5, (void*) (sizeof(float) * 2));
    //
    // ImVec4 clear_color = ImColor(114, 144, 154);
    //
    //
    //
    //
    //
    // // Main loop
    //
    // float speed=0.0;
    // float translate_x=0.0,translate_y=0.0,translate_z=0.0;
    // float rotate_x=0.0;
    //
    // while (!glfwWindowShouldClose(window))
    // {
    //   // printf("%s\n", glGetString(GL_VERSION));
    //     glfwPollEvents();
    //     ImGui_ImplGlfwGL3_NewFrame();
    //     ImGui::Text("%s Search", ICON_FA_SEARCH);
    //     gui->update();
    //
    //
    //
    //
    //     float ratio;
    //     int width, height;
    //     mat4x4 m, p, mvp;
    //     glfwGetFramebufferSize(window, &width, &height);
    //     ratio = width / (float) height;
    //     glViewport(0, 0, width, height);
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    //     mat4x4_identity(m);
    //
    //
    //     // if (ImGui::SliderFloat("speed", &speed, 0.0f, 10.0f)){
    //     //   std::cout << "value has been modified" << '\n';
    //     // }
    //     // ImGui::SliderFloat("translate_x", &translate_x, -10.0f, 10.0f);
    //     // ImGui::SliderFloat("translate_y", &translate_y, -10.0f, 10.0f);
    //     // ImGui::SliderFloat("translate_z", &translate_z, -10.0f, 10.0f);
    //     // ImGui::SliderFloat("rotate_x", &rotate_x, -10.0f, 10.0f);
    //
    //     mat4x4_translate_in_place(m,translate_x, translate_y, translate_z);
    //
    //     mat4x4_rotate_Z(m, m, (float) glfwGetTime()+speed);
    //     mat4x4_rotate_X(m, m, rotate_x);
    //     // mat4x4_rotate_Z(m, m, speed);
    //
    //
    //     mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    //     mat4x4_mul(mvp, p, m);
    //
    //
    //     shader_program->use();
    //     glUniformMatrix4fv(shader_program->uniform("MVP"), 1, GL_FALSE, (const GLfloat*) mvp);
    //
    //     glDrawArrays(GL_TRIANGLES, 0, 3);
    //
    //
    //     ImGui::Render();
    //     // ImGui::GetDrawData();
    //     glfwSwapBuffers(window);
    // }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
