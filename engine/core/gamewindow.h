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

    class GameWindow
    {
    public:
        static GameWindow &Instance()
        {
            static GameWindow instance;

            return instance;
        }
        int open(std::string pTitle, int pWidth, int pHeight, const std::string &prefDir);

        const SDL_Window *getSDLWindow() const;

        void delay(unsigned int millsec);
        const int getWidth() const { return width; }
        const int getHeight() const { return height; }
        std::shared_ptr<utils::IniBase> getSettings() const;

    protected:
    private:
        /** Default constructor */
        GameWindow();
        /** Default destructor */
        virtual ~GameWindow();

        SDL_Window *win;

        int width;
        int height;
        std::string title;
        std::shared_ptr<utils::IniBase> settings;
    };

} // namespace core

#endif // CORE_GAMEWINDOW_H
