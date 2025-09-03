#include "se_pch.h"
#include "GLFWApplication.h"
#include <GLFW/glfw3.h>

#include "Storm/Core/Input.h"
#include "Storm/Engine/Events/ApplicationEvent.h"
#include "Storm/Engine/Events/KeyEvent.h"
#include "Storm/Engine/Events/MouseEvent.h"


#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_dx12.h>

namespace Storm
{
    struct APIProp;
    

    Window* Window::Create(const WindowProps& props)
    {
        return new GLFWApplication(props);
    }

    static void GLFWErrorCallback(int error, const char* description)
    {
        SE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    GLFWApplication::GLFWApplication(const WindowProps& props)
    {
        SE_INFO("GLFW Init");
        Init(props);
    }

    GLFWApplication::~GLFWApplication()
    {
        Shutdown();
        SE_INFO("GLFW Close");
    }

    void GLFWApplication::Init(const WindowProps& props)
    {
        {
            m_Data.Width = props.Width;
            m_Data.Height = props.Height;
            m_Data.Title = props.Title;
        }

        int status = glfwInit();
        if (!status)
        {
            glfwTerminate();
            return;
        }
      

#ifdef DIRECTX12
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), NULL, NULL);

        if (!m_Window)
        {
            glfwTerminate();
            return;
        }
        HWND hwnd = glfwGetWin32Window(m_Window);
        m_Context = std::shared_ptr<GraphicsContext>(GraphicsContext::CreateGraphicsContext(APIProp(props.Width, props.Height, hwnd,m_Window)));
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(key, true);
                data.EventCallback(event);
                break;
            }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
                break;
            }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.EventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.EventCallback(event);
        });

        glfwMakeContextCurrent(m_Window);
        glfwSwapInterval(0);

#else

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        gwnd = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), NULL, NULL);
        
        if (!gwnd)
        {
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(gwnd);
        glfwSwapInterval(1);
#endif
    }

    void GLFWApplication::OnUpdate()
    {
        glfwPollEvents();
        m_Context->OnUpdate();
        glfwSwapBuffers(m_Window);
    }


    void GLFWApplication::Shutdown()
    {
        if(m_Window)
            glfwDestroyWindow(m_Window);

        glfwTerminate();

    }

    void GLFWApplication::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool GLFWApplication::IsVSync() const
    {
        return m_Data.VSync;
    }
}
