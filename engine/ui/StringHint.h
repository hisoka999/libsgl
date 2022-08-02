#ifndef __STRINGHINT_H__
#define __STRINGHINT_H__

#include <engine/ui/Hint.h>
#include <engine/graphics/text.h>

namespace UI
{
    class StringHint : public Hint
    {
    public:
        StringHint();
        StringHint(const std::string &hintText);
        ~StringHint();

        virtual void render(core::Renderer *renderer);

    private:
        std::shared_ptr<graphics::Text> font;
    };

} // namespace UI

#endif // __STRINGHINT_H__