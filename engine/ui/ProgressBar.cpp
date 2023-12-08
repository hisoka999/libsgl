#include "ProgressBar.h"
#include "engine/graphics/TextureManager.h"
#include <cmath>
namespace UI
{
    ProgressBar::ProgressBar(Object *parent)
        : UI::Object(parent), m_minValue(0), m_maxValue(100), m_currentValue(0)
    {
        init();
    }

    ProgressBar::ProgressBar(Object *parent, int pWidth, int pHeight)
        : UI::Object(parent, pWidth, pHeight), m_minValue(0), m_maxValue(100), m_currentValue(0)
    {
        init();
    }

    ProgressBar::~ProgressBar()
    {
    }

    void ProgressBar::render(core::Renderer *renderer)
    {
        renderer->setDrawColor(m_backgroundColor);
        renderer->drawRect(displayRect());

        renderer->setDrawColor(m_borderColor);
        renderer->fillRect(displayRect());

        graphics::Rect progressRect = displayRect();
        progressRect.x++;
        progressRect.y++;
        progressRect.width = std::max(float((float(m_currentValue - m_minValue) / (m_maxValue - m_minValue)) * progressRect.width) - 2, 0.0f);
        progressRect.height -= 2;

        renderer->setDrawColor(m_progressColor);
        renderer->fillRect(progressRect);
    }

    void ProgressBar::postRender([[maybe_unused]] core::Renderer *renderer)
    {
    }

    bool ProgressBar::handleEvents(core::Input *pInput)
    {
        auto _eventRect = eventRect();
        if (_eventRect.intersects(pInput->getMousePostion()))
        {
            if (pInput->isMouseButtonPressed(SDL_BUTTON_LEFT))
            {
                float relativeCurrentValue = (pInput->getMousePostion().getX() - _eventRect.x) / _eventRect.width;
                m_currentValue = (relativeCurrentValue * m_maxValue) + m_minValue;
                return true;
            }
        }

        return false;
    }

    void ProgressBar::setMinValue(int minValue)
    {
        m_minValue = minValue;
    }

    void ProgressBar::setMaxValue(int maxValue)
    {
        m_maxValue = maxValue;
    }

    void ProgressBar::setCurrentValue(int currentValue)
    {
        m_currentValue = currentValue;
    }

    int ProgressBar::getCurrentValue()
    {
        return m_currentValue;
    }

    void ProgressBar::init()
    {
        setObjectName("ProgressBar");
        if (getTheme() == nullptr)
        {
            setTheme(graphics::TextureManager::Instance().getDefaultTheme());
        }

        m_borderColor = getTheme()->getStyleColor(this, UI::StyleType::BorderColor);
        m_backgroundColor = getTheme()->getStyleColor(this, UI::StyleType::BackgroundColor);
        m_progressColor = getTheme()->getStyleColor(this, UI::StyleType::ForgroundColor);
    }
    void ProgressBar::setBackgroundColor(SDL_Color color)
    {
        m_backgroundColor = color;
    }

    void ProgressBar::setBorderColor(SDL_Color color)
    {
        m_borderColor = color;
    }

    void ProgressBar::setProgressColor(SDL_Color color)
    {
        m_progressColor = color;
    }
}