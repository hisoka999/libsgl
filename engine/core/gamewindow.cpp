#include "engine/core/gamewindow.h"
#include "engine/utils/exceptions.h"
#include <SDL2/SDL_mixer.h>
#include <engine/utils/os.h>
#include <engine/utils/logger.h>
#include <fstream>
#include <iostream>

namespace core
{
    GameWindow::GameWindow(std::string pTitle, const int pWidth, const int pHeight, const std::string &prefDir)
    {
        settings = std::make_shared<utils::IniBase>();
        width = pWidth;
        height = pHeight;
        title = pTitle;

        if (!utils::os::is_dir(utils::os::get_pref_dir("", prefDir)))
        {
            utils::os::create_dir(utils::os::get_pref_dir("", prefDir));

            SGL_LOG_TRACE("config dir: " + utils::os::get_pref_dir("", prefDir));
        }

        std::string path = utils::os::combine(utils::os::get_pref_dir("", prefDir), "settings.conf");
        if (!utils::os::is_file(path))
        {
            std::ofstream filestream;
            filestream.open(path, std::ios::out);
            filestream << "[Base]" << std::endl;
            filestream << "Width=" << std::to_string(pWidth) << std::endl;
            filestream << "Height=" << std::to_string(pHeight) << std::endl;
            filestream << "Fullscreen=2" << std::endl;

            SGL_LOG_TRACE(path + " created!");
            filestream.close();
        }
        settings->Setfilename(path);
        settings->read();
        width = settings->getValueI("Base", "Width");
        height = settings->getValueI("Base", "Height");

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
        {
            throw SDLException("GameWindow::SDL_Init");
        }

#ifdef __WIN32
        // SDL_SetHint(SDL_HINT_VIDEO_WIN_D3DCOMPILER,"d3dcompiler_46.dll");
        // sSDL_SetHint(SDL_HINT_RENDER_DIRECT3D11_DEBUG,"1");
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

#endif
#ifdef SDL_HINT_MOUSE_AUTO_CAPTURE
        SDL_SetHint(SDL_HINT_MOUSE_AUTO_CAPTURE, "0");
#endif
        win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                               SDL_WINDOW_SHOWN);
        if (win == nullptr)
        {
            throw SDLException("GameWindow::SDL_CreateWindow");
        }
        if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
        {
            throw SDLException("GameWindow::IMG_Init");
        }

        if (TTF_Init() != 0)
        {
            throw SDLException("GameWindow::TTF_Init");
        }

        // Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            throw SDLException(
                "SDL_mixer could not initialize! SDL_mixer Error:" + std::string(Mix_GetError()));
        }
        FullScreenMode fullscreen = (FullScreenMode)settings->getValueI("Base", "Fullscreen");
        setFullScreen(fullscreen);
    }

    GameWindow::~GameWindow()
    {
        if (icon != nullptr)
        {
            SDL_FreeSurface(icon);
        }

        if (win != nullptr)
        {
            SDL_DestroyWindow(win);
            Mix_CloseAudio();
            TTF_Quit();
            IMG_Quit();
            Mix_Quit();
            SDL_Quit();
            SGL_LOG_WARN("GameWindow destroyed");
        }
    }
    const SDL_Window *GameWindow::getSDLWindow() const
    {
        return win;
    }

    void GameWindow::delay(unsigned int millsec)
    {
        SDL_Delay(millsec);
    }

    int GameWindow::getWidth() const
    {
        return width;
    }

    int GameWindow::getHeight() const
    {
        return height;
    }

    std::shared_ptr<utils::IniBase> GameWindow::getSettings() const
    {
        return settings;
    }

    void GameWindow::setFullScreen(FullScreenMode mode)
    {
        switch (mode)
        {
        case FullScreenMode::Fullscreen:
            SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
            break;
        case FullScreenMode::FullscreenDesktop:
            SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
            break;
        default:
            SDL_SetWindowFullscreen(win, 0);
            break;
        }
    }

    void GameWindow::setWindowSize(int width, int height)
    {
        SDL_SetWindowSize(win, width, height);
        this->width = width;
        this->height = height;
    }

    void GameWindow::setWindowIcon(const std::string &iconPath)
    {
        icon = IMG_Load(iconPath.c_str());
        SDL_SetWindowIcon(win, icon);
    }

} // namespace core
