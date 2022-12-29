/*
 * Music.cpp
 *
 *  Created on: 02.06.2019
 *      Author: stefan
 */

#include <engine/core/Music.h>
#include <stdexcept>
#include <engine/utils/logger.h>

namespace core
{

    Music::Music()
        : _data(nullptr)
    {
    }

    Music::~Music()
    {
        Mix_FreeMusic(_data);
        _data = nullptr;
    }

} /* namespace core */

void core::Music::loadMusic(std::string filename)
{
    SGL_LOG_TRACE("load music %s", filename);
    _data = Mix_LoadMUS(filename.c_str());
    if (_data == nullptr)
    {
        SGL_LOG_ERROR("error load music file %s: %s", filename, Mix_GetError());
        throw std::runtime_error(
            std::string("failed to load music: ") + std::string(Mix_GetError()));
    }
}

void core::Music::play(int repeat)
{
    Mix_PlayMusic(_data, repeat);
}

void core::Music::pause()
{
    Mix_PauseMusic();
}

void core::Music::resume()
{
    Mix_ResumeMusic();
}

void core::Music::stop()
{
    Mix_HaltMusic();
}

void core::Music::setVolume(const int volume)
{
    Mix_VolumeMusic(volume);
}

bool core::Music::isMusicPlaying()
{
    return Mix_PlayingMusic() == 1;
}
