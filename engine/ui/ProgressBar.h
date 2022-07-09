#pragma once
#include "Object.h"

namespace UI
{
    class ProgressBar : public Object
    {
    public:
        ProgressBar(Object *parent = nullptr);
        ProgressBar(Object *parent, int pWidth, int pHeight);
        ~ProgressBar();

        virtual void render(core::Renderer *renderer);
        virtual void postRender(core::Renderer *renderer);
        virtual bool handleEvents(core::Input *pInput);

        void setMinValue(int minValue);
        void setMaxValue(int maxValue);
        void setCurrentValue(int currentValue);
        int getCurrentValue();

    private:
        void init();
        SDL_Color m_backgroundColor;
        SDL_Color m_borderColor;
        SDL_Color m_progressColor;
        int m_minValue;
        int m_maxValue;
        int m_currentValue;
    };

} // namespace UI
