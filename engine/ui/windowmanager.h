#ifndef UI_WINDOWMANAGER_H
#define UI_WINDOWMANAGER_H

#include <engine/graphics/texture.h>
#include <engine/ui/Container.h>
#include <engine/ui/dialogs/MessageDialog.h>
#include <vector>

namespace UI
{

    class WindowManager
    {
    public:
        /** Default constructor */
        WindowManager();
        /** Default destructor */
        virtual ~WindowManager();

        void addContainer(Container *c);
        void addWindow(Window *win);

        void render(core::Renderer *ren);
        bool handleInput(core::Input *inp);

        bool isWindowOpen() const;

        UI::dialogs::MessageDialog *showMessageDialog(const std::string &message, const std::string &ok,
                                                      const std::string &cancel)
        {
            messageDialog.setMessage(message);
            messageDialog.setButtonText(ok, cancel);

            messageDialog.setVisible(true);
            return &messageDialog;
        }
        std::shared_ptr<graphics::Text> getFont() { return font; }

        template<typename C>
        C *findContainer()
        {
            for (auto container: containers)
            {
                auto c = dynamic_cast<C *>(container);
                if (c != nullptr)
                {
                    return c;
                }
            }
            return nullptr;
        }

    protected:
        Window *lastActiveWindow() const;

    private:
        std::shared_ptr<graphics::Text> font;

        std::vector<Container *> containers;

        UI::dialogs::MessageDialog messageDialog;

        bool dragActive = false;
    };

} // namespace UI

#endif // UI_WINDOWMANAGER_H
