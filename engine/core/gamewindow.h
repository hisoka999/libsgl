#ifndef CORE_GAMEWINDOW_H
#define CORE_GAMEWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <engine/utils/IniBase.h>
#include <memory>
#include <string>

namespace core
{
    enum class FullScreenMode
    {
        Fullscreen = 0,
        FullscreenDesktop = 1,
        Windowed = 2
    };
    class GameWindow
    {
    public:
        GameWindow(std::string pTitle, const int pWidth, const int pHeight, const std::string &prefDir);
        /** Default destructor */
        virtual ~GameWindow();

        const SDL_Window *getSDLWindow() const;

        void delay(unsigned int millsec);
        int getWidth() const;
        int getHeight() const;
        std::shared_ptr<utils::IniBase> getSettings() const;
        void setFullScreen(FullScreenMode mode);
        void setWindowSize(int width, int height);
        void setWindowIcon(const std::string &iconPath);

    protected:
    private:
        SDL_Window *win;
        SDL_Surface *icon = nullptr;
        int width;
        int height;
        std::string title;
        std::shared_ptr<utils::IniBase> settings;
    };

} // namespace core

#endif // CORE_GAMEWINDOW_H
